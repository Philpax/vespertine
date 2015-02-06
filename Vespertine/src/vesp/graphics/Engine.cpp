#pragma warning(disable: 4005)
#include "vesp/graphics/Engine.hpp"
#include "vesp/graphics/Window.hpp"
#include "vesp/graphics/Shader.hpp"
#include "vesp/graphics/Buffer.hpp"
#include "vesp/graphics/FreeCamera.hpp"
#include "vesp/graphics/Mesh.hpp"

#include "vesp/math/Vector.hpp"
#include "vesp/math/Matrix.hpp"

#include "vesp/Log.hpp"

#include <d3d11.h>

namespace vesp { namespace graphics {

	VertexShader vertexShader("vs");
	PixelShader pixelShader("ps");
	PixelShader gridPixelShader("psGrid");
	Mesh floorMesh;
	Mesh gizmoMesh;

	IDXGISwapChain* Engine::SwapChain;
	ID3D11Device* Engine::Device;
	ID3D11DeviceContext* Engine::ImmediateContext;
	ID3D11RenderTargetView* Engine::RenderTargetView;

	Engine::Engine(StringPtr title)
	{
		this->window_ = std::make_unique<Window>(title, IVec2(1280, 800));
		this->frameCount_ = 0;
	}

	Engine::~Engine()
	{
		RenderTargetView->Release();
		ImmediateContext->Release();
		SwapChain->Release();
		Device->Release();
	}
	
	void Engine::Initialize()
	{
		auto size = this->window_->GetSize();

		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory( &desc, sizeof(desc) );
		desc.BufferCount = 1;
		desc.BufferDesc.Width = size.x;
		desc.BufferDesc.Height = size.y;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.OutputWindow = (HWND)this->window_->GetSystemRepresentation();
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Windowed = !this->window_->IsFullscreen();

		// todo error handling
		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			0, nullptr, 0, D3D11_SDK_VERSION, &desc, &SwapChain,
			&Device, nullptr, &ImmediateContext);

		ID3D11Texture2D* backBuffer;
		// error handling
		SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		Device->CreateRenderTargetView(backBuffer, nullptr, &RenderTargetView);
		backBuffer->Release();
		ImmediateContext->OMSetRenderTargets(1, &RenderTargetView, nullptr);
	
		D3D11_VIEWPORT vp;
		vp.Width = (float)size.x;
		vp.Height = (float)size.y;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		ImmediateContext->RSSetViewports(1, &vp);

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		Vector<StringByte> shaderSource;
	
		FileSystem::Get()->Read("data/shaders/default.vsh", shaderSource);
		vertexShader.Load(shaderSource.data(), layout, util::SizeOfArray(layout));

		FileSystem::Get()->Read("data/shaders/default.psh", shaderSource);
		pixelShader.Load(shaderSource.data());

		FileSystem::Get()->Read("data/shaders/grid.psh", shaderSource);
		gridPixelShader.Load(shaderSource.data());

		Vertex floorVertices[] =
		{
			{Vec3(1.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f)},
			{Vec3(-1.0f, 0.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f)},
			{Vec3(-1.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f)},

			{Vec3(1.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f)},
			{Vec3(1.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f)},
			{Vec3(-1.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f)},
		};
		
		floorMesh.Create(floorVertices, util::SizeOfArray(floorVertices));
		floorMesh.SetScale(Vec3(10, 1, 10));

		Vertex gizmoVertices[] =
		{
			{Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)},
			{Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)},

			{Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)},
			{Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)},

			{Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f)},
			{Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f)},
		};

		gizmoMesh.Create(gizmoVertices, util::SizeOfArray(gizmoVertices), 
			D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		this->camera_ = std::make_unique<FreeCamera>(
			Vec3(0.0f, 2.0f, -4.0f), 
			Quat(Vec3(0.0f, 0.0f, 0.0f))
		);
	}

	void Engine::Pulse()
	{
		this->window_->Pulse();

		float clearColour[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		ImmediateContext->ClearRenderTargetView(RenderTargetView, clearColour);
		
		auto freeCamera = static_cast<FreeCamera*>(this->camera_.get());
		freeCamera->Update();

		vertexShader.Activate();
		gridPixelShader.Activate();

		// Draw floor
		floorMesh.Draw();

		// Draw rotating gizmo
		pixelShader.Activate();
		auto seconds = this->timer_.GetSeconds();
		gizmoMesh.SetPositionAngle(Vec3(1,1,1), Quat(Vec3(0, seconds, 0)));
		gizmoMesh.Draw();

		// Draw stationary gizmo
		gizmoMesh.SetPositionAngle(Vec3(0,1,0), Quat());
		gizmoMesh.Draw();
		
		SwapChain->Present(0, 0);

		this->frameCount_++;
		if (this->fpsTimer_.GetSeconds() > 5)
		{
			LogInfo("FPS: %.01f", this->frameCount_ / this->fpsTimer_.GetSeconds());
			this->fpsTimer_.Restart();
			this->frameCount_ = 0;
		}
	}

	Window* Engine::GetWindow()
	{
		return this->window_.get();
	}

} }