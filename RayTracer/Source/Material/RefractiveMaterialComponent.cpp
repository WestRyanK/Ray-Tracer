#include "RefractiveMaterialComponent.h"
#include "GenericMaterialComponent.h"
#include "../Ray.h"
#include "../Scene/Scene.h"
#include "../RayTracer.h"
#include "../Jitterer.h"

const float RefractiveMaterialComponent::INDEX_OF_REFRACTION_VACUUM = 1.0f;
const float RefractiveMaterialComponent::INDEX_OF_REFRACTION_WATER = 1.33f;
const float RefractiveMaterialComponent::INDEX_OF_REFRACTION_ALCOHOL = 1.36f;
const float RefractiveMaterialComponent::INDEX_OF_REFRACTION_FUSED_QUARTZ = 1.46f;
const float RefractiveMaterialComponent::INDEX_OF_REFRACTION_CROWN_GLASS = 1.52f;
const float RefractiveMaterialComponent::INDEX_OF_REFRACTION_FLINT_GLASS = 1.65f;
const float RefractiveMaterialComponent::INDEX_OF_REFRACTION_SAPPHIRE = 1.77f;
const float RefractiveMaterialComponent::INDEX_OF_REFRACTION_HEAVY_FLINT_GLASS = 1.89f;
const float RefractiveMaterialComponent::INDEX_OF_REFRACTION_DIAMOND = 2.42f;

RefractiveMaterialComponent::RefractiveMaterialComponent(float indexOfRefraction, float jitterAngle, unsigned int raysPerRefraction) : 
	GenericMaterialComponent(),
	indexOfRefraction(indexOfRefraction),
	jitterAngle(jitterAngle),
	raysPerRefraction(raysPerRefraction)
{

}

glm::vec3 RefractiveMaterialComponent::GetComponentContribution(Scene* scene, Ray incomingRay, unsigned int rayTreeDepth, glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates)
{
	const float n1 = RefractiveMaterialComponent::INDEX_OF_REFRACTION_VACUUM;
	float n_it = n1 / this->indexOfRefraction; // Right now I assume any material is in a vacuum...

	if (glm::dot(incomingRay.GetRayDirection(), normal) > 0.0f)
	{ 
		n_it = 1.0f / n_it; // If the ray is going outside of an object, flip the ratio.
		normal = -normal; // The normal that we get always faces outside the object, but we're on the inside, so that needs to flip.
	}

	glm::vec3 I = incomingRay.GetRayDirection();
	float cos_theta = glm::dot(-I, normal);
	float theta = glm::acos(cos_theta);

	float n_it_squared = n_it * n_it;
	float cos_theta_squared = cos_theta * cos_theta;
		
	glm::vec3 transmittedDirection = n_it * I + (n_it * cos_theta - glm::sqrt(1 - n_it_squared * (1 - cos_theta_squared))) * normal;

	std::vector<Ray> transmittedRays = Jitterer::JitterRay(position, transmittedDirection, this->jitterAngle, this->raysPerRefraction);
	glm::vec3 transmittedColor = RayTracer::GetInstance()->GetAverageSceneColorFromRays(scene, transmittedRays, rayTreeDepth);
 
	return transmittedColor;
}
