#pragma once

#include <glm/glm.hpp>

class Scene;
class Ray;

class ITexture
{
public:
	virtual glm::vec3 GetColor(glm::vec2 textureCoordinates) = 0;
};
