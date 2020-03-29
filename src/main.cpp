#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/intersect.hpp>

#include "scene.h"
#include "save.h"

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
};

struct HitResult
{
	glm::vec3 position;
   	glm::vec3 normal;
};

glm::vec3 backgroundColor(const Ray& ray)
{
    float t = 0.5f * (ray.direction.y + 1.0f);
    const glm::vec3 top{0.5f, 0.7f, 1.0f};
    const glm::vec3 bottom(1.0f, 1.0f, 1.0f);
    return (1.0f - t) * top + t * bottom;
}

int main(int /*argc*/, char** /*argv*/)
{
	glm::ivec2 imageSize{800, 400};

	glm::vec3* imageBuffer = static_cast<glm::vec3*>(malloc(sizeof(glm::vec3) * imageSize.x * imageSize.y));
	
	for (int y = 0 ; y < imageSize.y; ++y)
	for (int x = 0 ; x < imageSize.x; ++x)
	{
		glm::vec3& currentPixel = imageBuffer[x + y * imageSize.x];
		currentPixel.r = 1.0f * x / imageSize.x;
		currentPixel.g = 1.0f * y / imageSize.y;
		currentPixel.b = 1.0f / 5.0f;
	}		

	glm::vec3 lowerLeft(-2.0f, -1.0f, -1.0f);
    glm::vec3 horizontal(4.0f, 0.0f, 0.0f);
    glm::vec3 vertical(0.0f, 2.0f, 0.0f);
   	glm::vec3 origin(0.0f, 0.0f, 0.0f);

	Scene scene;
	
   	scene.spheres.emplace_back(Sphere{glm::vec3(0.0f, 0.0f, -1.0f), 0.5f});
   	scene.spheres.emplace_back(Sphere{glm::vec3(0.0f, 100.5f, -1.0f), 100.0f});

	for (int y = 0 ; y < imageSize.y; ++y)
	for (int x = 0 ; x < imageSize.x; ++x)
	{
		glm::vec3& currentPixel = imageBuffer[x + y * imageSize.x];

		float u = float(x) / imageSize.x;
        float v = float(y) / imageSize.y;	

		Ray ray{origin, glm::normalize(lowerLeft + u * horizontal + v * vertical)};

		bool isHit = false;
        glm::length_t distanceToClosest = std::numeric_limits<glm::length_t>::max();

		for (const Sphere& sphere : scene.spheres)
		{
			glm::vec3 intersectionPosition;
			glm::vec3 intersectionNormal;

			if (glm::intersectRaySphere(ray.origin, ray.direction, sphere.center, sphere.radius, intersectionPosition, intersectionNormal))
			{
                const glm::length_t lenghtToIntersection = glm::vec3(intersectionPosition - origin).length();
                if(lenghtToIntersection < distanceToClosest)
                {
                    distanceToClosest = lenghtToIntersection;
                	glm::vec3 vectorColor = 0.5f * (intersectionNormal + glm::vec3(1.0f, 1.0f, 1.0f));
        			currentPixel = vectorColor;
         			isHit = true;
                }
			}
		}

		if (!isHit)
		{
			currentPixel = backgroundColor(ray);
		}
	}

	saveImageBufferToFile(imageSize, imageBuffer, "output.png");

	free(imageBuffer);

	return 0;
}