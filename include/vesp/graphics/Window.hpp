#pragma once

#include "vesp/Types.hpp"

#include <Windows.h>

namespace vesp { namespace graphics {

	class Window
	{
	public:
		Window(RawStringPtr title, IVec2 size);
		~Window();

		void SetTitle(RawStringPtr title);
		void SetPosition(IVec2 position);
		void SetSize(IVec2 size);

		void ClipToWindow(bool clip);

		IVec2 GetPosition();
		IVec2 GetSize();
		float GetAspectRatio();

		void* GetSystemRepresentation();

		bool IsFullscreen() const;
		bool HasFocus() const;

		void Pulse();

	private:
		HWND hwnd_;
	};

} }