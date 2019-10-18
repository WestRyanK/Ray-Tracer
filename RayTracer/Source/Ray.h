#pragma once

#include <glm/glm.hpp>

class Ray
{
private:
	glm::vec3 rayStart;
	glm::vec3 rayDirection;
public:
	Ray(glm::vec3 rayStart, glm::vec3 rayDirection);
	glm::vec3 GetRayStart();
	glm::vec3 GetRayDirection();
	glm::vec3 GetPointOnRayAtTime(float time);
	static const float START_EPSILON;
};
