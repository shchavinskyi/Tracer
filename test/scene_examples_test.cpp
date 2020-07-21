#include "logging.h"
#include "render.h"
#include "save.h"
#include "scene.h"
#include "test.h"
#include "utils.h"

#include <array>
#include <functional>
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
        caseName.append(SettingToString(settings));

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
    RenderTest(testSettings, "cornell_box", [](Scene& scene) { GenerateCornellBox(scene); });
}

TEST_CASE("scene_random")
{
    RenderTest(testSettings, "random", [](Scene& scene) {
        constexpr uint32_t sphereCount = 30;
        constexpr uint32_t materialCount = 7;
        GenerateRandomScene(scene, sphereCount, materialCount);
    });
}

TEST_CASE("scene_mesh_suzanne")
{
    RenderTest(testSettings, "suzanne", [](Scene& scene) {
        constexpr float boxWidth = 555.0f;
        GenerateBox(scene, boxWidth);
        const uint32_t mat = AddMaterial(scene, Material::CreateDiffuse(glm::vec3(0.95f, 0.15f, 0.05f)));
        LoadMeshToScene(scene, 180.0f, glm::vec3{boxWidth * 0.5f, boxWidth * 0.7f, boxWidth * 0.5f}, mat,
                        "data/suzanne.obj");
    });
}

TEST_CASE("scene_mesh_simple")
{
    RenderTest(testSettings, "mesh_simple", [](Scene& scene) {
        constexpr float boxWidth = 555.0f;
        GenerateBox(scene, boxWidth);
        const uint32_t mat1 = AddMaterial(scene, Material::CreateDiffuse(glm::vec3(0.5f, 0.59f, 0.8f)));
        LoadMeshToScene(scene, 100.0f, glm::vec3{boxWidth * 0.25f, boxWidth * 0.6f, boxWidth * 0.3f}, mat1,
                        "data/cube.obj");
        const uint32_t mat2 = AddMaterial(scene, Material::CreateMetal(glm::vec3(0.93f, 0.93f, 0.93f), 0.0f));
        LoadMeshToScene(scene, 120.0f, glm::vec3{boxWidth * 0.7f, boxWidth * 0.4f, boxWidth * 0.6f}, mat2,
                        "data/ico_sphere.obj");
    });
}
