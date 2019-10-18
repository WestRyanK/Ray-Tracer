#pragma once

#include <glm/glm.hpp>
#include "GenericBackground.h"
class Ray;

class SolidBackground : public GenericBackground
{
private:
	glm::vec3 backgroundColor;

public:
	SolidBackground(glm::vec3 backgroundColor);
	virtual glm::vec3 GetColorFromRay(Ray ray);
	glm::vec3 GetBackgroundColor();
};
