#pragma once

#include <glm/glm.hpp>
#include "ITexture.h"

class Scene;
class Ray;

class SolidTexture : public ITexture
{
private:
	glm::vec3 color;
public:
	SolidTexture(glm::vec3 color);
	SolidTexture(float r, float g, float b);
	virtual glm::vec3 GetColor(glm::vec2 textureCoordinates);
};
