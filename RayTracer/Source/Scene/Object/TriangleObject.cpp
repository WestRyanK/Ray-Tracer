#include "TriangleObject.h"
#include "../../Scene/Scene.h"
#include "../../Ray.h"
#include "../../Material/Material.h"

TriangleObject::TriangleObject(Material* material, glm::vec3 vertexA, glm::vec3 vertexB, glm::vec3 vertexC, bool isTwoSided, glm::vec2 textureCoordinateA, glm::vec2 textureCoordinateB, glm::vec2 textureCoordinateC) :
	GenericObject(material),
	vertexA(vertexA),
	vertexB(vertexB),
	vertexC(vertexC),
	isTwoSided(isTwoSided),
	textureCoordinateA(textureCoordinateA),
	textureCoordinateB(textureCoordinateB),
	textureCoordinateC(textureCoordinateC)
{
	this->ab = this->vertexB - this->vertexA;
	this->ac = this->vertexC - this->vertexA;
	this->normal = glm::normalize(glm::cross(ab, ac));
	this->UpdateBoundingBox();
}
GenericObject* TriangleObject::GetRayIntersection(Ray ray, float& outIntersectionTime, glm::vec3& outPosition, glm::vec3& outNormal, glm::vec2& outTextureCoordinates)
{
	if (!this->HasRayBoundingBoxIntersection(ray))
		return nullptr;

	glm::vec3 P = glm::cross(ray.GetRayDirection(), this->ac);
	float det = glm::dot(this->ab, P);

	if (glm::abs(det) < Ray::START_EPSILON)
	{ // Ray is roughly parallel to Triangle, so no intersection.
		return nullptr;
	}

	//if (!this->isTwoSided && det < 0)
	//{ // Triangle is one-sided and Ray is hitting back side.
	//	return nullptr;
	//}

	float inverseDet = 1.0f / det;
	
	glm::vec3 T = ray.GetRayStart() - this->vertexA;
	float u = glm::dot(T, P) * inverseDet;
	if (u <= 0.0f || u >= 1.0f)
	{ // Ray is outside Triangle on u-axis.
		return nullptr;
	}

	glm::vec3 Q = glm::cross(T, this->ab);
	float v = glm::dot(ray.GetRayDirection(), Q) * inverseDet;
	if (v <= 0.0f || u + v >= 1.0f)
	{ // Ray is outside Triangle on v-axis.
		return nullptr;
	}

	outIntersectionTime = inverseDet * glm::dot(this->ac, Q);
	outPosition = ray.GetPointOnRayAtTime(outIntersectionTime);
	outNormal = this->normal;
	if (this->isTwoSided && det < 0)
	{
		outNormal = -outNormal;
	}

	outTextureCoordinates = this->textureCoordinateA + u * (this->textureCoordinateC - this->textureCoordinateA) + v * (this->textureCoordinateB - this->textureCoordinateA);

	return this;
}
void TriangleObject::UpdateBoundingBox()
{
	this->BoundingBoxMin = glm::min(glm::min(this->vertexA, this->vertexB), this->vertexC);
	this->BoundingBoxMax = glm::max(glm::max(this->vertexA, this->vertexB), this->vertexC);
}
