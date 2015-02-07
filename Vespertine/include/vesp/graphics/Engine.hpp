#pragma once

#include "vesp/Util.hpp"
#include "vesp/Types.hpp"

#include <atlbase.h>
#include <memory>

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11BlendState;

namespace vesp { namespace graphics {

	class Window;
	class Camera;

	class Engine : public util::GlobalSystem<Engine>
	{
	public:
		Engine(StringPtr title);
		~Engine();

		void Initialize();
		void HandleResize(IVec2 size);
		void Pulse();

		Window* GetWindow();

		void SetBlendingEnabled(bool state);

		static IDXGISwapChain* SwapChain;
		static ID3D11Device* Device;
		static ID3D11DeviceContext* ImmediateContext;

	private:
		void CreateDevice(IVec2 size);
		void CreateDepthStencil(IVec2 size);
		void CreateRenderTargets(IVec2 size);
		void CreateBlendState();
		void CreateTestData();

		void DestroyDepthStencil();
		void DestroyRenderTargets();

		std::unique_ptr<Window> window_;
		std::unique_ptr<Camera> camera_;

		CComPtr<ID3D11RenderTargetView> renderTargetView_;
		CComPtr<ID3D11DepthStencilState> depthStencilState_;
		CComPtr<ID3D11DepthStencilView> depthStencilView_;
		CComPtr<ID3D11BlendState> blendState_;

		util::Timer timer_;
		util::Timer fpsTimer_;
		U32 frameCount_;
	};

} }