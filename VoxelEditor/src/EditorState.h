#pragma once

#include "Voxel.h"

namespace Voxel {

	struct EditorState
	{

		Ref<Scene> ActiveScene;
		// TODO: Allow asset selection
		Entity CurrentContext;

	};

}