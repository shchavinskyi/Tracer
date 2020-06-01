#include "logging.h"
#include "render.h"
#include "save.h"
#include "scene.h"
#include "test.h"
#include "utils.h"

#include <array>
#include <thread>

namespace {
template <std::size_t N>
void RenderTest(const std::array<Settings, N>& testSettings, std::string_view testName,
                const std::function<void(Scene&)>& fillSceneFunction)
{
    Scene scene;

    // Call fill function
    fillSceneFunction(scene);

    std::for_each(begin(testSettings), end(testSettings), [&](const Settings& settings) {
        std::string caseName(testName);
        caseName.append("_");
        caseName.append(settingToString(settings));

        TRACE_EXECUTION(caseName.data());
        LOG_INFO("Rendering test [%s]", caseName.data());

        // Assign settings
        scene.settings = settings;

        RenderBuffer imageBuffer = CreateImageBuffer(settings.imageSize);

        RenderSceneMT(scene, imageBuffer);

        SaveImageBufferToFile(imageBuffer, scene.settings.imageSize, std::string("output/" + caseName + ".png"));

        ReleaseBuffer(imageBuffer);
    });
}

constexpr std::array<Settings, 4> testSettings = {Settings{{400, 400}, 200, 5}, Settings{{400, 400}, 500, 10},
                                                  Settings{{400, 400}, 2000, 10}, Settings{{400, 400}, 10000, 10}};
} // namespace

TEST_CASE("scene_cornell_box")
{
    RenderTest(testSettings, "cornell_box", [](Scene& scene) { CornellBox(scene); });
}

TEST_CASE("scene_random")
{
    RenderTest(testSettings, "random", [](Scene& scene) {
        constexpr uint32_t sphereCount = 30;
        constexpr uint32_t materialCount = 7;
        GenerateRandomScene(scene, sphereCount, materialCount);
    });
}
