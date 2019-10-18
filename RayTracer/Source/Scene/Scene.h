#pragma once

#include <glm/glm.hpp>
#include <vector>

class Camera;
class GenericObject;
class GenericLight;
class GenericBackground;
class Ray;
class ObjectCollection;

class Scene
{
private:
	Camera* camera = nullptr;
	GenericBackground* background = nullptr;
	std::vector<GenericLight*> sceneLights;
	ObjectCollection* objects;
	glm::vec3 ambientLight;

public:
	Scene(ObjectCollection* collection);
	void SetCamera(Camera* camera);
	Camera* GetCamera();
	void SetBackground(GenericBackground* background);
	void SetAmbientLight(glm::vec3 ambientLight);
	glm::vec3 GetAmbientLight();
	GenericObject* GetObjectRayIntersection(Ray ray, float& outNearestIntersectionTime, glm::vec3& outPosition, glm::vec3& outNormal, glm::vec2& outTextureCoordinates, bool getNearestIntersection = true);
	void AddObject(GenericObject* object);
	void AddLight(GenericLight* light);
	void GetVisibleLightsFromPoint(glm::vec3 position, std::vector<GenericLight*>& outVisibleLights, std::vector<float>& outPercentVisible);
	glm::vec3 GetBackgroundColorFromRay(Ray ray);
	void Finalize();
};
