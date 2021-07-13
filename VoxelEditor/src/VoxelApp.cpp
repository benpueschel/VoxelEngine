#include "Voxel.h"
#include "Voxel/EntryPoint.h"

#include "EditorLayer.h"

namespace Voxel {

	class Voxel : public Application
	{
	public:
		Voxel(ApplicationCommandLineArgs args)
			: Application("VoxelEditor", args)
		{
			PushLayer(new EditorLayer());
		}

		~Voxel()
		{

		}

	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new Voxel(args);
	}

}