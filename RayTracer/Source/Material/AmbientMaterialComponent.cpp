#include "AmbientMaterialComponent.h"
#include "GenericMaterialComponent.h"
#include "ITexture.h"
#include "../Scene/Scene.h"
#include "../Ray.h"

AmbientMaterialComponent::AmbientMaterialComponent(ITexture* ambientTexture) : 
	GenericMaterialComponent(), 
	ambientTexture(ambientTexture)
{

}

glm::vec3 AmbientMaterialComponent::GetComponentContribution(Scene* scene, Ray incomingRay, unsigned int rayTreeDepth, glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates)
{
	return this->ambientTexture->GetColor(textureCoordinates) * scene->GetAmbientLight();
}
