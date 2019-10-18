#include "SphereAreaLight.h"
#include "../../RayTracer.h"
#include "../../Jitterer.h"

SphereAreaLight::SphereAreaLight(glm::vec3 position, glm::vec3 lightColor, float radius, float decayRatio ) :
	PointLight(position, lightColor, radius, decayRatio) 
{

}

glm::vec3 SphereAreaLight::GetDirectionToLight(glm::vec3 position)
{
	float distanceToLight = this->GetDistanceToLight(position);
	float jitterAngle = this->radius / distanceToLight;
	glm::vec3 directionToLight = glm::normalize(this->position - position);
	glm::vec3 jitteredDirectionToLight = Jitterer::JitterVector(directionToLight, jitterAngle);
	return jitteredDirectionToLight;
}

unsigned int SphereAreaLight::GetShadowRaysNeededCnt()
{
	return RayTracer::GetInstance()->GetMaxShadowRayCnt();
}
