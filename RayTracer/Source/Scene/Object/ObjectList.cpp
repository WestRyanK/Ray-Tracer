#include "ObjectList.h"
#include "IBoundingBox.h"
#include "../../Ray.h"
#include "GenericObject.h"

ObjectList::ObjectList()
{

}

void ObjectList::AddObject(GenericObject* object)
{
	this->objects.push_back(object);
}

GenericObject* ObjectList::GetObjectRayIntersection(Ray ray, float& outIntersectionTime, glm::vec3& outPosition, glm::vec3& outNormal, glm::vec2& outTextureCoordinates, bool getNearestIntersection)
{
	if (!this->HasRayBoundingBoxIntersection(ray))
		return nullptr;

	GenericObject* nearestObject = nullptr;

	for (GenericObject* object : this->objects)
	{
		float intersectionTime;
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoordinates;
		GenericObject* intersectedObject = object->GetRayIntersection(ray, intersectionTime, position, normal, textureCoordinates);
		
		if (intersectedObject != nullptr && intersectionTime > Ray::START_EPSILON && (intersectionTime < outIntersectionTime || nearestObject == nullptr))
		{
			outIntersectionTime = intersectionTime;
			outPosition = position;
			outNormal = normal;
			outTextureCoordinates = textureCoordinates;
			nearestObject = intersectedObject;

			if (!getNearestIntersection)
				return nearestObject;
		}
	}

	return nearestObject;
}

void ObjectList::UpdateBoundingBox()
{
	this->BoundingBoxMax = this->objects[0]->GetBoundingBoxMax();
	this->BoundingBoxMin = this->objects[0]->GetBoundingBoxMin();
	for (GenericObject* object : this->objects)
	{
		glm::vec3 objectMax = object->GetBoundingBoxMax();
		this->BoundingBoxMax = glm::max(this->BoundingBoxMax, objectMax);

		glm::vec3 objectMin = object->GetBoundingBoxMin();
		this->BoundingBoxMin = glm::min(this->BoundingBoxMin, objectMin);
	}
}

void ObjectList::Finalize()
{
	this->UpdateBoundingBox();
}
