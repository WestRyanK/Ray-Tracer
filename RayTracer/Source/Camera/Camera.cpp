#include "Camera.h"
#include "../Ray.h"
#include "../RandomGenerator.h"

Camera::Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up, float fovXDegrees, unsigned int horizontalResolution, unsigned int verticalResolution, unsigned int raysPerPixel) :
	lookFrom(lookFrom),
	lookAt(lookAt),
	up(up),
	fovXDegrees(fovXDegrees),
	horizontalResolution(horizontalResolution),
	verticalResolution(verticalResolution),
	raysPerPixel(raysPerPixel)
{
	this->ComputeCameraValues();
}

void Camera::ComputeCameraValues()
{
	this->n = glm::normalize(this->lookAt - this->lookFrom);
	this->u = glm::normalize(glm::cross(this->n, this->up));
	this->v = glm::normalize(glm::cross(this->u, this->n));

	float aspectRatio = (float)this->horizontalResolution / this->verticalResolution;
	this->viewPlaneWidth = 2 * d * glm::tan(glm::radians(this->fovXDegrees / 2.0f));
	this->viewPlaneHeight = this->viewPlaneWidth / aspectRatio;
	this->vpc = this->lookFrom + this->n * this->d;
	this->vpMin = this->vpc - (this->viewPlaneWidth / 2.0f) * this->u - (this->viewPlaneHeight / 2.0f) * this->v;

	this->resolutionMultiplier = (unsigned int)glm::round(glm::sqrt(raysPerPixel));
	this->raysPerPixel = this->resolutionMultiplier * this->resolutionMultiplier;
	this->subpixelHorizontalResolution = this->horizontalResolution * this->resolutionMultiplier;
	this->subpixelVerticalResolution = this->verticalResolution * this->resolutionMultiplier;

	this->subpixelWidth = this->viewPlaneWidth / (float)this->subpixelHorizontalResolution;
	this->subpixelHeight = this->viewPlaneHeight / (float)this->subpixelVerticalResolution;
}

std::vector<Ray> Camera::GetRaysForPixel(unsigned int x, unsigned int y)
{
	std::vector<Ray> rays;
	for (size_t subY = 0; subY < this->resolutionMultiplier; subY++)
	{
		for (size_t subX = 0; subX < this->resolutionMultiplier; subX++)
		{
			glm::vec3 pixelCenter = this->GetPixelCenter((unsigned int)x, (unsigned int)y, subX, subY);
			glm::vec3 direction = pixelCenter - this->lookFrom;
			Ray ray(pixelCenter, direction);
			rays.push_back(ray);
		}
	}

	return rays;
}

glm::vec3 Camera::GetPixelCenter(unsigned int x, unsigned int y, unsigned int subX, unsigned int subY)
{
	y = this->verticalResolution - y;

	glm::vec3 pixelCenter = this->vpMin + 
		((x * this->resolutionMultiplier + subX) * this->subpixelWidth * this->u) + 
		((y * this->resolutionMultiplier + subY) * this->subpixelHeight * this->v);

	if (this->raysPerPixel > 1)
	{
		pixelCenter += (RandomGenerator::RandomBetween(-this->subpixelWidth * this->jitterAmount, this->subpixelWidth * this->jitterAmount) * this->u) +
			(RandomGenerator::RandomBetween(-this->subpixelHeight * this->jitterAmount, this->subpixelHeight * this->jitterAmount) * this->v);
	}

	return pixelCenter;
}

unsigned int Camera::GetHorizontalResolution()
{
	return this->horizontalResolution;
}

unsigned int Camera::GetVerticalResolution()
{
	return this->verticalResolution;
}

void Camera::SetPixelJitterAmount(float jitterAmount)
{
	this->jitterAmount = jitterAmount;
}
