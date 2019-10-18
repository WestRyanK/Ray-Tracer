#pragma once

#include <glm/glm.hpp>
#include "GenericLight.h"

class PointLight : public GenericLight
{
protected:
	glm::vec3 position;
	float radius;
public:
	PointLight(glm::vec3 position, glm::vec3 lightColor, float radius = 0.0f, float decayRatio = 0.0f);
	virtual glm::vec3 GetDirectionToLight(glm::vec3 position);
	virtual float GetDistanceToLight(glm::vec3 position);
};
