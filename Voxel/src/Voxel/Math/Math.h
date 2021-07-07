#pragma once

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

using namespace glm;
namespace Voxel::Math {

	static bool NormalizeMatrix(glm::mat4& matrix)
	{
		if (epsilonEqual(matrix[3][3], 0.0f, epsilon<float>()))
			return false;

		for (length_t i = 0; i < 4; ++i)
			for (length_t j = 0; j < 4; ++j)
				matrix[i][j] /= matrix[3][3];

		return true;
	}

	static bool DecomposeRotation(mat4& transform, vec3& rotation)
	{
		mat4 LocalMatrix(transform);

		if (!NormalizeMatrix(LocalMatrix)) return false;

		vec3 Row[3] = { glm::vec3(0.0f) }, Pdum3 = glm::vec3(0.0f);

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		Row[0] = detail::scale(Row[0], 1.0f);
		Row[1] = detail::scale(Row[1], 1.0f);
		Row[2] = detail::scale(Row[2], 1.0f);

		rotation.y = asin(-Row[0][2]);
		if (cos(rotation.y) != 0)
		{
			rotation.x = atan2(Row[1][2], Row[2][2]);
			rotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else
		{
			rotation.x = atan2(-Row[2][0], Row[1][1]);
			rotation.z = 0;
		}

		return true;
	}

	static bool DecomposeTransform(mat4& transform, vec3& position, vec3& rotation, vec3& scale)
	{

		mat4 LocalMatrix(transform);

		// Normalize the matrix.
		if (!NormalizeMatrix(LocalMatrix)) return false;

		// First, isolate perspective.  This is the messiest.
		if (
			epsilonNotEqual(LocalMatrix[0][3], 0.0f, epsilon<float>()) ||
			epsilonNotEqual(LocalMatrix[1][3], 0.0f, epsilon<float>()) ||
			epsilonNotEqual(LocalMatrix[2][3], 0.0f, epsilon<float>()))
		{
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = 0.0f;
			LocalMatrix[3][3] = 1.0f;
		}

		// Next take care of translation (easy).
		position = vec4(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3] = { glm::vec3(0.0f) }, Pdum3 = glm::vec3(0.0f);

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute Scale
		scale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], 1.0f);
		scale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], 1.0f);
		scale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], 1.0f);

		// Extract Rotation
		rotation.y = asin(-Row[0][2]);
		if (cos(rotation.y) != 0)
		{
			rotation.x = atan2(Row[1][2], Row[2][2]);
			rotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else
		{
			rotation.x = atan2(-Row[2][0], Row[1][1]);
			rotation.z = 0;
		}

		return true;
	}

}