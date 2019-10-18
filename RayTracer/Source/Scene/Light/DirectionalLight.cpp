#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 lightColor) : 
	GenericLight(lightColor), 
	direction(glm::normalize(direction))
{

}

glm::vec3 DirectionalLight::GetDirectionToLight(glm::vec3 position)
{
	return -this->direction;
}

float DirectionalLight::GetDistanceToLight(glm::vec3 position)
{
	return INFINITY;
}
