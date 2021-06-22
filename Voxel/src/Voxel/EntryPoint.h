#pragma once

#ifdef PLATFORM_WINDOWS

extern Voxel::Application* Voxel::CreateApplication();

int main(int argc, char** argv)
{

	try
	{

		Voxel::Logger::Init();

		LOG_CORE_INFO("Creating Engine Application");

		auto app = Voxel::CreateApplication();

		app->Run();

		delete app;

	}
	catch (std::exception* exception)
	{
		LOG_CORE_FATAL(exception->what());
		return 1;
	}

	return 0;
}

#endif