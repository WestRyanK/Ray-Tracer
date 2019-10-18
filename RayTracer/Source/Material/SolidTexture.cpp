#include "SolidTexture.h"

SolidTexture::SolidTexture(glm::vec3 color)
{
	this->color = color;
}

SolidTexture::SolidTexture(float r, float g, float b) : SolidTexture(glm::vec3(r, g, b))
{

}

glm::vec3 SolidTexture::GetColor(glm::vec2 textureCoordinates)
{
	return this->color;
}
