// RayTracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "RayTracer.h"
#include "IO/PpmImageFileWriter.h"
#include "Ray.h"
#include "Scene/Scene.h"
#include "Scene/SolidBackground.h"
#include "Scene/Object/SphereObject.h"
#include "Scene/Object/TriangleObject.h"
#include "Scene/Object/MeshObject.h"
#include "Scene/Object/ObjectList.h"
#include "Scene/Object/ObjectHierarchy.h"
#include "Scene/Light/DirectionalLight.h"
#include "Scene/Light/PointLight.h"
#include "Scene/Light/SphereAreaLight.h"
#include "Material/Material.h"
#include "Material/AmbientMaterialComponent.h"
#include "Material/DiffuseMaterialComponent.h"
#include "Material/SpecularMaterialComponent.h"
#include "Material/ReflectiveMaterialComponent.h"
#include "Material/RefractiveMaterialComponent.h"
#include "Material/SolidTexture.h"
#include "Material/ImageTexture.h"
#include "Camera/Camera.h"
#include "Image.h"
#include "RandomGenerator.h"

#include <Windows.h>

const float ft = 12.0f;

GenericObject* GetPlane(glm::vec2 size, glm::vec3 start, bool isCentered)
{
	if (isCentered)
	{
		start = glm::vec3(start.x - size.x * 0.5f, start.y, start.z - size.y * 0.5f);
	}
	glm::vec3 xOffset = size.x * glm::vec3(1, 0, 0);
	glm::vec3 yOffset = size.y * glm::vec3(0, 0, 1);

	MeshObject* checkerboard = new MeshObject();
	Material* material = new Material();
	//ImageTexture* texture = new ImageTexture("painting.jpg");
	ImageTexture* texture = new ImageTexture("Assets\\checker.png");
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(texture));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(texture));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(1.0f, 1.0f, 1.0f), 16));
	checkerboard->AddTriangle(new TriangleObject(material, start, start + yOffset, start + xOffset, true));
	checkerboard->AddTriangle(new TriangleObject(material, start + xOffset + yOffset, start + xOffset, start + yOffset, true, glm::vec2(1,0), glm::vec2(1, 1), glm::vec2(0,0)));
	checkerboard->Finalize();
	return checkerboard;
}

std::vector<TriangleObject*> GetWallTriangles(glm::vec3 cornerA, glm::vec3 cornerB, glm::vec3 cornerC, Material* material, glm::vec2 textureRepeats)
{
	std::vector<TriangleObject*> triangles;

	TriangleObject* object = new TriangleObject(material, cornerA, cornerB, cornerC, true, 
		glm::vec2(0, textureRepeats.y), glm::vec2(0, 0), glm::vec2(textureRepeats.x,textureRepeats.y));
	triangles.push_back(object);
	object = new TriangleObject(material, cornerA + (cornerB - cornerA) + cornerC - cornerA, cornerB, cornerC,  true, 
		glm::vec2(textureRepeats.x,0), glm::vec2(0,0), glm::vec2(textureRepeats.x, textureRepeats.y));
	triangles.push_back(object);
	return triangles;
}

MeshObject* GetWall(glm::vec3 cornerA, glm::vec3 cornerB, glm::vec3 cornerC, Material* material, glm::vec2 textureRepeats)
{
	MeshObject* mesh = new MeshObject();
	std::vector<TriangleObject*> triangles = GetWallTriangles(cornerA, cornerB, cornerC, material, textureRepeats);
	for (TriangleObject* triangle : triangles)
	{
		mesh->AddTriangle(triangle);
	}

	mesh->Finalize();
	return mesh;
}

GenericObject* GetCheckerboard(unsigned int rows, unsigned int columns, float squareWidth, float squareHeight, float startX, float startY, bool isCentered)
{
	MeshObject* checkerboard = new MeshObject();
	Material** materials = new Material*[2];
	materials[0] = new Material();
	materials[1] = new Material();
	materials[0]->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.2f))));
	materials[0]->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.2f))));
	materials[0]->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1.0f)), 16));
	materials[1]->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.8f))));
	materials[1]->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.8f))));
	materials[1]->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1,1,1)), 16));

	if (isCentered)
	{
		startX -= columns * squareWidth / 2.0f;
		startY -= rows * squareHeight / 2.0f;
	}

	float h = 0;
	for (size_t y = 0; y < rows; y++)
	{
		for (size_t x = 0; x < columns; x++)
		{
			TriangleObject* triangle = new TriangleObject(materials[(x + y) % 2],
				glm::vec3((x + 1) * squareWidth + startX, h, y * squareHeight + startY),
				glm::vec3(x * squareWidth + startX, h, y * squareHeight + startY),
				glm::vec3(x * squareWidth + startX, h, (y + 1) * squareHeight + startY), false);
			checkerboard->AddTriangle(triangle);

			triangle = new TriangleObject(materials[(x + y) % 2],
				glm::vec3(x * squareWidth + startX, h, (y + 1) * squareHeight + startY),
				glm::vec3((x + 1) * squareWidth + startX, h, (y + 1) * squareHeight + startY),
				glm::vec3((x + 1) * squareWidth + startX, h, y * squareHeight + startY), false);
			checkerboard->AddTriangle(triangle);
		}
	}

	checkerboard->Finalize();
	return checkerboard;
}

