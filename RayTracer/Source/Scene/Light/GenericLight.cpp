#include "GenericLight.h"

GenericLight::GenericLight(glm::vec3 lightColor, float decayRatio) : 
	lightColor(lightColor),
	decayRatio(decayRatio)
{ 

}

glm::vec3 GenericLight::GetLightColor(glm::vec3 position)
{
	float decay = 1.0f;
	if (this->decayRatio != 0)
	{
		float distance = this->GetDistanceToLight(position);
		decay = 1.0f / (distance * decayRatio);
	}
	return this->lightColor * decay;
}

unsigned int GenericLight::GetShadowRaysNeededCnt()
{
	return 1;
}
