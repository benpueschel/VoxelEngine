#include "ContentBrowserPanel.h"

namespace Voxel {

	static constexpr float s_DirectoryRefreshRate = 2000.0f;

	ContentBrowserPanel::ContentBrowserPanel(EditorState& state, std::filesystem::path& assetsDirectory)
		: m_State(state), m_AssetsDirectory(assetsDirectory), Panel("Content Browser")
	{
		SetCurrentDirectory(m_AssetsDirectory);
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin(GetImGuiID().c_str());

		if (m_CurrentDirectory != m_AssetsDirectory)
		{
			if(ImGui::Button("<--"))
				SetCurrentDirectory(m_CurrentDirectory.parent_path());
		}

		for (auto& entry : m_DirectoryEntries)
		{
			std::string filename = entry.path().filename().string();
			//std::string stem = entry.path().filename().stem().string();

			if (entry.is_directory())
			{
				if (ImGui::Button(filename.c_str()))
					SetCurrentDirectory(entry.path());
			}
			else
			{
				ImGui::Button(filename.c_str());
			}
		}

		ImGui::End();

	}

	void ContentBrowserPanel::OnUpdate(Timestep& timestep)
	{
		if (m_DirectoryRefreshTimer.ElapsedMillis() > s_DirectoryRefreshRate)
		{
			m_DirectoryRefreshTimer.Reset();
			FetchDirectoryEntries();
		}
	}

	void ContentBrowserPanel::OnEvent(Event& event)
	{

	}

	void ContentBrowserPanel::SetCurrentDirectory(const std::filesystem::path& directory)
	{
		m_DirectoryRefreshTimer.Reset();
		m_CurrentDirectory = directory;
		FetchDirectoryEntries();
	}

	void ContentBrowserPanel::FetchDirectoryEntries()
	{
		m_DirectoryEntries.clear();

		for (auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			if(entry.exists())
				m_DirectoryEntries.push_back(entry);
		}
	}

}