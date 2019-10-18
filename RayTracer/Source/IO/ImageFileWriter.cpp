#include "ImageFileWriter.h"
#include <string>
#include <iostream>
#include <fstream>
#include "../Image.h"


ImageFileWriter::ImageFileWriter()
{

}

void ImageFileWriter::Save(std::string filename, Image* image)
{
	std::ofstream file;
	file.open(filename, std::ios::trunc | std::ios::out);

	this->WriteFileContents(file, image);

	file.close();
}