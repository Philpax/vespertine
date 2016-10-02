#include "vesp/Console.hpp"
#include "vesp/Containers.hpp"
#include "vesp/Log.hpp"
#include "vesp/EventManager.hpp"
#include "vesp/Profiler.hpp"

#include "vesp/graphics/Engine.hpp"
#include "vesp/graphics/Window.hpp"

#include "vesp/math/Vector.hpp"

namespace vesp {

	Console::Console()
	{
		this->module_ = std::make_unique<script::Module>("console");

		InputManager::Get()->Subscribe(
			Action::Console, this, &Console::ConsolePress);

		EventManager::Get()->Subscribe(
			"Render.Gui", [&](const void*) { this->Draw(); return true; });

		this->AddCommand("console.history", [&] {
			return sol::as_table(this->history_);
		});

		// TODO: Autoexec
	}

	Console::~Console()
	{
	}

	void Console::PostInitialisation()
	{
		EventManager::Get()->Fire("Console.ReadyForBinding");
	}

	void Console::SetActive(bool active)
	{
		if (!this->active_ && active)
			this->inputNeedsFocus_ = true;

		this->active_ = active;
		
		if (this->active_)
			InputManager::Get()->AddGuiLock();
		else
			InputManager::Get()->RemoveGuiLock();
	}

	bool Console::GetActive() const
	{
		return this->active_;
	}

	void Console::AddMessage(StringView text, graphics::Colour colour)
	{
		this->messages_.push_back({text.CopyToVector(), colour});

		if (this->messages_.size() >= 1024)
			this->messages_.pop_front();

		this->scrollToBottom_ = true;
	}

	void Console::Draw()
	{
		if (!this->active_)
			return;

		VESP_PROFILE_FN();

		Array<StringByte, 256> inputBuffer;	
		inputBuffer.assign(0);

		auto size = graphics::Engine::Get()->GetWindow()->GetSize() / 2;
		ImGui::SetNextWindowSize(
			ImVec2(float(size.x), float(size.y)), ImGuiSetCond_FirstUseEver);

		bool active;
		ImGui::Begin("Console", &active);
		{
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
			{
				for (auto& text : this->messages_)
				{
					auto c = text.colour;
					ImGui::PushStyleColor(ImGuiCol_Text, ImColor(c.r, c.g, c.b, c.a));
					ImGui::TextUnformatted(text.text.data(), text.text.data() + text.text.size());
					ImGui::PopStyleColor();
				}

				if (this->scrollToBottom_)
				{
					ImGui::SetScrollHere();
					this->scrollToBottom_ = false;
				}
			}

			ImGui::EndChild();
			ImGui::Separator();

			ImGui::PushItemWidth(-1.0f);
			if (ImGui::InputText("Input", inputBuffer.data(),
				inputBuffer.size(), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory,
				[](ImGuiTextEditCallbackData *data) { return ((Console*)data->UserData)->TextboxCallback(data); },
				this))
			{
				StringView view = inputBuffer.data();

				if (view.size)
				{
					this->AddMessage(view, graphics::Colour::CornflowerBlue);
					this->Execute(view);
				}

				this->inputNeedsFocus_ = true;
			}
			
			if (this->inputNeedsFocus_)
				ImGui::SetKeyboardFocusHere();
		
			ImGui::PopItemWidth();
		}
		ImGui::End();

		this->inputNeedsFocus_ = false;

		if (!active)
			this->SetActive(false);
	}

	void Console::Execute(StringView code)
	{
		// Store in history
		if (this->history_.empty() || code != this->history_.back())
		{
			this->history_.push_back(code.CopyToVector());
			if (this->history_.size() > MaxHistoryLength)
				this->history_.pop_front();
			this->historyIndex_ = this->history_.size();
		}
				

		// Try loading with return prefixed
		auto codeWithReturn = Concat("return ", code);
		auto parseResult = this->module_->ParseString(codeWithReturn);

		// If that doesn't work, try loading without return
		if (!parseResult.valid())
			parseResult = this->module_->ParseString(code);

		// If it still doesn't work, log and error
		if (!parseResult.valid())
		{
			auto errorStr = parseResult.get<std::string>();
			LogError("Failed to parse console input: %s", errorStr.c_str());
			return;
		}

		// Run the parse
		auto runResult = this->module_->RunParseResult(parseResult);

		// If it's non-existent/nil, don't bother displaying it
		if (!runResult.valid())
			return;

		// Convert it to an object
		sol::object runResultObj = runResult;

		// TODO: Magic conversion magic (automatically calling functions, etc)
		// Removed while waiting on sol2 fix

		// If the resulting object is invalid, don't bother displaying it
		if (!runResultObj.valid())
			return;

		// Display the result
		this->PrettyPrint(runResultObj, 0);
	}

	void Console::PrettyPrint(sol::object obj, size_t level)
	{
		// Generate pad
		auto pad = Repeat(' ', level * 2);

		// If it's a table, pretty-print it
		if (obj.is<sol::table>())
		{
			auto table = obj.as<sol::table>();
			auto nextPad = Repeat(' ', (level + 1) * 2);

			size_t index = 0;
			LogInfo("%.*s{", pad.size(), pad.data());
			table.for_each([&](sol::object const& key, sol::object const& value)
			{
				if (index >= table.size())
					return;
				
				// Print key
				auto keyStr = this->module_->ToString(key);
				LogInfo("%.*s[%.*s] =", nextPad.size(), nextPad.data(), keyStr.size(), keyStr.data());

				// Print value
				this->PrettyPrint(value, level + 2);
				++index;
			});
			LogInfo("%.*s}", pad.size(), pad.data());
		}
		else
		{
			// Otherwise, just print the value out
			auto objStr = this->module_->ToString(obj);
			if (obj.is<RawStringPtr>())
				LogInfo("%.*s\"%.*s\"", pad.size(), pad.data(), objStr.size(), objStr.data());
			else
				LogInfo("%.*s%.*s", pad.size(), pad.data(), objStr.size(), objStr.data());
		}
	}

	void Console::ConsolePress(float state)
	{
		if (state == 1.0f)
			this->SetActive(!this->GetActive());
	}

	int Console::TextboxCallback(ImGuiTextEditCallbackData* data)
	{
		switch (data->EventFlag)
		{
		case ImGuiInputTextFlags_CallbackHistory:
		{
			if (this->history_.empty())
				break;

			switch (data->EventKey)
			{
			case ImGuiKey_UpArrow:
				this->historyIndex_--;
				break;
			case ImGuiKey_DownArrow:
				this->historyIndex_++;
				break;
			}

			// Handle wraparound
			S32 historySize = this->history_.size();
			if (this->historyIndex_ >= historySize)
				this->historyIndex_ -= historySize;
			else if (this->historyIndex_ < 0)
				this->historyIndex_ += historySize;

			auto& currentItem = this->history_[this->historyIndex_];
			strncpy_s(data->Buf, data->BufSize, currentItem.data(), currentItem.size());
			data->BufTextLen = std::min(data->BufSize, int(currentItem.size()));
			data->BufDirty = true;
			data->CursorPos = data->SelectionStart = data->SelectionEnd = data->BufTextLen;
			break;
		}
		}
		return 0;
	}
}
