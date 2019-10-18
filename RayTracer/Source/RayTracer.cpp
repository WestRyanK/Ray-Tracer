#include "RayTracer.h"

#include <vector>
#include "Camera/Camera.h"
#include "Image.h"
#include "Ray.h"
#include "Scene/Scene.h"
#include "Scene/Object/GenericObject.h"
#include <iostream>
#include "IO/PpmImageFileWriter.h"

RayTracer::RayTracer() : maxRayTreeDepth(1)
{

}

RayTracer* RayTracer::instance = nullptr;

RayTracer* RayTracer::GetInstance()
{
	if (RayTracer::instance == nullptr)
	{
		RayTracer::instance = new RayTracer();
	}

	return RayTracer::instance;
}

Image* RayTracer::Render(Scene* scene)
{
	scene->Finalize();
	Camera* camera = scene->GetCamera();
	Image* image = new Image(camera->GetHorizontalResolution(), camera->GetVerticalResolution());

	for (size_t y = 308; y < camera->GetVerticalResolution(); y++)
	//for (size_t y = camera->GetVerticalResolution() - 1; y >=0; y--
	{
		for (size_t x = 0; x < camera->GetHorizontalResolution(); x++)
		{
			std::vector<Ray> rays = camera->GetRaysForPixel((unsigned int)x, (unsigned int)y);

			glm::vec3 averageColor = this->GetAverageSceneColorFromRays(scene, rays, 0);

			image->SetPixel((unsigned int)x, (unsigned int)y, averageColor);
		}
		std::cout << "Row " << y << " of " << camera->GetVerticalResolution() << " " << (100.0f * y / camera->GetVerticalResolution()) << "%" << std::endl;
		PpmImageFileWriter writer;
		writer.Save("output.ppm", image);


	}

	return image;
}

void RayTracer::SetMaxRayTreeDepth(unsigned int maxRayTreeDepth)
{
	this->maxRayTreeDepth = maxRayTreeDepth;
}

glm::vec3 RayTracer::GetAverageSceneColorFromRays(Scene* scene, std::vector<Ray> rays, unsigned int rayTreeDepth)
{
	glm::vec3 averageColor(0, 0, 0);

	for (Ray ray : rays)
	{
		averageColor += this->GetSceneColorFromRay(scene, ray, rayTreeDepth);
	}

	averageColor /= rays.size();

	return averageColor;
}

glm::vec3 RayTracer::GetSceneColorFromRay(Scene* scene, Ray ray, unsigned int rayTreeDepth)
{
	if (rayTreeDepth > this->maxRayTreeDepth)
		return scene->GetBackgroundColorFromRay(ray);

	float hitTime;
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoordinates;
	GenericObject* hitObject = scene->GetObjectRayIntersection(ray, hitTime, position, normal, textureCoordinates);

	if (hitObject == nullptr)
	{
		return scene->GetBackgroundColorFromRay(ray);
	}
	else
	{
		return hitObject->GetColorFromRay(scene, ray, rayTreeDepth + 1, position, normal, textureCoordinates);
	}
}

void RayTracer::SetMaxShadowRayCnt(unsigned int maxShadowRayCnt)
{
	this->maxShadowRayCnt = maxShadowRayCnt;
}

unsigned int RayTracer::GetMaxShadowRayCnt()
{
	return this->maxShadowRayCnt;
}
