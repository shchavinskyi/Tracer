#include "logging.h"
#include "render.h"
#include "save.h"
#include "scene.h"
#include "utils.h"

int main(int /*argc*/, char** /*argv*/)
{
    Scene scene;

    GenerateCornellBox(scene);

    RenderBuffer imageBuffer = CreateImageBuffer(scene.settings.imageSize);
    {
        TRACE_EXECUTION("TraceScene");

        RenderSceneMT(scene, imageBuffer);
    }

    SaveImageBufferToFile(imageBuffer, scene.settings.imageSize, "output.png");

    ReleaseBuffer(imageBuffer);

    return 0;
}
