#include "logging.h"
#include "render.h"
#include "save.h"
#include "scene.h"
#include "utils.h"

#include <thread>

int main(int /*argc*/, char** /*argv*/)
{
    Scene scene;
    scene.settings = DefaultSettings();

    // Generate Cornell Box
    CornellBox(scene);

    RenderBuffer imageBuffer = CreateImageBuffer(scene.settings.imageSize);

    {
        TRACE_EXECUTION("TraceScene");

        unsigned int threadCount = std::thread::hardware_concurrency();
        LOG_INFO("[ %d ] hardware concurrent threads are supported.", threadCount);

        RenderSceneMT(scene, imageBuffer, threadCount);
    }

    SaveImageBufferToFile(imageBuffer, scene.settings.imageSize, "output.png");

    ReleaseBuffer(imageBuffer);

    return 0;
}
