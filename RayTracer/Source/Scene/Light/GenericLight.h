#pragma once

#include <glm/glm.hpp>

class GenericLight
{
private:
	glm::vec3 lightColor;
	float decayRatio;
public:
	GenericLight(glm::vec3 lightColor, float decayRatio = 0.0f);
	virtual glm::vec3 GetDirectionToLight(glm::vec3 position) = 0;
	virtual float GetDistanceToLight(glm::vec3 position) = 0;
	virtual unsigned int GetShadowRaysNeededCnt();
	glm::vec3 GetLightColor(glm::vec3 position);
};
