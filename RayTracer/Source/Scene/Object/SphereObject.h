#pragma once

#include "GenericObject.h"
class Ray;
class Material;

class SphereObject : public GenericObject
{
private:
	glm::vec3 position;
	float radius;
	float radius_squared;
protected:
	virtual void UpdateBoundingBox();
public:
	SphereObject(Material* material, glm::vec3 position, float radius);
	virtual GenericObject* GetRayIntersection(Ray ray, float& outIntersectionTime, glm::vec3& outPosition, glm::vec3& outNormal, glm::vec2& outTextureCoordinates);
};
