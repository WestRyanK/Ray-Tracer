#include "MeshObject.h"
#include "TriangleObject.h"
#include "ObjectList.h"
#include "../../Scene/Scene.h"
#include "../../Ray.h"
#include "../../Material/Material.h"


MeshObject::MeshObject() : GenericObject(nullptr)
{
	this->triangles = new ObjectList();
}

void MeshObject::AddTriangle(TriangleObject* triangle)
{
	this->triangles->AddObject(triangle);
}

GenericObject* MeshObject::GetRayIntersection(Ray ray, float& outIntersectionTime, glm::vec3& outPosition, glm::vec3& outNormal, glm::vec2& outTextureCoordinates)
{
	if (!this->HasRayBoundingBoxIntersection(ray))
		return nullptr;

	return this->triangles->GetObjectRayIntersection(ray, outIntersectionTime, outPosition, outNormal, outTextureCoordinates);
}

void MeshObject::UpdateBoundingBox()
{
	this->BoundingBoxMin = this->triangles->GetBoundingBoxMin();
	this->BoundingBoxMax = this->triangles->GetBoundingBoxMax();
}

void MeshObject::Finalize()
{
	this->triangles->Finalize();
	this->UpdateBoundingBox();
}
