#include <iostream>
#include <string>

#include <glm/vec2.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


#include "hello.h"

struct Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

const uint8_t MAX_COLOR = 255;
const int imageComp = 3; // RGB

int main(int /*argc*/, char** /*argv*/)
{
	glm::ivec2 imageSize{640, 480};

	Color* imageBuffer = static_cast<Color*>(malloc(imageComp * imageSize.x * imageSize.y));
	
	for (int y = 0 ; y < imageSize.y; ++y)
	for (int x = 0 ; x < imageSize.x; ++x)
	{
		Color& currentPixel = imageBuffer[x + y * imageSize.x];
		currentPixel.r = static_cast<uint8_t>(MAX_COLOR * x / imageSize.x);
		currentPixel.g = static_cast<uint8_t>(MAX_COLOR * y / imageSize.y);
		currentPixel.b = static_cast<uint8_t>(MAX_COLOR / 5);
	}		

	const std::string filename("output.png");
	int result = stbi_write_png(filename.c_str(), imageSize.x, imageSize.y, imageComp, imageBuffer, imageSize.x * imageComp);
	std::cout << " Wrote image to " << filename << " with result " << result << '\n';

	free(imageBuffer);

	const int n = 6;
	std::cout << n << ' ' << number(n) << '\n';

	return 0;
}