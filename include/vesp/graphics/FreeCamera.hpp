#pragma once

#include "vesp/graphics/Camera.hpp"

namespace vesp { namespace graphics {

	class FreeCamera : public Camera
	{
	public:
		FreeCamera(Vec3 const& position, Quat const& angle);
		virtual ~FreeCamera();

		virtual void Update();

		void SetPosition(Vec3 const& position);
		void SetAngle(Quat const& angle);

		Vec3 GetPosition() const;
		Quat GetAngle() const;

	protected:
		void CalculateMatrices();

		Vec3 position_;
		Quat angle_;
		float pitch_;
		float yaw_;
		float speed_ = 10.0f;
		util::Timer frameDeltaTimer_;
	};

} }