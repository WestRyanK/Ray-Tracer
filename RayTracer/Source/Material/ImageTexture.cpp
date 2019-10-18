#include "ImageTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImageTexture::ImageTexture(std::string texturePath) : 
	texturePath(texturePath)
{
	this->textureData = stbi_load(texturePath.c_str(), &(this->width), &(this->height), &(this->channelCount), 0); 
}

glm::vec3 ImageTexture::GetColor(glm::vec2 textureCoordinates)
{
	int x = (int)(glm::fract(textureCoordinates.x) * this->width);
	int y = (int)(glm::fract(textureCoordinates.y) * this->height);


	unsigned int index = y * this->width * this->channelCount + x * this->channelCount;
	unsigned char r = this->textureData[index + 0];
	unsigned char g = this->textureData[index + 1];
	unsigned char b = this->textureData[index + 2];

	glm::vec3 color((float)r / 255, (float)g / 255, (float)b / 255);
	return color;
}
