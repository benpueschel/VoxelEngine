#include "ContentBrowserPanel.h"

namespace Voxel {

	static std::string ToLowerCase(std::string& string)
	{
		std::string str = string;
		std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
			return std::tolower(c);
		});
		return str;
	}

	static constexpr float s_DirectoryRefreshRate = 2000.0f;
	static float s_ColumnWidth = 128.0f;
	static float s_ColumnPadding = 16.0f;

	ContentBrowserPanel::ContentBrowserPanel(EditorState& state, std::filesystem::path& assetsDirectory)
		: m_State(state), m_AssetsDirectory(assetsDirectory), Panel("Content Browser")
	{
		LoadIcons(std::filesystem::path("assets/icons"));
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

		int columns = glm::max<int>(1, std::floor(ImGui::GetContentRegionAvail().x / s_ColumnWidth));

		ImGui::Columns(columns, 0, false);

		for (auto& entry : m_DirectoryEntries)
		{
			std::string filename = entry.path().filename().string();
			//std::string stem = entry.path().filename().stem().string();
			std::string extension = entry.path().extension().string();

			ImVec4 buttonColor = { 0, 0, 0, 0 };

			if (m_SelectionContext == entry.path())
				buttonColor = { 1, 1, 1, 0.1f };

			if (entry.is_directory())
			{
				if (UI::ImageButton(filename.c_str(), GetFolderTexture(ToLowerCase(filename)), glm::vec2(s_ColumnWidth), buttonColor))
					m_SelectionContext = entry.path();
				
				if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					SetCurrentDirectory(entry.path());

				ImGui::TextWrapped(filename.c_str());
			}
			else
			{
				if (UI::ImageButton(filename.c_str(), GetFileTexture(ToLowerCase(extension)), glm::vec2(s_ColumnWidth), buttonColor))
					m_SelectionContext = entry.path();

				//if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				//	OpenFile(entry.path)

				ImGui::TextWrapped(filename.c_str());
			}

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &s_ColumnWidth, 64.0f, 512.0f);
		ImGui::SliderFloat("Padding", &s_ColumnPadding, 0.0f, 64.0f);

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

	void ContentBrowserPanel::LoadIcons(std::filesystem::path& directory)
	{
		for (auto& entry : std::filesystem::directory_iterator(directory))
		{
			if (entry.is_directory()) continue;

			std::string name = entry.path().filename().stem().string();
			m_Icons.insert({ name, Texture2D::Create(entry.path().string()) });
		}
	}

	Ref<Texture2D>& ContentBrowserPanel::GetFileTexture(const std::string& fileExtension)
	{
		if (fileExtension == ".cpp")
			return m_Icons["cpp"];
		if (fileExtension == ".h")
			return m_Icons["h"];
		if (fileExtension == ".hpp")
			return m_Icons["hpp"];
		if (fileExtension == ".cs")
			return m_Icons["csharp"];
		if (fileExtension == ".txt")
			return m_Icons["document"];
		if (fileExtension == ".ttf")
			return m_Icons["font"];
		if (fileExtension == ".fbx")
			return m_Icons["3d"];

		return m_Icons["file"];
	}

	Ref<Texture2D>& ContentBrowserPanel::GetFolderTexture(const std::string& folderName)
	{
		if (folderName == "scripts")
			return m_Icons["folder-scripts-open"];
		if (folderName == "settings")
			return m_Icons["folder-config-open"];
		if (folderName == "fonts")
			return m_Icons["folder-font-open"];

		return m_Icons["folder-open"];
	}

}