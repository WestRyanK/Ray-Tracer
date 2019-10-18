#include "SolidBackground.h"
#include "GenericBackground.h"
#include "../Ray.h"

SolidBackground::SolidBackground(glm::vec3 backgroundColor) : GenericBackground(), backgroundColor(backgroundColor)
{

}

glm::vec3 SolidBackground::GetColorFromRay(Ray ray)
{
	return this->backgroundColor;
}

glm::vec3 SolidBackground::GetBackgroundColor()
{ 
	return this->backgroundColor;
}
