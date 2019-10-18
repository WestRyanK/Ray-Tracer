#pragma once

#include <glm/glm.hpp>
#include "GenericMaterialComponent.h"

class Scene;
class Ray;

class RefractiveMaterialComponent : public GenericMaterialComponent
{
private:
	float indexOfRefraction;
	float jitterAngle;
	unsigned int raysPerRefraction;
public:
	RefractiveMaterialComponent(float indexOfRefraction, float jitterAngle, unsigned int raysPerRefraction);
	virtual glm::vec3 GetComponentContribution(Scene* scene, Ray incomingRay, unsigned int rayTreeDepth, glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates);

	static const float INDEX_OF_REFRACTION_VACUUM;
	static const float INDEX_OF_REFRACTION_WATER;
	static const float INDEX_OF_REFRACTION_ALCOHOL;
	static const float INDEX_OF_REFRACTION_FUSED_QUARTZ;
	static const float INDEX_OF_REFRACTION_CROWN_GLASS;
	static const float INDEX_OF_REFRACTION_FLINT_GLASS;
	static const float INDEX_OF_REFRACTION_SAPPHIRE;
	static const float INDEX_OF_REFRACTION_HEAVY_FLINT_GLASS;
	static const float INDEX_OF_REFRACTION_DIAMOND;
};
