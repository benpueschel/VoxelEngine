#pragma once

#include "Voxel.h"

namespace Voxel {

	struct EditorState
	{

		Ref<Scene> ActiveScene;
		int GizmoType = -1;
		// TODO: Allow asset selection
		Entity CurrentContext;

	};

}