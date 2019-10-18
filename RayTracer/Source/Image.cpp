#include "Image.h"

Image::Image(unsigned int width, unsigned int height) : width(width), height(height)
{
	this->data = new glm::vec3[width * height];

	glm::vec3 defaultPixelValue(0, 0, 0);

	for (size_t y = 0; y < this->height; y++)
	{
		for (size_t x = 0; x < this->width; x++)
		{
			this->data[y * this->width + x] = defaultPixelValue;
		}
	}
}

Image::~Image()
{
	delete[] this->data;
}

void Image::SetPixel(unsigned int x, unsigned int y, glm::vec3 color)
{
	this->data[y * this->width + x] = color;
}
 
glm::vec3 Image::GetPixel(unsigned int x, unsigned int y)
{
	return this->data[y * this->width + x];
}

unsigned int Image::GetWidth()
{
	return this->width;
}

unsigned int Image::GetHeight()
{
	return this->height;
}