void AddCheckerboard(Scene* scene, unsigned int rows, unsigned int columns, float squareWidth, float squareHeight, float startX, float startY, bool isCentered)
{
	Material** materials = new Material*[2];
	materials[0] = new Material();
	materials[1] = new Material();
	materials[0]->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.2f))));
	materials[0]->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.2f))));
	materials[0]->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1.0f)), 16));
	materials[1]->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.8f))));
	materials[1]->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.8f))));
	materials[1]->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1,1,1)), 16));

	if (isCentered)
	{
		startX -= columns * squareWidth / 2.0f;
		startY -= rows * squareHeight / 2.0f;
	}

	float h = 0;
	for (size_t y = 0; y < rows; y++)
	{
		for (size_t x = 0; x < columns; x++)
		{
			TriangleObject* triangle = new TriangleObject(materials[(x + y) % 2],
				glm::vec3((x + 1) * squareWidth + startX, h, y * squareHeight + startY),
				glm::vec3(x * squareWidth + startX, h, y * squareHeight + startY),
				glm::vec3(x * squareWidth + startX, h, (y + 1) * squareHeight + startY), false);
			scene->AddObject(triangle);

			triangle = new TriangleObject(materials[(x + y) % 2],
				glm::vec3(x * squareWidth + startX, h, (y + 1) * squareHeight + startY),
				glm::vec3((x + 1) * squareWidth + startX, h, (y + 1) * squareHeight + startY),
				glm::vec3((x + 1) * squareWidth + startX, h, y * squareHeight + startY), false);
			scene->AddObject(triangle);
		}
	}
}

GenericObject* GetPlane(glm::vec3 cornerA, glm::vec3 cornerB, glm::vec3 cornerC, float jitterAmount, unsigned int raysPerReflection, bool isTranslucent, bool isReflective)
{
	MeshObject* mesh = new MeshObject();

	Material* material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(1, 1, 1))));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(1, 0, 1))));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1, 1, 1)), 32));
	if (isReflective)
		material->AddMaterialComponent(0.8f, new ReflectiveMaterialComponent(jitterAmount, raysPerReflection));
	if (isTranslucent)
		material->AddMaterialComponent(0.8f, new RefractiveMaterialComponent(RefractiveMaterialComponent::INDEX_OF_REFRACTION_WATER, jitterAmount, raysPerReflection));

	TriangleObject* object = new TriangleObject(material, cornerA, cornerC, cornerB, true);
	mesh->AddTriangle(object);
	object = new TriangleObject(material, cornerB, cornerC, cornerA + (cornerB - cornerA) + cornerC - cornerA, true);
	mesh->AddTriangle(object);

	return mesh;
}

GenericObject* GetPyramid(glm::vec3 center, glm::vec3 frontVector, float baseSize, float height)
{
	Material* material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(1.0f, 0.95f, 0.0f))));
	material->AddMaterialComponent(0.5f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(1.0f, 0.95f, 0.0f))));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1.0f, 0.95f, 0.0f)), 30));
	material->AddMaterialComponent(0.5f, new ReflectiveMaterialComponent(0.005f, 4));
	MeshObject* mesh = new MeshObject();
	mesh->AddTriangle(new TriangleObject(material, center + frontVector * glm::vec3(baseSize, 0, baseSize), center + frontVector * glm::vec3(baseSize, 0, -baseSize), center + glm::vec3(0, height, 0), true));
	mesh->AddTriangle(new TriangleObject(material, center + frontVector * glm::vec3(baseSize, 0, -baseSize), center + frontVector * glm::vec3(-baseSize, 0, -baseSize), center + glm::vec3(0, height, 0), true));
	mesh->AddTriangle(new TriangleObject(material, center + frontVector * glm::vec3(-baseSize, 0, -baseSize), center + frontVector * glm::vec3(-baseSize, 0, baseSize), center + glm::vec3(0, height, 0), true));
	mesh->AddTriangle(new TriangleObject(material, center + frontVector * glm::vec3(-baseSize, 0, baseSize), center + frontVector * glm::vec3(baseSize, 0, baseSize), center + glm::vec3(0, height, 0), true));

	return mesh;
}

GenericObject* GetCube(glm::vec3 center, glm::vec3 frontVector, glm::vec3 up, glm::vec3 size, float jitterAmount, unsigned int raysPerReflection, ITexture* texture = nullptr)
{
	glm::vec3 u = glm::normalize(frontVector);
	glm::vec3 v = glm::cross(up, u);
	glm::vec3 t = glm::cross(u, v);

	Material* material = new Material();
	if (texture == nullptr)
	{
		//material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.8f, 0.8f, 1.0f))));
		material->AddMaterialComponent(0.05f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.8f, 0.8f, 1.0f))));
		material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1, 1, 1)), 4));
		material->AddMaterialComponent(0.2f, new ReflectiveMaterialComponent(jitterAmount, raysPerReflection));
		material->AddMaterialComponent(0.8f, new RefractiveMaterialComponent(RefractiveMaterialComponent::INDEX_OF_REFRACTION_FLINT_GLASS, jitterAmount, raysPerReflection));
	}
	else
	{
		material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(texture));
		material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(texture));
	}

	glm::vec3 vert[]
	{
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(-1, 1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, 1),
		glm::vec3(1, 1, 1)
	};
	// 6 7
	// 4 5

	// 2 3
	// 0 1

	std::vector<unsigned int> index
	{
		//bottom x
		0, 1, 2,
		//top x
		4, 6, 5,
		//left x
		6, 4, 2,
		//right x
		1, 5, 3,
		//back x
		0, 4, 1,
		//back x
		2, 3, 6
	};

	MeshObject* mesh = new MeshObject();
	
	for (size_t i = 0; i < index.size(); i += 3)
	{
		std::vector<TriangleObject*> triangles = GetWallTriangles(
			vert[index[i + 0]].x * v * size.x + vert[index[i + 0]].y * t * size.y + vert[index[i + 0]].z * u * size.z + center,
			vert[index[i + 1]].x * v * size.x + vert[index[i + 1]].y * t * size.y + vert[index[i + 1]].z * u * size.z + center,
			vert[index[i + 2]].x * v * size.x + vert[index[i + 2]].y * t * size.y + vert[index[i + 2]].z * u * size.z + center, material, glm::vec2(1, 1));
		for (TriangleObject* triangle : triangles)
		{
			mesh->AddTriangle(triangle);
		}
	}

	mesh->Finalize();
	return mesh;
}

