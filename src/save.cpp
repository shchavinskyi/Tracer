#include "save.h"

#include "data.h"
#include "logging.h"
#include "scene.h"

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

void SaveImageBufferToFile(const RenderBuffer& renderBuffer, const ImageSize& size, std::string_view filename)
{
    constexpr int numberOfChenels = 3; /* rgb */
    int result = stbi_write_png(filename.data(), int(size.width), int(size.height), numberOfChenels,
                                renderBuffer.buffer, int(size.width * numberOfChenels));

    LOG_INFO("Image successfully save to file : %s with result %d", filename.data(), result);
}
