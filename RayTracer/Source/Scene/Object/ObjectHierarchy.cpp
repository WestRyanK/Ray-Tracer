#include "ObjectHierarchy.h"
#include "IBoundingBox.h"
#include "../../Ray.h"
#include "GenericObject.h"

ObjectHierarchy::ObjectHierarchy(unsigned int maxObjectsPerNode, unsigned int maxHierarchyDepth, unsigned int hierarchyDepth) :
	maxObjectsPerNode(maxObjectsPerNode),
	maxHierarchyDepth(maxHierarchyDepth),
	hierarchyDepth(hierarchyDepth),
	hasChildren(false)
{

}

ObjectHierarchy::ObjectHierarchy(unsigned int maxObjectsPerNode, unsigned int maxHierarchyDepth) : 
	ObjectHierarchy(maxObjectsPerNode, maxHierarchyDepth, 0)
{

}

GenericObject* ObjectHierarchy::GetObjectRayIntersection(Ray ray, float& outIntersectionTime, glm::vec3& outPosition, glm::vec3& outNormal, glm::vec2& outTextureCoordinates, bool getNearestIntersection)
{
	if (!this->HasRayBoundingBoxIntersection(ray))
		return nullptr;

	GenericObject* nearestObject = ObjectList::GetObjectRayIntersection(ray, outIntersectionTime, outPosition, outNormal, outTextureCoordinates, getNearestIntersection);
	if (this->HasChildren() && (nearestObject == nullptr || getNearestIntersection))
	{
		for (size_t i = 0; i < ObjectHierarchy::CHILD_NODE_COUNT; i++)
		{
			float outIntersectionTimeChild;
			glm::vec3 outPositionChild;
			glm::vec3 outNormalChild;
			glm::vec2 outTextureCoordinatesChild;
			GenericObject* nearestObjectChild = this->children[i]->GetObjectRayIntersection(ray, outIntersectionTimeChild, outPositionChild, outNormalChild, outTextureCoordinatesChild, getNearestIntersection);

			if (nearestObjectChild != nullptr && outIntersectionTimeChild > Ray::START_EPSILON && (outIntersectionTimeChild < outIntersectionTime || nearestObject == nullptr))
			{
				nearestObject = nearestObjectChild;
				outPosition = outPositionChild;
				outIntersectionTime = outIntersectionTimeChild;
				outNormal = outNormalChild;
				outTextureCoordinates = outTextureCoordinatesChild;
			}
		}
	}

	return nearestObject;
}

void ObjectHierarchy::Finalize()
{
	this->UpdateBoundingBox();
	this->SubdivideHierarchy(this->BoundingBoxMin, this->BoundingBoxMax);
}






void ObjectHierarchy::ConstructChildren()
{
	this->children = new ObjectHierarchy*[ObjectHierarchy::CHILD_NODE_COUNT];
	for (size_t i = 0; i < ObjectHierarchy::CHILD_NODE_COUNT; i++)
	{
		this->children[i] = new ObjectHierarchy(this->maxObjectsPerNode, this->maxHierarchyDepth, this->hierarchyDepth + 1);
	}

	this->hasChildren = true;
}

bool ObjectHierarchy::HasChildren()
{
	return this->hasChildren;
}

void ObjectHierarchy::MoveObjectsToChildrenBasedOnSplit(int splitDimension, float split)
{
	std::vector<GenericObject*> nonChildObjects;
	for (GenericObject* object : this->objects)
	{
		bool isObjectMinBelowSplit = object->GetBoundingBoxMin()[splitDimension] < split;
		bool isObjectMaxBelowSplit = object->GetBoundingBoxMax()[splitDimension] < split;
		if (isObjectMinBelowSplit && isObjectMaxBelowSplit)
		{
			this->children[0]->AddObject(object);
		}
		else if (!isObjectMinBelowSplit && !isObjectMaxBelowSplit)
		{
			this->children[1]->AddObject(object);
		}
		else
		{
			nonChildObjects.push_back(object);
		}
	}
	this->objects = nonChildObjects;
}

void ObjectHierarchy::UpdateChildrensBoundingBoxes(int splitDimension, float split)
{
	glm::vec3 splitMax = this->BoundingBoxMax;
	splitMax[splitDimension] = split;
	glm::vec3 splitMin = this->BoundingBoxMin;
	splitMin[splitDimension] = split;
	this->children[0]->SubdivideHierarchy(this->BoundingBoxMin, splitMax);
	this->children[1]->SubdivideHierarchy(splitMin, this->BoundingBoxMax);
}

void ObjectHierarchy::SubdivideHierarchy(glm::vec3 boundingBoxMin, glm::vec3 boundingBoxMax)
{
	this->BoundingBoxMin = boundingBoxMin;
	this->BoundingBoxMax = boundingBoxMax;

	if (this->hierarchyDepth < this->maxHierarchyDepth && this->objects.size() > this->maxObjectsPerNode)
	{
		int splitDimension = this->GetLargestDimensionOfBoundingBox();
		float split = this->GetBoundingBoxCenter()[splitDimension];

		this->ConstructChildren();
		this->MoveObjectsToChildrenBasedOnSplit(splitDimension, split);
		this->UpdateChildrensBoundingBoxes(splitDimension, split);
	}
}
