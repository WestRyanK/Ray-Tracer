#pragma once

#include <glm/glm.hpp>
#include <vector>

class Ray;

class Jitterer
{
private:
	static glm::mat3x3 GetRotateAroundAxis(glm::vec3 u, float theta);
public:
	static glm::vec3 JitterVector(glm::vec3 inputVector, float solidAngle);
	static std::vector<Ray> JitterRay(glm::vec3 position, glm::vec3 inputVector, float solidAngle, unsigned int jitteredRayCount);
};
