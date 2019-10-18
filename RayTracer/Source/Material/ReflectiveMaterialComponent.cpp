#include "ReflectiveMaterialComponent.h"
#include <vector>
#include "GenericMaterialComponent.h"
#include "../Jitterer.h"
#include "../Ray.h"
#include "../Scene/Scene.h"
#include "../RayTracer.h"

ReflectiveMaterialComponent::ReflectiveMaterialComponent(float jitterAngle, unsigned int raysPerReflection) : 
	GenericMaterialComponent(),
	jitterAngle(jitterAngle),
	raysPerReflection(raysPerReflection)
{

}

glm::vec3 ReflectiveMaterialComponent::GetComponentContribution(Scene* scene, Ray incomingRay, unsigned int rayTreeDepth, glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates)
{
	glm::vec3 directionToIncomingRay = -incomingRay.GetRayDirection();
	glm::vec3 reflectedDirection = 2.0f * normal * glm::dot(normal, directionToIncomingRay) - directionToIncomingRay;

	std::vector<Ray> reflectedRays = Jitterer::JitterRay(position, reflectedDirection, this->jitterAngle, this->raysPerReflection);
	glm::vec3 reflectedColor = RayTracer::GetInstance()->GetAverageSceneColorFromRays(scene, reflectedRays, rayTreeDepth);
 
	return reflectedColor;
}
