#include "Ray.h"

const float Ray::START_EPSILON = 0.01f;

Ray::Ray(glm::vec3 rayStart, glm::vec3 rayDirection) :
	rayStart(rayStart),
	rayDirection(glm::normalize(rayDirection))
{

}

glm::vec3 Ray::GetRayStart()
{
	return this->rayStart;
}

glm::vec3 Ray::GetRayDirection()
{
	return this->rayDirection;
}

glm::vec3 Ray::GetPointOnRayAtTime(float time)
{
	glm::vec3 pointOnRay = this->rayStart + this->rayDirection * time;
	return pointOnRay;
}
