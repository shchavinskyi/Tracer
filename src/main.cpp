#include "logging.h"
#include "save.h"
#include "scene.h"
#include "utils.h"

#include <glm/vec3.hpp>
#include <thread>

int main(int /*argc*/, char** /*argv*/)
{
    // Prepare scene
    Scene scene;
    scene.settings = DefaultSettings();

    CornellBox(scene);

    RenderBuffer imageBuffer;
    imageBuffer.start = 0;
    imageBuffer.length = scene.settings.imageSize.width * scene.settings.imageSize.height;
    imageBuffer.buffer = static_cast<glm::vec3*>(malloc(sizeof(glm::vec3) * imageBuffer.length));

    {
        TRACE_EXECUTION("TraceScene");

        unsigned int threadCount = std::thread::hardware_concurrency();
        INFO("[ %d ] hardware concurrent threads are supported.", threadCount);

        RenderSceneMT(scene, imageBuffer, threadCount);
    }

    SaveImageBufferToFile(imageBuffer.buffer, scene.settings.imageSize, "output.png");

    free(imageBuffer.buffer);

    return 0;
}
