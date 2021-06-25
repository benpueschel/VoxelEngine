#include "Voxel.h"
#include "Voxel/EntryPoint.h"

#include "EditorLayer.h"

namespace Voxel {

	class Voxel : public Application
	{
	public:
		Voxel()
			: Application("VoxelEditor")
		{
			PushLayer(new EditorLayer());
		}

		~Voxel()
		{

		}

	};

	Application* CreateApplication()
	{
		return new Voxel();
	}

}