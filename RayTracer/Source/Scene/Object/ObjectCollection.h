#pragma once

#include <glm/glm.hpp>
#include "IBoundingBox.h"
#include <vector>

class Camera;
class GenericObject;
class GenericLight;
class GenericBackground;
class Ray;

class ObjectCollection : public IBoundingBox
{
public:
	virtual void AddObject(GenericObject* object) = 0;
	virtual GenericObject* GetObjectRayIntersection(Ray ray, float& outIntersectionTime, glm::vec3& outPosition, glm::vec3& outNormal, glm::vec2& outTextureCoordinates, bool getNearestIntersection = true) = 0;
	virtual void Finalize() = 0;
};
