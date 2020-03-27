#include <iostream>
#include <string>

#include <limits>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/intersect.hpp>


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


#include "hello.h"

struct Color
{
	float r;
	float g;
	float b;
};

struct Texel
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	static constexpr uint8_t MAX_VALUE = std::numeric_limits<uint8_t>::max();
};

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
};



const int imageComp = 3; // RGB

int main(int /*argc*/, char** /*argv*/)
{
	glm::ivec2 imageSize{800, 400};



	Color* imageBuffer = static_cast<Color*>(malloc(sizeof(Color) * imageSize.x * imageSize.y));
	
	for (int y = 0 ; y < imageSize.y; ++y)
	for (int x = 0 ; x < imageSize.x; ++x)
	{
		Color& currentPixel = imageBuffer[x + y * imageSize.x];
		currentPixel.r = 1.0f * x / imageSize.x;
		currentPixel.g = 1.0f * y / imageSize.y;
		currentPixel.b = 1.0f / 5.0f;
	}		

	glm::vec3 lowerLeft(-2.0, -1.0, -1.0);
    glm::vec3 horizontal(4.0, 0.0, 0.0);
    glm::vec3 vertical(0.0, 2.0, 0.0);
   	glm::vec3 origin(0.0, 0.0, 0.0);

   	glm::vec3 sphereCenter(0,0,-1);
   	float sphereRadius = 0.5;

	for (int y = 0 ; y < imageSize.y; ++y)
	for (int x = 0 ; x < imageSize.x; ++x)
	{
		float u = float(x) / imageSize.x;
        float v = float(y) / imageSize.y;

		Ray ray{origin, glm::normalize(lowerLeft + u * horizontal + v * vertical)};

		glm::vec3 intersectionPosition;
   		glm::vec3 intersectionNormal;
		if (glm::intersectRaySphere(ray.origin, ray.direction, sphereCenter, sphereRadius, intersectionPosition, intersectionNormal))
		{
			Color& currentPixel = imageBuffer[x + y * imageSize.x];
			currentPixel.r = 1.0f;
		}
	}

	// Covert to image format buffer

	Texel* buffer = static_cast<Texel*>(malloc(sizeof(Texel) * imageSize.x * imageSize.y));

	for (int y = 0 ; y < imageSize.y; ++y)
	for (int x = 0 ; x < imageSize.x; ++x)
	{
		int index = x + y * imageSize.x;
		Color& currentColor = imageBuffer[index];
		Texel& currentTexel = buffer[index];

		currentTexel.r = static_cast<uint8_t>(255.999f * currentColor.r);
		currentTexel.g = static_cast<uint8_t>(255.999f * currentColor.g);
		currentTexel.b = static_cast<uint8_t>(255.999f * currentColor.b);
	}	


	const std::string filename("output.png");
	int result = stbi_write_png(filename.c_str(), imageSize.x, imageSize.y, imageComp, buffer, imageSize.x * imageComp);
	std::cout << " Wrote image to " << filename << " with result " << result << '\n';

	free(buffer);

	free(imageBuffer);

	const int n = 6;
	std::cout << n << ' ' << number(n) << '\n';

	return 0;
}