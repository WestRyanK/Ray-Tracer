#pragma once
#pragma once

#include <glm/glm.hpp>
#include "GenericObject.h"
#include <vector>

class Ray;
class Material;
class Scene;
class TriangleObject;
class ObjectList;

class MeshObject : public GenericObject
{
protected:
	ObjectList* triangles;
	virtual void UpdateBoundingBox();

public:
	/* Triangles are clockwise winding order */
	MeshObject();

	void AddTriangle(TriangleObject* triangle);
	virtual GenericObject* GetRayIntersection(Ray ray, float& outIntersectionTime, glm::vec3& outPosition, glm::vec3& outNormal, glm::vec2& outTextureCoordinates);
	void Finalize();
};
