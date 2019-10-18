#pragma once

#include <glm/glm.hpp>
#include "GenericObject.h"

class Ray;
class Material;
class Scene;

class TriangleObject: public GenericObject
{
protected:
	bool isTwoSided;
	glm::vec3 vertexA;
	glm::vec3 vertexB;
	glm::vec3 vertexC;
	glm::vec2 textureCoordinateA;
	glm::vec2 textureCoordinateB;
	glm::vec2 textureCoordinateC;
	glm::vec3 normal;
	glm::vec3 ab;
	glm::vec3 ac;
	virtual void UpdateBoundingBox();

public:
	/* Triangles are clockwise winding order */
	TriangleObject(Material* material, glm::vec3 vertexA, glm::vec3 vertexB, glm::vec3 vertexC, bool isTwoSided, glm::vec2 textureCoordinateA = glm::vec2(0.0f, 1.0f), glm::vec2 textureCoordinateB = glm::vec2(0.0f, 0.0f), glm::vec2 textureCoordinateC = glm::vec2(1.0f, 1.0f));

	virtual GenericObject* GetRayIntersection(Ray ray, float& outIntersectionTime, glm::vec3& outPosition, glm::vec3& outNormal, glm::vec2& outTextureCoordinates);
};
