#include "DiffuseMaterialComponent.h"
#include <vector>
#include "GenericMaterialComponent.h"
#include "ITexture.h"
#include "../Ray.h"
#include "../Scene/Scene.h"
#include "../Scene/Light/GenericLight.h"

DiffuseMaterialComponent::DiffuseMaterialComponent(ITexture* diffuseTexture) : GenericMaterialComponent(), diffuseTexture(diffuseTexture)
{

}

glm::vec3 DiffuseMaterialComponent::GetComponentContribution(Scene* scene, Ray incomingRay, unsigned int rayTreeDepth, glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates)
{
	glm::vec3 diffuseColor = glm::vec3(0, 0, 0);

	std::vector<GenericLight*> visibleLights;
	std::vector<float> percentLightVisible;
	scene->GetVisibleLightsFromPoint(position, visibleLights, percentLightVisible);

	for (size_t i = 0; i < visibleLights.size(); i++)
	{
		diffuseColor += percentLightVisible[i] * visibleLights[i]->GetLightColor(position) * this->diffuseTexture->GetColor(textureCoordinates) * glm::max(0.0f, glm::dot(normal, visibleLights[i]->GetDirectionToLight(position)));
	}

	return diffuseColor;
}
