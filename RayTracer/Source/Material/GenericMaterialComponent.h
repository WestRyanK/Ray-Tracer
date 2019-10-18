#pragma once

#include <glm/glm.hpp>

class Scene;
class Ray;

class GenericMaterialComponent
{
public:
	GenericMaterialComponent() { };
	virtual glm::vec3 GetComponentContribution(Scene* scene, Ray incomingRay, unsigned int rayTreeDepth, glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates) = 0;
};
