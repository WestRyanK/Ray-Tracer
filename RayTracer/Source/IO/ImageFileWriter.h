#pragma once

#include <string>

class Image;

class ImageFileWriter
{
public:
	ImageFileWriter();
	void Save(std::string filename, Image* image);

protected:
	virtual void WriteFileContents(std::ofstream& file, Image* image) = 0;
};
