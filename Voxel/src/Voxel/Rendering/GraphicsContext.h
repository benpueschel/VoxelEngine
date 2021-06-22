#pragma once

namespace Voxel {

	class GraphicsContext
	{

	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void Destroy() = 0;

	};

}