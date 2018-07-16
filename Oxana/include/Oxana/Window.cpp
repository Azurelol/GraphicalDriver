#include "Window.h"

namespace Oxana
{
  void LogWindow::Update()
  {
    if (buffer.size() == bufferSize)
      buffer.pop_front();

    buffer.push_back(function());
  }

	void LogWindow::Reset()
	{
		buffer.clear();
	}

	void LogWindow::Draw(unsigned index)
	{
		ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
		ImGui::Begin(title.c_str());
		if (!buffer.empty())
			ImGui::TextUnformatted(buffer[index].c_str());
		ImGui::End();
	}

  void NotificationWindow::Update()
  {
    bool canNotify = onCheckCondition();
    if (canNotify && !hasActivated)
    {
      enabled = true;
    }
  }

	void NotificationWindow::Draw(unsigned index)
	{
		if (enabled)
			ImGui::OpenPopup(title.c_str());

		if (ImGui::BeginPopupModal(title.c_str(), &enabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			std::string msg = onNotify();
			ImGui::Text(msg.c_str());

			ImGui::Separator();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::Checkbox("Don't ask me next time", &persistent);
			ImGui::PopStyleVar();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				enabled = false;
				if (persistent) hasActivated = false;
			}
			ImGui::EndPopup();
		}
	}

	void NotificationWindow::Draw(bool & enabled, const std::string& title, const std::string & message, bool& persistent)
	{
		if (enabled)
			ImGui::OpenPopup(title.c_str());

		if (ImGui::BeginPopupModal(title.c_str(), &enabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text(message.c_str());

			ImGui::Separator();

			if (persistent) {
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				ImGui::Checkbox("Don't ask me next time", &persistent);
				ImGui::PopStyleVar();
			}

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				enabled = false;
			}
			ImGui::EndPopup();
		}
	}

	void NotificationWindow::Reset()
	{
		hasActivated = false;
	}









}