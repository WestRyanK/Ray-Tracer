#pragma once

#include <glm/glm.hpp>
#include "GenericMaterialComponent.h"

class Scene;
class Ray;
class ITexture;

class DiffuseMaterialComponent : public GenericMaterialComponent
{
private:
	ITexture* diffuseTexture;
public:
	DiffuseMaterialComponent(ITexture* diffuseTexture);
	virtual glm::vec3 GetComponentContribution(Scene* scene, Ray incomingRay, unsigned int rayTreeDepth, glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates);
};
