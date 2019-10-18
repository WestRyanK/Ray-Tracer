#include "PpmImageFileWriter.h"
#include <iostream>
#include <fstream>
#include "../Image.h"

PpmImageFileWriter::PpmImageFileWriter()
{

}

void PpmImageFileWriter::WriteFileContents(std::ofstream& file, Image* image)
{
	file << "P3" << std::endl;
	file << image->GetWidth() << " " << image->GetHeight() << std::endl;
	file << "255" << std::endl;
	for (size_t y = 0; y < image->GetHeight(); y++)
	{
		for (size_t x = 0; x < image->GetWidth(); x++)
		{
			glm::vec3 pixel = glm::clamp(image->GetPixel((unsigned int)x, (unsigned int)y) * 255.0f, glm::vec3(0.0f), glm::vec3(255.0f));
			file << (int)pixel.r << " " << (int)pixel.g << " " << (int)pixel.b << "  ";
		}
		file << std::endl;
	}
}