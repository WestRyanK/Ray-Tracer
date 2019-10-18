#include "IBoundingBox.h"
#include "../../Ray.h"
#include <algorithm>

bool IBoundingBox::HasRayBoundingBoxIntersection(Ray ray)
{
	float tMin = -INFINITY;
	float tMax = INFINITY;

	for (size_t i = 0; i < glm::vec3::length(); i++)
	{
		float rayOrigin = ray.GetRayStart()[i];
		float rayDirection = ray.GetRayDirection()[i];
		float boundsMin = this->BoundingBoxMin[i];
		float boundsMax = this->BoundingBoxMax[i];
		if (rayDirection == 0)
		{	// Ray is parallel with planes
			if (rayOrigin < boundsMin || rayOrigin > boundsMax)
			{
				return false;
			}
		}
		else
		{
			float t1 = (boundsMin - rayOrigin) / rayDirection;
			float t2 = (boundsMax - rayOrigin) / rayDirection;

			if (t1 > t2)
			{
				std::swap(t1, t2);
			}
			if (t1 > tMin)
			{
				tMin = t1;
			}
			if (t2 < tMax)
			{
				tMax = t2;
			}
			if (tMin > tMax)
			{
				return false;
			}
			if (tMax < 0)
			{
				return false;
			}
		}
	}
	return true;
}

glm::vec3 IBoundingBox::GetBoundingBoxMin()
{
	return this->BoundingBoxMin;
}

glm::vec3 IBoundingBox::GetBoundingBoxMax()
{
	return this->BoundingBoxMax;
}

glm::vec3 IBoundingBox::GetBoundingBoxSize()
{
	glm::vec3 boundingBoxSize = this->BoundingBoxMax - this->BoundingBoxMin;
	return boundingBoxSize;
}

int IBoundingBox::GetLargestDimensionOfBoundingBox()
{
	glm::vec3 boundingBoxSize = this->GetBoundingBoxSize();

	int largestDimensionIndex = 0;
	for (size_t i = 1; i < boundingBoxSize.length(); i++)
	{
		if (boundingBoxSize[largestDimensionIndex] < boundingBoxSize[i])
		{
			largestDimensionIndex = i;
		}
	}

	return largestDimensionIndex;
}

glm::vec3 IBoundingBox::GetBoundingBoxCenter()
{
	glm::vec3 center = (this->BoundingBoxMax + this->BoundingBoxMin) * 0.5f;
	return center;
}
