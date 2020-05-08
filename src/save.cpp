#include "save.h"

#include "logging.h"
#include "settings.h"

#include <glm/vec3.hpp>
#include <string>

#ifdef __clang__

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#pragma clang diagnostic ignored "-Wcast-qual"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wextra-semi-stmt"
#pragma clang diagnostic ignored "-Wimplicit-int-conversion"

#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#ifdef __clang__

#pragma clang diagnostic pop

#endif

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

void SaveImageBufferToFile(glm::vec3* renderBuffer, const ImageSize& size, const std::string& filename)
{
    Color* outputBuffer = static_cast<Color*>(malloc(sizeof(Color) * size.width * size.height));

    for (uint32_t y = 0; y < size.height; ++y)
        for (uint32_t x = 0; x < size.width; ++x)
        {
            const uint32_t index = x + y * size.width;
            glm::vec3& color = renderBuffer[index];
            Color& outputTexel = outputBuffer[index];

            outputTexel.r = static_cast<uint8_t>(255.999f * color.r);
            outputTexel.g = static_cast<uint8_t>(255.999f * color.g);
            outputTexel.b = static_cast<uint8_t>(255.999f * color.b);
        }

    constexpr int numberOfChenels = 3; /* rgb */
    int result = stbi_write_png(filename.c_str(), int(size.width), int(size.height), numberOfChenels, outputBuffer,
                                int(size.width * numberOfChenels));

    INFO("Image successfully save to file : %s with result %d", filename.c_str(), result);

    free(outputBuffer);
}
