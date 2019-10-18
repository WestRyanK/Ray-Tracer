#pragma once

#include <glm/glm.hpp>
#include "ITexture.h"
#include <string>

class Scene;
class Ray;

class ImageTexture : public ITexture
{
private:
	std::string texturePath;
	int width;
	int height;
	int channelCount;
	unsigned char* textureData;

public:
	ImageTexture(std::string texturePath);
	virtual glm::vec3 GetColor(glm::vec2 textureCoordinates);
};
