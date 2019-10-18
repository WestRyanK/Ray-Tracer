#pragma once

#include <glm/glm.hpp>
#include "PointLight.h"

class SphereAreaLight : public PointLight
{
public:
	SphereAreaLight(glm::vec3 position, glm::vec3 lightColor, float radius, float decayRatio = 0.0f);
	virtual glm::vec3 GetDirectionToLight(glm::vec3 position);
	virtual unsigned int GetShadowRaysNeededCnt();
};