void AddOrbLights(Scene* scene, float hallwayWidth, float height, float spacing, float startOffset, unsigned int count)
{
	Material* material = new Material();
	glm::vec3 orbLightColor(0.35f, 0.25f, 0.1f);
	material->AddMaterialComponent(0.1f, new DiffuseMaterialComponent(new SolidTexture(orbLightColor)));
	material->AddMaterialComponent(2.0f, new AmbientMaterialComponent(new SolidTexture(orbLightColor)));
	material->AddMaterialComponent(0.9f, new RefractiveMaterialComponent(RefractiveMaterialComponent::INDEX_OF_REFRACTION_FLINT_GLASS, 0.03f, 9));
	float orbRadius = 0.5f * ft;
	orbLightColor *= 0.25f;
	float orbDecay = 0.008f;
	for (size_t i = 0; i < count; i++)
	{
		glm::vec3 orbPosition(hallwayWidth / 2 - orbRadius * 1.2f, height, spacing * i + startOffset);
		scene->AddObject(new SphereObject(material, orbPosition, orbRadius));
		scene->AddLight(new SphereAreaLight(orbPosition, orbLightColor, orbRadius * 1.1f, orbDecay));
		//scene->AddLight(new PointLight(orbPosition, orbLightColor, orbRadius * 1.1f, orbDecay));
		orbPosition.x = -orbPosition.x;
		scene->AddObject(new SphereObject(material, orbPosition, orbRadius));
		scene->AddLight(new SphereAreaLight(orbPosition, orbLightColor, orbRadius * 1.1f, orbDecay));
		//scene->AddLight(new PointLight(orbPosition, orbLightColor, orbRadius * 1.1f, orbDecay));
	}
}

Scene* GetHallwayScene(unsigned int width, unsigned int height)
{
	float hallwayWidth = 5.0f * ft;
	float hallwayHeight = 7.5f * ft;
	float hallwayLength = 30.0f * ft;
	float hallwayBoardHeight = 0.0f * ft;
	float personHeight = 5.5f * ft;

	Material* material;

	Scene* scene = new Scene(new ObjectHierarchy(2, 10));
	Camera* camera = new Camera(glm::vec3(-0.5f * ft, personHeight, 30 * ft), glm::vec3(1 * ft, personHeight * 0.5f, 0), glm::vec3(0, 1, 0), 45.0f, width, height, 16);
	scene->SetAmbientLight(glm::vec3(0.24f,0.24f,0.3f));

	scene->SetCamera(camera);
	SolidBackground* background = new SolidBackground(glm::vec3(0.5f, 0.6f, 0.8f));
	scene->SetBackground(background);



	ImageTexture* wall = new ImageTexture("Assets\\stone_wall.jpg");
	// Hallway 
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(wall));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(wall));
	scene->AddObject(GetWall(glm::vec3(hallwayWidth / 2, hallwayBoardHeight, hallwayLength), glm::vec3(hallwayWidth / 2, hallwayBoardHeight, 0), glm::vec3(hallwayWidth / 2, hallwayHeight, hallwayLength), 
		material, glm::vec2(2,1)));
	scene->AddObject(GetWall(glm::vec3(-hallwayWidth / 2, hallwayBoardHeight, hallwayLength), glm::vec3(-hallwayWidth / 2, hallwayBoardHeight, 0), glm::vec3(-hallwayWidth / 2, hallwayHeight, hallwayLength), 
		material, glm::vec2(2,1)));
	// Ceiling
	ImageTexture* ceiling = new ImageTexture("Assets\\stone_wall.jpg");
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(ceiling));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(ceiling));
	scene->AddObject(GetWall(glm::vec3(hallwayWidth / 2, hallwayHeight, hallwayLength), glm::vec3(hallwayWidth / 2, hallwayHeight, 0), glm::vec3(-hallwayWidth / 2, hallwayHeight, hallwayLength), 
		material, glm::vec2(2,1)));
	// Floor
	ImageTexture* floor = new ImageTexture("Assets\\dirt2.jpg");
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(floor));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(floor));
	scene->AddObject(GetWall(glm::vec3(hallwayWidth / 2, 0, hallwayLength), glm::vec3(hallwayWidth / 2, 0, 0), glm::vec3(-hallwayWidth / 2, 0, hallwayLength), material, glm::vec2(5,1)));
	// Door
	ImageTexture* door = new ImageTexture("Assets\\door.jpg");
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(door));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(door));
	scene->AddObject(GetWall(glm::vec3(hallwayWidth / 2, 0, 0), glm::vec3(hallwayWidth / 2, hallwayHeight, 0), glm::vec3(-hallwayWidth / 2, 0, 0), material, glm::vec2(1,1)));

	AddOrbLights(scene, hallwayWidth, personHeight, 13 * ft, 10 * ft, 2);

	ImageTexture* crate = new ImageTexture("Assets\\crate.jpg");
	scene->AddObject(GetCube(glm::vec3(1 * ft, 1 * ft, 11 * ft), glm::vec3(1.0f, 0.0f, 2.0f), glm::vec3(0, 1, 0), glm::vec3(1.0f * ft, 1.0f * ft, 1.0f * ft), 0.0f, 1.0f, crate));

	// Silver Sphere
	material = new Material();
	glm::vec3 sphereColor(0.9f, 0.9f, 0.9f);
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(sphereColor)));
	material->AddMaterialComponent(0.5f, new DiffuseMaterialComponent(new SolidTexture(sphereColor)));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(sphereColor), 20.0f));
	material->AddMaterialComponent(0.5f, new ReflectiveMaterialComponent(0.06f, 8));
	float sphereRadius = 0.8f * ft;
	scene->AddObject(new SphereObject(material, glm::vec3(-hallwayWidth / 2 + sphereRadius, sphereRadius, 15 * ft), sphereRadius));

	return scene;
}

