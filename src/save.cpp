#include "save.h"

#include "logging.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

void SaveImageBufferToFile(glm::vec3* renderBuffer, const glm::ivec2& size, const std::string& filename)
{
    Color* outputBuffer = static_cast<Color*>(malloc(sizeof(Color) * size.x * size.y));

    for (int y = 0; y < size.y; ++y)
        for (int x = 0; x < size.x; ++x)
        {
            const int index = x + y * size.x;
            glm::vec3& color = renderBuffer[index];
            Color& outputTexel = outputBuffer[index];

            outputTexel.r = static_cast<uint8_t>(255.999f * color.r);
            outputTexel.g = static_cast<uint8_t>(255.999f * color.g);
            outputTexel.b = static_cast<uint8_t>(255.999f * color.b);
        }

    constexpr int numberOfChenels = 3; /* rgb */
    int result =
        stbi_write_png(filename.c_str(), size.x, size.y, numberOfChenels, outputBuffer, size.x * numberOfChenels);

    INFO("Image successfully save to file : %s ", filename.c_str());

    free(outputBuffer);
}