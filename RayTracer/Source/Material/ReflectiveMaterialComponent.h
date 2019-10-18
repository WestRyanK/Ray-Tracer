#pragma once

#include <glm/glm.hpp>
#include "GenericMaterialComponent.h"

class Scene;
class Ray;

class ReflectiveMaterialComponent : public GenericMaterialComponent
{
private:
	float jitterAngle;
	unsigned int raysPerReflection;
public:
	ReflectiveMaterialComponent(float jitterAngle, unsigned int raysPerReflection);
	virtual glm::vec3 GetComponentContribution(Scene* scene, Ray incomingRay, unsigned int rayTreeDepth, glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates);
};
