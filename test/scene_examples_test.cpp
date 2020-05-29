#include "render.h"
#include "save.h"
#include "scene.h"
#include "test.h"
#include "utils.h"

#include <thread>

TEST_CASE("scene_cornell_box")
{
    Scene scene;

    CornellBox(scene);

    RenderBuffer imageBuffer = CreateImageBuffer(scene.settings.imageSize);

    RenderSceneMT(scene, imageBuffer);

    SaveImageBufferToFile(imageBuffer, scene.settings.imageSize, "output_cornell_box.png");

    ReleaseBuffer(imageBuffer);
}

TEST_CASE("scene_random")
{
    Scene scene;

    constexpr uint32_t sphereCount = 20;
    constexpr uint32_t materialCount = 8;
    GenerateRandomScene(scene, sphereCount, materialCount);

    RenderBuffer imageBuffer = CreateImageBuffer(scene.settings.imageSize);

    RenderSceneMT(scene, imageBuffer);

    SaveImageBufferToFile(imageBuffer, scene.settings.imageSize, "output_random.png");

    ReleaseBuffer(imageBuffer);
}
