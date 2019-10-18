#pragma once
#include <glm/glm.hpp>

class Image
{
protected:
	unsigned int width;
	unsigned int height;
	glm::vec3* data;

public:
	Image(unsigned int width, unsigned int height);
	void SetPixel(unsigned int x, unsigned int y, glm::vec3 color);
	glm::vec3 GetPixel(unsigned int x, unsigned int y);
	unsigned int GetWidth();
	unsigned int GetHeight();
	~Image();
};