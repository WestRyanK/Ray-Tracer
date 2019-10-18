#pragma once

#include <glm/glm.hpp>
#include "IBoundingBox.h"

class Ray;
class Material;
class Scene;

class GenericObject : public IBoundingBox
{
protected:
	Material* material;
public:
	GenericObject(Material* material);
	glm::vec3 GetColorFromRay(Scene* scene, Ray ray, unsigned int rayTreeDepth, glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates);

	virtual GenericObject* GetRayIntersection(Ray ray, float& outIntersectionTime, glm::vec3& outPosition, glm::vec3& outNormal, glm::vec2& outTextureCoordinates) = 0;
};
