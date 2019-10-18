#pragma once

#include <glm/glm.hpp>
#include "GenericLight.h"

class DirectionalLight : public GenericLight
{
private:
	glm::vec3 direction;
public:
	DirectionalLight(glm::vec3 direction, glm::vec3 lightColor);
	virtual glm::vec3 GetDirectionToLight(glm::vec3 position);
	virtual float GetDistanceToLight(glm::vec3 position);
};
