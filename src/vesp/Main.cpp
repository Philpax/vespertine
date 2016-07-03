#include "vesp/Main.hpp"
#include "vesp/Log.hpp"
#include "vesp/Console.hpp"
#include "vesp/EventManager.hpp"
#include "vesp/FileSystem.hpp"
#include "vesp/InputManager.hpp"

#include "vesp/graphics/Engine.hpp"
#include "vesp/graphics/Window.hpp"

#include "vesp/world/HeightMapTerrain.hpp"

#include "vesp/math/Vector.hpp"

namespace vesp
{
	util::Timer GlobalTimer;

	bool Initialize(RawStringPtr name)
	{
		GlobalTimer.Restart();

		FileSystem::Create();
		Logger::Create("log.txt");

		InputManager::Create();

		Console::Create();
		LogInfo("Vespertine (%s %s)", __DATE__, __TIME__);

		EventManager::Create();

		graphics::Engine::Create(name);
		graphics::Engine::Get()->Initialize();

		world::HeightMapTerrain::Create();

		Console::Get()->GetModule()->GetState().set_function("quit", &vesp::Quit);

		return true;
	}

	void Shutdown()
	{
		world::HeightMapTerrain::Destroy();

		graphics::Engine::Destroy();

		EventManager::Destroy();

		LogInfo("Vespertine shutting down");
		Logger::Destroy();
		Console::Destroy();

		InputManager::Destroy();
		FileSystem::Destroy();
	}

	bool Running = true;

	void Loop()
	{
		while (Running)
		{
			graphics::Engine::Get()->PrePulse();
			InputManager::Get()->Pulse();

			MSG msg{};
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) > 0)
			{
				switch (msg.message)
				{
				case WM_QUIT:
					Quit();
					break;
				}
				
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				InputManager::Get()->FeedEvent(&msg);
			}

			graphics::Engine::Get()->Pulse();
		}
	}

	void Quit()
	{
		Running = false;
		EventManager::Get()->Fire("Engine.Quit");
	}

	util::Timer const& GetGlobalTimer()
	{
		return GlobalTimer;
	}
}