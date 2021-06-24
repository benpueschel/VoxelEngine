#include <pch.h>
#include "Voxel.h"
#include "Voxel/EntryPoint.h"

//#include "EditorLayer.h"

namespace Voxel {

	using namespace Voxel;

	class Voxel : public Application
	{
	public:
		Voxel()
		{
			//PushLayer(new EditorLayer());
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