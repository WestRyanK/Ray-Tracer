#include "SpecularMaterialComponent.h"
#include <vector>
#include "GenericMaterialComponent.h"
#include "ITexture.h"
#include "../Ray.h"
#include "../Scene/Scene.h"
#include "../Scene/Light/GenericLight.h"

SpecularMaterialComponent::SpecularMaterialComponent(ITexture* specularTexture, float phongExponent) :
	GenericMaterialComponent(),
	specularTexture(specularTexture),
	phongExponent(phongExponent)
{

}

glm::vec3 SpecularMaterialComponent::GetComponentContribution(Scene* scene, Ray incomingRay, unsigned int rayTreeDepth, glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates)
{
	glm::vec3 specularColor = glm::vec3(0, 0, 0);

	std::vector<GenericLight*> visibleLights;
	std::vector<float> percentLightVisible;
	scene->GetVisibleLightsFromPoint(position, visibleLights, percentLightVisible);

	for (size_t i = 0; i < visibleLights.size(); i++)
	{
		glm::vec3 directionToLight = visibleLights[i]->GetDirectionToLight(position);
		glm::vec3 reflectedRay = 2.0f * normal * glm::dot(normal, directionToLight) - directionToLight;
		float incomingRay_dot_reflectedRay = glm::dot(-incomingRay.GetRayDirection(), reflectedRay);
		if (incomingRay_dot_reflectedRay > 0.0f)
		{ // Only add a specular highlight if the incomingRay and reflectedRay face the same direction
			specularColor += visibleLights[i]->GetLightColor(position) * this->specularTexture->GetColor(textureCoordinates) * glm::max(0.0f, glm::pow(incomingRay_dot_reflectedRay, this->phongExponent));
		}
	}

	return specularColor;
}