Scene* GetGalleryScene(unsigned int width, unsigned int height)
{
	float galleryWidth = 20.0f * ft;
	float galleryHeight = 10.0f * ft;
	float galleryLength = 25.0f * ft;
	float galleryBoardHeight = 0.0f * ft;
	float personHeight = 5.5f * ft;

	Material* material;

	Scene* scene = new Scene(new ObjectHierarchy(2, 20));
	Camera* camera = new Camera(glm::vec3(-9.0f * ft, personHeight, 30 * ft), glm::vec3(1 * ft, personHeight * 0.5f, 0), glm::vec3(0, 1, 0), 45.0f, width, height, 16);
	scene->SetAmbientLight(glm::vec3(0.3,0.3f,0.31f));

	scene->SetCamera(camera);
	SolidBackground* background = new SolidBackground(glm::vec3(0.5f, 0.6f, 0.8f));
	scene->SetBackground(background);


	// Walls wallpaper
	ImageTexture* wall = new ImageTexture("Assets\\damask_wallpaper.jpg");
	ImageTexture* wall2 = new ImageTexture("Assets\\damask_wallpaper2.jpg");
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(wall));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(wall));
	scene->AddObject(GetWall(glm::vec3(galleryWidth / 2, galleryBoardHeight, galleryLength), glm::vec3(galleryWidth / 2, galleryBoardHeight, 0), glm::vec3(galleryWidth / 2, galleryHeight, galleryLength), 
		material, glm::vec2(10, 4)));
	scene->AddObject(GetWall(glm::vec3(-galleryWidth / 2, galleryBoardHeight, galleryLength), glm::vec3(-galleryWidth / 2, galleryBoardHeight, 0), glm::vec3(-galleryWidth / 2, galleryHeight, galleryLength), 
		material, glm::vec2(10, 4)));
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(wall2));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(wall2));
	scene->AddObject(GetWall(glm::vec3(galleryWidth / 2, 0, 0), glm::vec3(galleryWidth / 2, galleryHeight, 0), glm::vec3(-galleryWidth / 2, 0, 0), material, glm::vec2(4, 10)));
	// Ceiling
	ImageTexture* ceiling = new ImageTexture("Assets\\ceiling2.jpg");
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(ceiling));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(ceiling));
	material->AddMaterialComponent(0.2f, new SpecularMaterialComponent(new SolidTexture(1.0f, 1.0f, 1.0f), 10.0f));
	scene->AddObject(GetWall(glm::vec3(galleryWidth / 2, galleryHeight, galleryLength), glm::vec3(galleryWidth / 2, galleryHeight, 0), glm::vec3(-galleryWidth / 2, galleryHeight, galleryLength), 
		material, glm::vec2(8,8)));
	// Floor
	ImageTexture* floor = new ImageTexture("Assets\\marble_tile2.jpg");
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(floor));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(floor));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(1.0f, 1.0f, 1.0f), 32.0f));
	scene->AddObject(GetWall(glm::vec3(galleryWidth / 2, 0, galleryLength), glm::vec3(galleryWidth / 2, 0, 0), glm::vec3(-galleryWidth / 2, 0, galleryLength), material, glm::vec2(8,8)));
	// Door
	ImageTexture* door = new ImageTexture("Assets\\door2.jpg");
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(door));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(door));
	scene->AddObject(GetWall(glm::vec3(galleryWidth / 2 - 0.1f * ft, 0, 3.0f * ft), glm::vec3(galleryWidth / 2 - 0.1f * ft, 7.0f * ft, 3.0f * ft), glm::vec3(galleryWidth / 2 - 0.1f * ft, 0, 6.5f * ft), 
		material, glm::vec2(1,1)));

	// Mona Lisa
	ImageTexture* mona = new ImageTexture("Assets\\mona_lisa.jpg");
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(mona));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(mona));
	scene->AddObject(GetWall(glm::vec3(-7 * ft, 3 * ft, 0.1 * ft), glm::vec3(-7 * ft, 6 * ft, 0.1 * ft), glm::vec3(-5 * ft, 3 * ft, 0.1 * ft), material, glm::vec2(1,1)));

	// Starry Night
	ImageTexture* starry = new ImageTexture("Assets\\starry_night.jpg");
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(starry));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(starry));
	scene->AddObject(GetWall(glm::vec3(2.5f * ft, 3 * ft, 0.1 * ft), glm::vec3(2.5f * ft, 6.5f * ft, 0.1 * ft), glm::vec3(8 * ft, 3 * ft, 0.1 * ft), material, glm::vec2(1,1)));

	glm::vec3 spherePosition(4.0f * ft, 9.4f * ft, 5.0f * ft);
	float sphereLightRadius = 0.4f * ft;
	Material* sphereLightMat = new Material();
	sphereLightMat->AddMaterialComponent(6.0f, new AmbientMaterialComponent(new SolidTexture(1.0f, 0.9f, 0.8f)));
	scene->AddObject(new SphereObject(sphereLightMat, spherePosition, sphereLightRadius));
	glm::vec3 sphereLightColor(1.0f, 0.8f, 0.7f);
	sphereLightColor *= 0.8f;
	scene->AddLight(new SphereAreaLight(spherePosition, sphereLightColor, sphereLightRadius * 1.5f, 0.015f));
	spherePosition.x = -6.0f * ft;
	scene->AddObject(new SphereObject(sphereLightMat, spherePosition, sphereLightRadius));
	scene->AddLight(new SphereAreaLight(spherePosition, sphereLightColor, sphereLightRadius * 1.5f, 0.015f));

	ImageTexture* pedestal = new ImageTexture("Assets\\marble.jpg");
	scene->AddObject(GetCube(glm::vec3(3.0f * ft, 0.25f * ft, 14 * ft), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0, 1, 0), glm::vec3(2.0f * ft, 0.25f * ft, 2.0f * ft),0.0f, 1.0f, pedestal));
	scene->AddObject(GetCube(glm::vec3(3.0f * ft, 0.5f * ft, 14 * ft), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0, 1, 0), glm::vec3(1.5f * ft, 0.25f * ft, 1.5f * ft),0.0f, 1.0f, pedestal));
	scene->AddObject(GetCube(glm::vec3(3.0f * ft, 0.75f * ft, 14 * ft), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0, 1, 0), glm::vec3(1.0f * ft, 0.25f * ft, 1.0f * ft),0.0f, 1.0f, pedestal));

	scene->AddObject(GetCube(glm::vec3(-5.0f * ft, 0.25f * ft, 14 * ft), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0, 1, 0), glm::vec3(2.0f * ft, 0.25f * ft, 2.0f * ft),0.0f, 1.0f, pedestal));
	scene->AddObject(GetCube(glm::vec3(-5.0f * ft, 0.5f * ft, 14 * ft), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0, 1, 0), glm::vec3(1.5f * ft, 0.25f * ft, 1.5f * ft),0.0f, 1.0f, pedestal));
	scene->AddObject(GetCube(glm::vec3(-5.0f * ft, 0.75f * ft, 14 * ft), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0, 1, 0), glm::vec3(1.0f * ft, 0.25f * ft, 1.0f * ft),0.0f, 1.0f, pedestal));

	scene->AddObject(GetCube(glm::vec3(3.0f * ft, 0.25f * ft, 6 * ft), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0, 1, 0), glm::vec3(2.0f * ft, 0.25f * ft, 2.0f * ft),0.0f, 1.0f, pedestal));
	scene->AddObject(GetCube(glm::vec3(3.0f * ft, 0.5f * ft, 6 * ft), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0, 1, 0), glm::vec3(1.5f * ft, 0.25f * ft, 1.5f * ft),0.0f, 1.0f, pedestal));
	scene->AddObject(GetCube(glm::vec3(3.0f * ft, 0.75f * ft, 6 * ft), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0, 1, 0), glm::vec3(1.0f * ft, 0.25f * ft, 1.0f * ft),0.0f, 1.0f, pedestal));

	scene->AddObject(GetCube(glm::vec3(-5.0f * ft, 0.25f * ft, 6 * ft), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0, 1, 0), glm::vec3(2.0f * ft, 0.25f * ft, 2.0f * ft),0.0f, 1.0f, pedestal));
	scene->AddObject(GetCube(glm::vec3(-5.0f * ft, 0.5f * ft, 6 * ft), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0, 1, 0), glm::vec3(1.5f * ft, 0.25f * ft, 1.5f * ft),0.0f, 1.0f, pedestal));
	scene->AddObject(GetCube(glm::vec3(-5.0f * ft, 0.75f * ft, 6 * ft), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0, 1, 0), glm::vec3(1.0f * ft, 0.25f * ft, 1.0f * ft),0.0f, 1.0f, pedestal));

	// Gold Sphere
	material = new Material();
	glm::vec3 sphereColor(0.9f, 0.8f, 0.2f);
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(sphereColor)));
	material->AddMaterialComponent(0.3f, new DiffuseMaterialComponent(new SolidTexture(sphereColor)));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(sphereColor), 20.0f));
	material->AddMaterialComponent(0.7f, new ReflectiveMaterialComponent(0.2f, 2));
	float sphereRadius = 0.9f * ft;
	scene->AddObject(new SphereObject(material, glm::vec3(3 * ft, sphereRadius + 1 * ft, 6 * ft), sphereRadius));

	// Diamond Sphere
	material = new Material();
	glm::vec3 sphereColor2(0.9f, 0.9f, 0.9f);
	material->AddMaterialComponent(0.1f, new AmbientMaterialComponent(new SolidTexture(sphereColor2)));
	material->AddMaterialComponent(0.1f, new DiffuseMaterialComponent(new SolidTexture(sphereColor2)));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(sphereColor2), 20.0f));
	material->AddMaterialComponent(0.1f, new ReflectiveMaterialComponent(0.1f, 7));
	material->AddMaterialComponent(0.8f, new RefractiveMaterialComponent(RefractiveMaterialComponent::INDEX_OF_REFRACTION_FLINT_GLASS, 0.1f, 5));
	float sphereRadius2 = 1.2f * ft;
	scene->AddObject(new SphereObject(material, glm::vec3(-5 * ft, sphereRadius2 + 1 * ft, 14 * ft), sphereRadius2));

	scene->AddObject(GetCube(glm::vec3(-5 * ft, 1.75f * ft, 6 * ft), glm::vec3(3, 1, 2.0f), glm::vec3(0, 1, 0.5f), glm::vec3(0.5f * ft, 0.5f * ft, 0.5f * ft), 0.010f, 5));
	scene->AddObject(GetCube(glm::vec3(3 * ft, 2.5f * ft, 14 * ft), glm::vec3(1, 1, 1.0f), glm::vec3(0, 1, 1.0f), glm::vec3(1 * ft, 1 * ft, 1 * ft), 0.040f, 5));

	return scene;
}

