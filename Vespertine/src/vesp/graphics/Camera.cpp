#pragma warning(disable: 4005)
#include "vesp/graphics/Camera.hpp"
#include "vesp/graphics/Window.hpp"

namespace vesp { namespace graphics {

	Camera::Camera()
		: fov_(60.0f), nearPlane_(0.1f), farPlane_(10000.0f)
	{
		this->CalculateMatrices();

		auto constants = this->MakeConstants();
		this->constantBuffer_.Create(&constants, 1);
	}

	Camera::~Camera()
	{
	}

	void Camera::Update()
	{
		this->CalculateMatrices();

		auto constants = this->MakeConstants();
		this->constantBuffer_.Load(&constants, 1);
		this->constantBuffer_.UseVS(0);
	}

	void Camera::CalculateMatrices()
	{
		this->aspectRatio_ = Engine::Get()->GetWindow()->GetAspectRatio();
		this->projection_ = math::DXPerspective(
			this->fov_, this->aspectRatio_, this->nearPlane_, this->farPlane_);
		this->viewProjection_ = this->projection_ * this->view_;
	}

	Camera::Constants Camera::MakeConstants()
	{
		Constants constants;
		constants.viewProjection = this->viewProjection_;

		return constants;
	}

} }