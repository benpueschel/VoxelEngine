#pragma once

#include "Voxel/Scene/ScriptableEntity.h"

namespace Voxel {

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() {
				return static_cast<ScriptableEntity*>(new T());
			};
			DestroyScript = [](NativeScriptComponent* script) {
				delete script;
				script->Instance = nullptr;
			};
		}

		void OnImGuiRender();
	};

}