Scene* GetScene6(unsigned int width, unsigned int height)
{
	Material* material;

	Scene* scene = new Scene(new ObjectList());
	Camera* camera = new Camera(glm::vec3(2, 7, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 45.0f, width, height, 1);
	scene->SetAmbientLight(glm::vec3(0.2f,0.2f,0.2f));

	scene->SetCamera(camera);
	SolidBackground* background = new SolidBackground(glm::vec3(0.5f, 0.6f, 0.8f));
	scene->SetBackground(background);

	scene->AddLight(new PointLight(glm::vec3(6,5,7), glm::vec3(0.85f, 0.8f, 0.8f)));
	scene->AddLight(new PointLight(glm::vec3(-5, 7, 12), glm::vec3(0.50f, 0.3f, 0.15f)));
	//scene->AddLight(new SphereAreaLight(glm::vec3(6,5,7), glm::vec3(0.85f, 0.8f, 0.8f), 1.0f));
	//scene->AddLight(new SphereAreaLight(glm::vec3(-5, 7, 12), glm::vec3(0.50f, 0.3f, 0.15f), 2.0f));

	// Green Sphere
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.2f, 0.9f, 0.2f))));
	material->AddMaterialComponent(0.2f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.2f, 0.9f, 0.2f))));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(0.2f, 0.9f, 0.2f)), 8.0f));
	material->AddMaterialComponent(0.8f, new ReflectiveMaterialComponent(0.0f, 1));
	scene->AddObject(new SphereObject(material, glm::vec3(5.0f, 1.0f, 3.0f), 1.0f));

	// Orange Sphere
	material = new Material();
	material->AddMaterialComponent(0.8f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.8f, 0.4f, 0.2f))));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.8f, 0.4f, 0.2f))));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1)), 2.0f));
	scene->AddObject(new SphereObject(material, glm::vec3(-2.5f, 1.5f, 12.0f), 1.5f));

	// Blue Sphere
	material = new Material();
	material->AddMaterialComponent(0.8f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.1f, 0.4f, 0.7f))));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.1f, 0.4f, 0.7f))));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1)), 2.0f));
	scene->AddObject(new SphereObject(material, glm::vec3(-2.0f, 1.5f, 8.0f), 0.5f));

	//scene->AddObject(GetCube(glm::vec3(0, 2.5f, 4.0f), glm::vec3(3, 1, 2.0f), glm::vec3(0, 1, 0.5f), 1.0f, 0.015f, 1));
	//scene->AddObject(GetCube(glm::vec3(3, 3.0f, 12.0f), glm::vec3(1, 2, 1.5f), glm::vec3(0, 1, 0), 0.5f, 0.00f, 1));

	scene->AddObject(GetPlane(glm::vec3(-3, 0, -4.0f), glm::vec3(6, 0, -3), glm::vec3(-3, 6, -4.0f), 0.0f, 1, false, true));

	ImageTexture* checker = new ImageTexture("Assets\\checker.png");
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(checker));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(checker));
	scene->AddObject(GetWall(glm::vec3(-30, 0, -30), glm::vec3(-30, 0, 30), glm::vec3(30, 0, -30), material, glm::vec2(4, 4)));
	//scene->AddObject(GetCheckerboard(6, 6, 5, 5, 0, 0, true));

	return scene;
}

