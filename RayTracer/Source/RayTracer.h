#pragma once

#include <glm/glm.hpp>
#include <vector>
class Image;
class Scene;
class Ray;

class RayTracer
{
private:
	unsigned int maxRayTreeDepth = 10;
	unsigned int maxShadowRayCnt = 16;
	RayTracer();
	static RayTracer* instance;
public:
	static RayTracer* GetInstance();
	Image* Render(Scene* scene);
	glm::vec3 GetSceneColorFromRay(Scene* scene, Ray ray, unsigned int rayTreeDepth);
	glm::vec3 GetAverageSceneColorFromRays(Scene* scene, std::vector<Ray> rays, unsigned int rayTreeDepth);
	void SetMaxRayTreeDepth(unsigned int maxRayTreeDepth);
	void SetMaxShadowRayCnt(unsigned int maxShadowRayCnt);
	unsigned int GetMaxShadowRayCnt();
};
