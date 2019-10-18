#include "Material.h"
#include "GenericMaterialComponent.h"
#include "../Scene/Scene.h"
#include "../Ray.h"

Material::Material()
{

}

glm::vec3 Material::GetColor(Scene* scene, Ray incomingRay, unsigned int rayTreeDepth, glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates)
{
	glm::vec3 color = glm::vec3(0, 0, 0);

	for (size_t i = 0; i < this->materialComponentContributions.size(); i++)
	{
		color += this->materialComponentContributions[i] *  this->materialComponents[i]->GetComponentContribution(scene, incomingRay, rayTreeDepth, position, normal, textureCoordinates);
	}

	return color;
}

void Material::AddMaterialComponent(float percentContribution, GenericMaterialComponent* materialComponent)
{
	this->materialComponents.push_back(materialComponent);
	this->materialComponentContributions.push_back(percentContribution);
}