Scene* GetScene5(unsigned int width, unsigned int height)
{
	Scene* scene = new Scene(new ObjectList());
	Camera* camera = new Camera(glm::vec3(5, 5, 18), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 45.0f, width, height, 4);
	scene->SetAmbientLight(glm::vec3(0.2f,0.2f,0.2f));

	scene->SetCamera(camera);
	SolidBackground* background = new SolidBackground(glm::vec3(0.5f, 0.6f, 0.8f));
	scene->SetBackground(background);

	scene->AddLight(new SphereAreaLight(glm::vec3(6, 6, 3), glm::vec3(0.9f, 0.8f, 0.7f), 1));
	scene->AddLight(new SphereAreaLight(glm::vec3(-5, 7, 10), glm::vec3(0.5f, 0.3f, 0.1f), 1));

	// Bronze Ball
	Material* material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.7f, 0.45f, 0.1f))));
	material->AddMaterialComponent(0.7f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.7f, 0.45f, 0.1f))));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(0.5f, 0.45f, 0.0f)), 20.0f));
	material->AddMaterialComponent(0.3f, new ReflectiveMaterialComponent(0.06f, 8));
	scene->AddObject(new SphereObject(material, glm::vec3(-2.0f, 0.65f, 8.0f), 0.65f));

	scene->AddObject(GetPyramid(glm::vec3(-1.5f, 0.0f, 4.0f), glm::normalize(glm::vec3(1,0,1)), 0.5f, 1.5f));

	// Glass Ball
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(1))));
	material->AddMaterialComponent(0.2f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(1))));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1.0f, 1.0f, 1.0f)), 6.0f));
	material->AddMaterialComponent(0.8f, new RefractiveMaterialComponent(RefractiveMaterialComponent::INDEX_OF_REFRACTION_FLINT_GLASS, 0.00f, 1));
	scene->AddObject(new SphereObject(material, glm::vec3(2.0f, 1.25f, 8.0f), 1.25f));

	// Red Sphere
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.9f, 0.0f, 0.0f))));
	material->AddMaterialComponent(0.2f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.9f, 0.0f, 0.0f))));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1.0f, 1.0f, 1.0f)), 6.0f));
	material->AddMaterialComponent(0.8f, new RefractiveMaterialComponent(RefractiveMaterialComponent::INDEX_OF_REFRACTION_FLINT_GLASS, 0.10f, 9));
	scene->AddObject(new SphereObject(material, glm::vec3(5.5f, 0.75f, 8.0f), 0.75f));

	// Green Sphere
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.2f, 0.8f, 0.2f))));
	material->AddMaterialComponent(0.2f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.2f, 0.8f, 0.2f))));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1.0f, 1.0f, 1.0f)), 64.0f));
	scene->AddObject(new SphereObject(material, glm::vec3(5.0f, 0.75f, -2.0f), 0.75f));

	// Blue Sphere
	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.2f, 0.2f, 0.8f))));
	material->AddMaterialComponent(0.2f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.2f, 0.2f, 0.8f))));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1.0f, 1.0f, 1.0f)), 10.0f));
	scene->AddObject(new SphereObject(material, glm::vec3(5.0f, 0.75f, -8.0f), 0.75f));


	scene->AddObject(GetPlane(glm::vec3(-3.0f, 0.0f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-3.0f, 4.0f, 0.5f), 0.00f, 1, false, true));
	scene->AddObject(GetPlane(glm::vec3(-5.5f, 0.0f, 2.0f), glm::vec3(-3.5f, 0.0f, 1.0f), glm::vec3(-5.5f, 3.0f, 2.0f), 0.03f, 9, false, true));
	scene->AddObject(GetPlane(glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(5.2f, 0.0f, 3.0f), glm::vec3(3.0f, 3.5f, 3.0f), 0.02f, 9, true, false));

	scene->AddObject(GetCheckerboard(2, 2, 5, 5, 0, 0, true));

	return scene;
}

