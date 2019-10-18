#include "GenericObject.h"
#include "../../Scene/Scene.h"
#include "../../Ray.h"
#include "../../Material/Material.h"

GenericObject::GenericObject(Material* material) : material(material)
{

}

glm::vec3 GenericObject::GetColorFromRay(Scene* scene, Ray ray, unsigned int rayDepth, glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates)
{
	return this->material->GetColor(scene, ray, rayDepth, position, normal, textureCoordinates);
}
