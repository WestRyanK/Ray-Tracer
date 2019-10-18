#pragma once

#include <glm/glm.hpp>
#include "GenericMaterialComponent.h"

class Scene;
class Ray;
class ITexture;

class AmbientMaterialComponent : public GenericMaterialComponent
{
private:
	ITexture* ambientTexture;
public:
	AmbientMaterialComponent(ITexture* ambientTexture);
	virtual glm::vec3 GetComponentContribution(Scene* scene, Ray incomingRay, unsigned int rayTreeDepth, glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates);
};