Scene* GetScene4(unsigned int width, unsigned int height)
{
	//Scene* scene = new Scene(new ObjectList());
	Scene* scene = new Scene(new ObjectHierarchy(1, 20));
	Camera* camera = new Camera(glm::vec3(4, 5, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 45.0f, width, height, 1);
	scene->SetAmbientLight(glm::vec3(0.1f,0.1f,0.1f));

	scene->SetCamera(camera);
	SolidBackground* background = new SolidBackground(glm::vec3(0.1f, 0.1f, 0.1f));
	scene->SetBackground(background);

	PointLight* light = new PointLight(glm::vec3(4, 5, 7), glm::vec3(0.9f, 0.8f, 0.7f));
	scene->AddLight(light);


	Material* material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.5f, 0.25f, 0.0f))));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.5f, 0.25f, 0.0f))));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(0.5f, 0.25f, 0.0f)), 20.0f));
	GenericObject* object = new SphereObject(material, glm::vec3(1, 1, 4), 1);
	scene->AddObject(object);

	//scene->AddObject(GetCheckerboard2(glm::vec2(10, 10), glm::vec3(), true));
	AddCheckerboard(scene, 20, 20, 5, 5, 0, 0, true);
	//scene->AddObject(GetCheckerboard(6, 6, 5, 5, 0, 0, true));

	return scene;
}

Scene* GetScene3(unsigned int width, unsigned int height)
{
	Scene* scene = new Scene(new ObjectList());
	Camera* camera = new Camera(glm::vec3(3, 5, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 45.0f, width, height, 1);
	scene->SetAmbientLight(glm::vec3(0.1f,0.1f,0.2f));

	scene->SetCamera(camera);
	SolidBackground* background = new SolidBackground(glm::vec3(0.1f, 0.1f, 0.2f));
	scene->SetBackground(background);

	//DirectionalLight* light = new DirectionalLight(glm::vec3(-1 , -1, -1), glm::vec3(1.0f, 1.0f, 1.0f));
	PointLight* light = new PointLight(glm::vec3(-3, 6, 5), glm::vec3(0.9f, 0.8f, 0.5f));
	scene->AddLight(light);

	Material* material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(1, 0, 0))));
	material->AddMaterialComponent(0.7f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(1, 0, 0))));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1, 1, 1)), 64.0f));
	material->AddMaterialComponent(0.3f, new ReflectiveMaterialComponent(0.0f, 1));
	//material->AddMaterialComponent(0.90f, new RefractiveMaterialComponent(RefractiveMaterialComponent::INDEX_OF_REFRACTION_FLINT_GLASS));
	SphereObject* sphere = new SphereObject(material, glm::vec3(0, 2, 0), 2);
	scene->AddObject(sphere);

	scene->AddObject(GetCheckerboard(5, 5, 6, 6, 0, 0, true));

	return scene;
}

