#pragma once

#include "vesp/util/GlobalSystem.hpp"
#include "vesp/util/Timer.hpp"

#include "vesp/Containers.hpp"

namespace vesp
{
	class Profiler : public util::GlobalSystem<Profiler>
	{
	public:
		Profiler();

		void BeginSection(RawStringPtr title);
		void EndSection();

		void BeginFrame();
		void EndFrame();

	private:
		void Draw();

		struct Section
		{
			RawStringPtr title;
			Section* parent;
	
			util::Timer timer;
			Vector<UniquePtr<Section>> children;

			F32 duration;
		};

		void DrawSection(Section* section);

		UniquePtr<Section> savedRoot_;
		UniquePtr<Section> root_;
		Section* currentSection_ = nullptr;
		bool drawGui_ = false;
		bool frozen_ = false;
	};

	struct ProfileBlock
	{
		ProfileBlock(RawStringPtr title)
		{
			Profiler::Get()->BeginSection(title);
		}

		~ProfileBlock()
		{
			Profiler::Get()->EndSection();
		}
	};
}

#define VESP_PROFILE_BLOCK(title) vesp::ProfileBlock PB##__LINE__(title)
#define VESP_PROFILE_FN() VESP_PROFILE_BLOCK(__FUNCTION__)