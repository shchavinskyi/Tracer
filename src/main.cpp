#include "logging.h"
#include "save.h"
#include "scene.h"
#include "utils.h"

#include <glm/vec3.hpp>

int main(int /*argc*/, char** /*argv*/)
{
    // Prepare scene
    Scene scene;
    scene.settings = DefaultSettings();

    /*constexpr uint32_t sphereCount = 20;
    constexpr uint32_t materialCount = 10;
    GenerateRandomScene(scene, camera, settings, sphereCount, materialCount);*/

    CornellBox(scene);

    int bufferSize = sizeof(glm::vec3) * scene.settings.imageSize.x * scene.settings.imageSize.y;
    glm::vec3* imageBuffer = static_cast<glm::vec3*>(malloc(bufferSize));

    {
        TRACE_EXECUTION("TraceScene");
        TraceScene(scene, imageBuffer);
    }

    saveImageBufferToFile(scene.settings.imageSize, imageBuffer, "output.png");

    free(imageBuffer);

    return 0;
}