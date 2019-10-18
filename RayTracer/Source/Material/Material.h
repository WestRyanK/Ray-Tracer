#pragma once

#include <glm/glm.hpp>
#include <vector>

class GenericMaterialComponent;
class Ray;
class Scene;

class Material
{
private:
	std::vector<GenericMaterialComponent*> materialComponents;
	std::vector<float> materialComponentContributions;
public:
	Material();
	void AddMaterialComponent(float percentContribution, GenericMaterialComponent* materialComponent);
	virtual glm::vec3 GetColor(Scene* scene, Ray incomingRay, unsigned int rayTreeDepth, glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates);
};
