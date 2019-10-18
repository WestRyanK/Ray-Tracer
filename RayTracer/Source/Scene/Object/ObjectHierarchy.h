#pragma once

#include <glm/glm.hpp>
#include "ObjectList.h"
#include <vector>

class Camera;
class GenericObject;
class GenericLight;
class GenericBackground;
class Ray;

class ObjectHierarchy : public ObjectList
{
private:
	ObjectHierarchy** children;
	unsigned int maxObjectsPerNode;
	unsigned int maxHierarchyDepth;
	unsigned int hierarchyDepth;
	void SubdivideHierarchy(glm::vec3 boundingBoxMin, glm::vec3 boundingBoxMax);
	const int CHILD_NODE_COUNT = 2;
	bool hasChildren;

	void ConstructChildren();
	void MoveObjectsToChildrenBasedOnSplit(int splitDimension, float split);
	void UpdateChildrensBoundingBoxes(int splitDimension, float split);
protected:
	//virtual void UpdateBoundingBox();
	bool HasChildren();
	ObjectHierarchy(unsigned int maxObjectsPerNode, unsigned int maxHierarchyDepth, unsigned int hierarchyDepth);
public:
	ObjectHierarchy(unsigned int maxObjectsPerNode, unsigned int maxHierarchyDepth);
	GenericObject* GetObjectRayIntersection(Ray ray, float& outIntersectionTime, glm::vec3& outPosition, glm::vec3& outNormal, glm::vec2& outTextureCoordinates, bool getNearestIntersection = true);
	void Finalize();
};


