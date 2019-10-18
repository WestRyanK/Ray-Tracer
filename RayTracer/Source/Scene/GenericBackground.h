#pragma once

#include <glm/glm.hpp>

class Ray;

class GenericBackground
{
public:
	GenericBackground() { }
	virtual glm::vec3 GetColorFromRay(Ray ray) = 0;
};
