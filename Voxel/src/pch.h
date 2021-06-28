#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <exception>

#include <string>
#include <sstream>

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Voxel/Core/Core.h"
#include "Voxel/Logging/Logger.h"
#include "Voxel/Debug/Assert.h"
#include "Voxel/Debug/Instrumentor.h"

#ifdef VOXEL_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
