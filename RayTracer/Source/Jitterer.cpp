#include "Jitterer.h"
#include <glm/gtc/type_ptr.hpp>
#include "RandomGenerator.h"
#include "Ray.h"

std::vector<Ray> Jitterer::JitterRay(glm::vec3 position, glm::vec3 inputVector, float solidAngle, unsigned int jitteredRayCount)
{
	std::vector<Ray> jitteredRays;
	for (size_t i = 0; i < jitteredRayCount; i++)
	{
		glm::vec3 jitteredDirection = Jitterer::JitterVector(inputVector, solidAngle);
		Ray jitteredRay(position, jitteredDirection);
		jitteredRays.push_back(jitteredRay);
	}

	return jitteredRays;
}

glm::vec3 Jitterer::JitterVector(glm::vec3 inputVector, float solidAngle)
{
	// We want any two orthonormal vectors, u and v, that are normal to the input vector.
	// We simply cross the input with a vector that is guaranteed not to be the input or its inverse to get u.
	glm::vec3 otherVector = glm::vec3(-inputVector.y, inputVector.z, inputVector.x);
	glm::vec3 u = glm::normalize(glm::cross(inputVector, otherVector));
	//glm::vec3 v = glm::normalize(glm::cross(inputVector, u));

	const float PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998f;
	float rotate_u_theta = RandomGenerator::RandomBetween(0.0f, solidAngle);
	float rotate_circle_theta = RandomGenerator::RandomBetween(0.0f, 2 * PI);
	glm::mat3x3 rotate_u = Jitterer::GetRotateAroundAxis(u, rotate_u_theta);
	glm::mat3x3 rotate_circle = Jitterer::GetRotateAroundAxis(glm::normalize(inputVector), rotate_circle_theta);

	glm::vec3 jitteredVector = rotate_circle * rotate_u * inputVector;

	return jitteredVector;

}

glm::mat3x3 Jitterer::GetRotateAroundAxis(glm::vec3 u, float theta)
{
	float cos0 = glm::cos(theta);
	float sin0 = glm::sin(theta);
	float m11 = cos0 + u.x * u.x * (1 - cos0);
	float m21 = u.y * u.x * (1 - cos0) + u.z * sin0;
	float m31 = u.z * u.x * (1 - cos0) - u.y * sin0;
	float m12 = u.x * u.y * (1 - cos0) - u.z * sin0;
	float m22 = cos0 + u.y * u.y * (1 - cos0);
	float m32 = u.z * u.y * (1 - cos0) + u.x * sin0;
	float m13 = u.x * u.z * (1 - cos0) + u.y * sin0;
	float m23 = u.y * u.z * (1 - cos0) - u.x * sin0;
	float m33 = cos0 + u.z * u.z * (1 - cos0);
	float mat[]= { m11, m21, m31, m12, m22, m32, m13, m23, m33 };
	glm::mat3x3 rotation = glm::make_mat3x3(mat);
	return rotation;
}
