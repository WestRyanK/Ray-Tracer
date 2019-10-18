#include "PointLight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 lightColor, float radius, float decayRatio) : 
	GenericLight(lightColor, decayRatio), 
	position(position),
	radius(radius)
{

}

glm::vec3 PointLight::GetDirectionToLight(glm::vec3 position)
{
	return glm::normalize(this->position - position);
}

float PointLight::GetDistanceToLight(glm::vec3 position)
{
	return glm::length(this->position - position) - radius;
}
