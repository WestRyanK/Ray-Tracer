#pragma once

#include "ImageFileWriter.h"

class Image;

class PpmImageFileWriter : public ImageFileWriter
{
public:
	PpmImageFileWriter();

protected:
	virtual void WriteFileContents(std::ofstream& file, Image* image);
};
