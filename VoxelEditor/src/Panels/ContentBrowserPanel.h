#pragma once

#include "Panel.h"
#include "../EditorState.h"

#include "Voxel/Core/Timer.h"

#include <filesystem>
#include <vector>

namespace Voxel {

	class ContentBrowserPanel : public Panel
	{
	public:
		// TODO: take a project to extract the assets directory
		ContentBrowserPanel(EditorState& state, std::filesystem::path& assetsDirectory = std::filesystem::path("assets"));

		virtual void OnImGuiRender() override;
		virtual void OnUpdate(Timestep& timestep) override;
		virtual void OnEvent(Event& event) override;

		void SetCurrentDirectory(const std::filesystem::path& directory);
		void FetchDirectoryEntries();
		void LoadIcons(std::filesystem::path& directory);

		Ref<Texture2D>& ContentBrowserPanel::GetFileTexture(const std::string& fileExtension);
		Ref<Texture2D>& ContentBrowserPanel::GetFolderTexture(const std::string& folderName);
	private:
		EditorState& m_State;
		
		std::filesystem::path m_AssetsDirectory;
		std::filesystem::path m_CurrentDirectory{};
		std::filesystem::path m_SelectionContext{};

		std::vector<std::filesystem::directory_entry> m_DirectoryEntries;
		Timer m_DirectoryRefreshTimer;

		std::map<std::string, Ref<Texture2D>> m_Icons;
	};

}