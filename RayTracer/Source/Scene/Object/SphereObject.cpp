#include "SphereObject.h"
#include "../../Material/Material.h"
#include "../../Ray.h"

SphereObject::SphereObject(Material* material, glm::vec3 position, float radius) : 
	GenericObject(material), 
	position(position), 
	radius(radius), 
	radius_squared(radius * radius)
{
	this->UpdateBoundingBox();
}

// See the following link for an explanation of the geometric approach and explanations of the variable names.
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
GenericObject* SphereObject::GetRayIntersection(Ray ray, float& outIntersectionTime, glm::vec3& outPosition, glm::vec3& outNormal, glm::vec2& outTextureCoordinates)
{
	if (!this->HasRayBoundingBoxIntersection(ray))
		return nullptr;

	glm::vec3 L = this->position - ray.GetRayStart();
	float t_ca = glm::dot(L, ray.GetRayDirection());
	if (t_ca < 0)
	{ // Sphere is behind ray
		return nullptr; 
	}

	float L_squared = glm::dot(L, L);
	float d_squared = L_squared - t_ca * t_ca;
	if (d_squared > this->radius_squared)
	{ // The closest the ray gets to the sphere is still outside the radius
		return nullptr; 
	}

	float t_hc = glm::sqrt(this->radius_squared - d_squared);

	if (L_squared < this->radius_squared + Ray::START_EPSILON)
	{ // The ray starts inside the sphere
		outIntersectionTime = t_ca + t_hc;
	}
	else
	{
		outIntersectionTime = t_ca - t_hc;
	}

	outPosition = ray.GetPointOnRayAtTime(outIntersectionTime);
	outNormal = glm::normalize(outPosition - this->position);

	return this;
}

void SphereObject::UpdateBoundingBox()
{
	this->BoundingBoxMin = this->position - this->radius;
	this->BoundingBoxMax = this->position + this->radius;
}
