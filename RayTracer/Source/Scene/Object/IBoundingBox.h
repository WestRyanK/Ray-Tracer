#pragma once

#include <glm/glm.hpp>

class Ray;

class IBoundingBox
{
protected:
	glm::vec3 BoundingBoxMin;
	glm::vec3 BoundingBoxMax;
	virtual void UpdateBoundingBox() = 0;
public:
	glm::vec3 GetBoundingBoxMin();
	glm::vec3 GetBoundingBoxMax();
	glm::vec3 GetBoundingBoxSize();
	glm::vec3 GetBoundingBoxCenter();
	int GetLargestDimensionOfBoundingBox();
	bool HasRayBoundingBoxIntersection(Ray ray);
};