Scene* GetScene2(unsigned int width, unsigned int height)
{
	Scene* scene = new Scene(new ObjectList());
	Camera* camera = new Camera(glm::vec3(0, 6, 20), glm::vec3(0, -4, 0), glm::vec3(0, 1, 0), 45.0f, width, height, 1);
	scene->SetAmbientLight(glm::vec3(0.2f, 0.2f, 0.2f));

	scene->SetCamera(camera);
	SolidBackground* background = new SolidBackground(glm::vec3(0.1f, 0.1f, 0.2f));
	scene->SetBackground(background);

	Material* material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.5f, 0.25f, 0.0f))));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.5f, 0.25f, 0.0f))));
	material->AddMaterialComponent(1.0f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(0.5f, 0.25f, 0.0f)), 20.0f));
	GenericObject* object = new SphereObject(material, glm::vec3(-3, 0, -3), 3);
	scene->AddObject(object);

	material = new Material();
	material->AddMaterialComponent(1.0f, new AmbientMaterialComponent(new SolidTexture(glm::vec3(0.25f, 0.25f, 0.25f))));
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.25f, 0.25f, 0.25f))));
	material->AddMaterialComponent(0.5f, new SpecularMaterialComponent(new SolidTexture(glm::vec3(1.0f, 1.0f, 1.0f)), 6.0f));
	object = new SphereObject(material, glm::vec3(2, -5, -1), 2);
	scene->AddObject(object);


	DirectionalLight* light = new DirectionalLight(glm::vec3(1 , -2, -1), glm::vec3(0.8f, 0.8f, 0.8f));
	scene->AddLight(light);
	light = new DirectionalLight(glm::vec3(-4 , -3, -1), glm::vec3(0.2f, 0.2f, 0.2f));
	scene->AddLight(light);

	return scene;
}

Scene* GetScene1(unsigned int width, unsigned int height)
{
	//Scene* scene = new Scene(new ObjectList());
	Scene* scene = new Scene(new ObjectHierarchy(1, 3));
	Camera* camera = new Camera(glm::vec3(4.5f, -20, 5), glm::vec3(4.5f, 0, 4), glm::vec3(0, 1, 0), 45.0f, width, height, 1);
	scene->SetAmbientLight(glm::vec3(0.2f, 0.2f, 0.2f));

	scene->SetCamera(camera);
	SolidBackground* background = new SolidBackground(glm::vec3(0.5f, 0.6f, 0.8f));
	scene->SetBackground(background);

	Material* material = new Material();
	material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(new SolidTexture(glm::vec3(0.5f, 0.25f, 0.0f))));
	scene->AddObject(new SphereObject(material, glm::vec3(0.5f, 0.0f, 0.5f), 0.5f));
	scene->AddObject(new SphereObject(material, glm::vec3(1.0f, 0.0f, 3.0f), 0.5f));
	scene->AddObject(new SphereObject(material, glm::vec3(1.5f, 0.0f, 7.0f), 0.5f));
	scene->AddObject(new SphereObject(material, glm::vec3(3.0f, 0.0f, 5.0f), 0.5f));
	scene->AddObject(new SphereObject(material, glm::vec3(4.5f, 0.0f, 1.0f), 0.5f));
	scene->AddObject(new SphereObject(material, glm::vec3(8.0f, 0.0f, 7.0f), 1.0f));
	scene->AddObject(new SphereObject(material, glm::vec3(7.0f, 0.0f, 4.0f), 1.0f));
	//material->AddMaterialComponent(1.0f, new DiffuseMaterialComponent(new ImageTexture("painting.jpg")));
	//scene->AddObject(new TriangleObject(material, glm::vec3(0, 0, 0), glm::vec3(10, 0, 0), glm::vec3(0, 8, 0), true, glm::vec2(0,1), glm::vec2(0, 0), glm::vec2(1, 1)));



	DirectionalLight* light = new DirectionalLight(glm::vec3(0 , 1, -1), glm::vec3(0.8f, 0.8f, 0.8f));
	scene->AddLight(light);

	return scene;
}

int main()
{
	unsigned int width = 640;
	unsigned int height = 480;

	RandomGenerator::Init();

	RayTracer* rayTracer = RayTracer::GetInstance();
	rayTracer->SetMaxRayTreeDepth(3);
	rayTracer->SetMaxShadowRayCnt(20);

	//Scene* scene = GetScene1(width, height);
	//scene->Finalize();
	//float a;
	//glm::vec3 b;
	//glm::vec3 c;
	//glm::vec2 d;
	//glm::vec2 pick(7.0f, 4.0f);
	//scene->GetObjectRayIntersection(Ray(glm::vec3(pick.x, 5, pick.y), glm::vec3(0, -1, 0)), a, b, c, d, true);

	Image* image = rayTracer->Render(GetGalleryScene(width, height));

	PpmImageFileWriter writer;
	writer.Save("output.ppm", image);
	delete image;

	system("start C:\\Users\\Ryan\\Downloads\\OpenSeeIt_v0.47.16.4\\OpenSeeIt.exe C:\\Users\\Ryan\\Drive\\BYU\\2019_Fall\\cs_655\\lab_01\\RayTracer\\output.ppm");
}
