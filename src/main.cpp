#include "bvh.h"
#include "camera.h"
#include "logging.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "save.h"
#include "scene.h"
#include "settings.h"
#include "utils.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// TODO find a way to store generators somewhere
RandomFloatGenerator floatGenerator;
RandomVectorGenerator colorGenerator(0.0f, 1.0f);

void GenerateRandomScene(Scene& scene, uint32_t sphereCount, uint32_t materialCount)
{
    AddSphereAndMaterial(scene, Sphere{glm::vec3(0.0f, 0.0f, -1000.0f), 1000.0f},
                         Material{MaterialType::Diffuse, glm::vec3(0.8f, 0.8f, 0.0f), 0.0f});

    // Generate materials
    for (uint32_t i = 1; i < materialCount; ++i)
    {
        glm::vec3 color = colorGenerator.Generate();

        float materialFactor = floatGenerator.Generate();
        MaterialType randomMaterialType = MaterialType::Dielectric;
        if (materialFactor < 0.4f)
        {
            randomMaterialType = MaterialType::Diffuse;
        }
        else if (materialFactor > 0.6f)
        {
            randomMaterialType = MaterialType::Metal;
        }

        AddMaterial(scene, Material{randomMaterialType, color, floatGenerator.Generate()});
    }

    constexpr float width = 4.0f;
    constexpr float minRadiuswidth = 0.1f;
    constexpr float maxRadiuswidth = 0.5f;
    constexpr float maxHeight = 3.0f;

    // Generate spheres
    for (uint32_t i = 0; i < sphereCount - 1; ++i)
    {
        glm::vec3 position;
        position.x = (floatGenerator.Generate() * width * 2.0f) - width;
        position.y = (floatGenerator.Generate() * width * 2.0f) - width;
        float radius = floatGenerator.Generate() * (maxRadiuswidth - minRadiuswidth) + minRadiuswidth;
        position.z = glm::max(radius, floatGenerator.Generate() * maxHeight);

        AddSphere(scene, Sphere{position, radius}, (rand() % (materialCount - 1)) + 1);
    }
}

void TraceScene(const Scene& scene, const Settings& setting, const Camera& camera, glm::vec3* imageBuffer,
                const BVHTree& tree)
{
    int pixelsCount = setting.imageSize.x * setting.imageSize.y;
    int progress = 0;

    for (int y = 0; y < setting.imageSize.y; ++y)
    {
        for (int x = 0; x < setting.imageSize.x; ++x)
        {
            glm::vec3 acumulatedColor(0.0f, 0.0f, 0.0f);

            for (uint32_t i = 0; i < setting.samplesPerPixel; ++i)
            {
                float u = (float(x) + floatGenerator.Generate()) / setting.imageSize.x;
                float v = (float(y) + floatGenerator.Generate()) / setting.imageSize.y;

                Ray ray = GetRay(camera, u, v);

                acumulatedColor += TracePath(ray, setting.maxBounces, scene, tree);
            }

            acumulatedColor /= setting.samplesPerPixel;
            int pixelIndex = x + y * setting.imageSize.x;
            imageBuffer[pixelIndex] = acumulatedColor;

            // Outpu pregress every 10 %
            int percent = int(float(pixelIndex) / pixelsCount * 100);
            if (percent % 10 == 0 && percent != progress)
            {
                progress = percent;
                INFO("Tracing porgress %d", progress);
            }
        }
    }

    INFO("Tracing finshed");
}

int main(int /*argc*/, char** /*argv*/)
{
    Settings settings = DefaultSettings();

    glm::vec3 cameraPosition(-5.0f, 0.0f, 1.5f);
    glm::vec3 at(0.0f, 0.0f, 1.5f);
    glm::vec3 up(0.0f, 0.0f, 1.0f);

    const float aspectRatio = float(settings.imageSize.x) / settings.imageSize.y;
    const float fov = 45.0f;
    Camera camera = CameraFromView(cameraPosition, at, up, fov, aspectRatio);

    // Prepare scene
    Scene scene;
    constexpr uint32_t sphereCount = 50;
    constexpr uint32_t materialCount = 5;
    GenerateRandomScene(scene, sphereCount, materialCount);

    int bufferSize = sizeof(glm::vec3) * settings.imageSize.x * settings.imageSize.y;
    glm::vec3* imageBuffer = static_cast<glm::vec3*>(malloc(bufferSize));

    BVHTree tree = BuildBVHTree(scene.spheresGeometry);
    {
        TRACE_EXECUTION("TraceScene");
        TraceScene(scene, settings, camera, imageBuffer, tree);
    }

    saveImageBufferToFile(settings.imageSize, imageBuffer, "output.png");

    free(imageBuffer);

    return 0;
}