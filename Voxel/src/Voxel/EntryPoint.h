#pragma once

#ifdef VOXEL_PLATFORM_WINDOWS

extern Voxel::Application* Voxel::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Voxel::Logger::Init();

	LOG_CORE_INFO("Creating Engine Application");

	PROFILE_BEGIN_SESSION("Startup", "VoxelProfile-Startup.json");
	auto app = Voxel::CreateApplication({ argc, argv });
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Runtime", "VoxelProfile-Runtime.json");
	app->Run();
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Shutdown", "VoxelProfile-Shutdown.json");
	delete app;
	PROFILE_END_SESSION();
}

#endif