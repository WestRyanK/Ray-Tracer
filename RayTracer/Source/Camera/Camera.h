#pragma once

#include <glm/glm.hpp>
#include <vector>

class Ray;

class Camera
{
private:
	void ComputeCameraValues();

	// Input values
	glm::vec3 lookFrom;
	glm::vec3 lookAt;
	glm::vec3 up;
	float fovXDegrees;
	unsigned int horizontalResolution;
	unsigned int verticalResolution;
	unsigned int subpixelHorizontalResolution;
	unsigned int subpixelVerticalResolution;
	unsigned int resolutionMultiplier;
	unsigned int raysPerPixel;
	float jitterAmount = 0.5f;
	const float d = 1.0f;

	// Calculated values
	glm::vec3 n;
	glm::vec3 u;
	glm::vec3 v;
	glm::vec3 vpc;
	glm::vec3 vpMin;
	float viewPlaneWidth;
	float viewPlaneHeight;
	float subpixelWidth;
	float subpixelHeight;

protected:
	virtual glm::vec3 GetPixelCenter(unsigned int x, unsigned int y, unsigned int subX, unsigned int subY);

public:
	/*
		RaysPerPixel is rounded to the nearest perfect square to facilitate subdividing each pixel.
	*/
	Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up, float fovXDegrees, unsigned int horizontalResolution, unsigned int verticalResolution, unsigned int raysPerPixel);
	std::vector<Ray> GetRaysForPixel(unsigned int x, unsigned int y);
	unsigned int GetHorizontalResolution();
	unsigned int GetVerticalResolution();
	void SetPixelJitterAmount(float jitterAmount);
};
