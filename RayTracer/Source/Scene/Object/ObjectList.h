#pragma once

#include <glm/glm.hpp>
#include "ObjectCollection.h"
#include <vector>

class Camera;
class GenericObject;
class GenericLight;
class GenericBackground;
class Ray;

class ObjectList : public ObjectCollection
{
protected:
	std::vector<GenericObject*> objects;
	virtual void UpdateBoundingBox();
public:
	ObjectList();
	void AddObject(GenericObject* object);
	virtual GenericObject* GetObjectRayIntersection(Ray ray, float& outIntersectionTime, glm::vec3& outPosition, glm::vec3& outNormal, glm::vec2& outTextureCoordinates, bool getNearestIntersection = true);
	virtual void Finalize();
};


