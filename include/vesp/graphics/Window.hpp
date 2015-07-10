#pragma once

#include "vesp/Types.hpp"

#include <Windows.h>

namespace vesp { namespace graphics {

	class Window
	{
	public:
		Window(StringPtr title, IVec2 size);
		~Window();

		void SetTitle(StringPtr title);
		void SetPosition(IVec2 position);
		void SetSize(IVec2 size);

		IVec2 GetPosition();
		IVec2 GetSize();
		float GetAspectRatio();

		void* GetSystemRepresentation();

		bool IsFullscreen();

		void Pulse();

	private:
		HWND hwnd_;
	};

} }