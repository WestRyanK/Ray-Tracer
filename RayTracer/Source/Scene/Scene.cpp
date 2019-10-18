#include "Scene.h"
#include "../Ray.h"
#include "../Camera/Camera.h"
#include "Object/GenericObject.h"
#include "Object/ObjectCollection.h"
#include "Light/GenericLight.h"
#include "GenericBackground.h"


Scene::Scene(ObjectCollection* collection) :
	objects(collection)
{

}

void Scene::SetCamera(Camera* camera)
{
	this->camera = camera;
}

Camera* Scene::GetCamera()
{
	return this->camera;
}

GenericObject* Scene::GetObjectRayIntersection(Ray ray, float& outNearestIntersectionTime, glm::vec3& outPosition, glm::vec3& outNormal, glm::vec2& outTextureCoordinates, bool getNearestIntersection)
{
	return this->objects->GetObjectRayIntersection(ray, outNearestIntersectionTime, outPosition, outNormal, outTextureCoordinates, getNearestIntersection);
}

void Scene::SetBackground(GenericBackground* background)
{
	this->background = background;
}

glm::vec3 Scene::GetBackgroundColorFromRay(Ray ray)
{
	return this->background->GetColorFromRay(ray);
}

void Scene::AddObject(GenericObject* object)
{
	this->objects->AddObject(object);
}

void Scene::AddLight(GenericLight* light)
{
	this->sceneLights.push_back(light);
}

void Scene::SetAmbientLight(glm::vec3 ambientLight)
{
	this->ambientLight = ambientLight;
}

glm::vec3 Scene::GetAmbientLight()
{
	return this->ambientLight;
}

void Scene::GetVisibleLightsFromPoint(glm::vec3 position, std::vector<GenericLight*>& outVisibleLights, std::vector<float>& outPercentVisible)
{
	for (GenericLight* light : this->sceneLights)
	{
		unsigned int shadowRaysNeededCnt = light->GetShadowRaysNeededCnt();
		unsigned int shadowRaysHitLightCnt = 0;
		for (size_t i = 0; i < shadowRaysNeededCnt; i++)
		{
			glm::vec3 directionToLight = light->GetDirectionToLight(position);
			float distanceToLight = light->GetDistanceToLight(position);

			if (distanceToLight > 0.0f)
			{
				Ray rayToLight(position, directionToLight);
				float nearestIntersectionTime = 0;
				glm::vec3 position;
				glm::vec3 normal;
				glm::vec2 textureCoordinates;
				GenericObject* nearestObject = this->objects->GetObjectRayIntersection(rayToLight, nearestIntersectionTime, position, normal, textureCoordinates, true);
				if (nearestIntersectionTime < Ray::START_EPSILON || nearestIntersectionTime > distanceToLight)
				{
					shadowRaysHitLightCnt++;
				}
			}
		}

		if (shadowRaysHitLightCnt > 0)
		{
			outVisibleLights.push_back(light);
			outPercentVisible.push_back((float)shadowRaysHitLightCnt / shadowRaysNeededCnt);
		}
	}
}

void Scene::Finalize()
{
	this->objects->Finalize();
}
