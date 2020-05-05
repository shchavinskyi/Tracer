#include "utils.h"

#include "random.h"
#include "scene.h"

namespace {
RandomFloatGenerator floatGenerator;
RandomVectorGenerator colorGenerator(0.0f, 1.0f);
} // namespace

void GenerateRandomScene(Scene& scene, uint32_t sphereCount, uint32_t materialCount)
{
    glm::vec3 cameraPosition(-6.0f, 0.0f, 2.5f);
    glm::vec3 at(0.0f, 0.0f, 0.5f);
    glm::vec3 up(0.0f, 0.0f, 1.0f);

    const float aspectRatio = float(scene.settings.imageSize.x) / scene.settings.imageSize.y;
    const float fov = 45.0f;
    scene.camera = CameraFromView(cameraPosition, at, up, fov, aspectRatio);

    AddSphereAndMaterial(scene, Sphere{glm::vec3(0.0f, 0.0f, 5.0f), 2.0f},
                         Material{MaterialType::Light, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f});

    size_t matId = AddMaterial(scene, Material{MaterialType::Diffuse, glm::vec3(0.8f, 0.8f, 0.0f), 0.0f});
    float rectWidth = 8.0f;
    AddTriangle(scene,
                Triangle{glm::vec3(-1.0f * rectWidth, -1.0f * rectWidth, 0.0f),
                         glm::vec3(1.0f * rectWidth, -1.0f * rectWidth, 0.0f),
                         glm::vec3(-1.0f * rectWidth, 1.0f * rectWidth, 0.0f)},
                matId);

    AddTriangle(scene,
                Triangle{glm::vec3(1.0f * rectWidth, -1.0f * rectWidth, 0.0f),
                         glm::vec3(1.0f * rectWidth, 1.0f * rectWidth, 0.0f),
                         glm::vec3(-1.0f * rectWidth, 1.0f * rectWidth, 0.0f)},
                matId);

    matId = AddMaterial(scene, Material{MaterialType::Metal, glm::vec3(0.9f, 1.0f, 1.0f), 0.01f});
    float rectWidth2 = 6.0f;
    AddTriangle(scene,
                Triangle{glm::vec3(rectWidth, -1.0f * rectWidth2, 0.0f),
                         glm::vec3(rectWidth, -1.0f * rectWidth2, 1.0f * rectWidth2),
                         glm::vec3(rectWidth, 1.0f * rectWidth2, 0.0f)},
                matId);

    AddTriangle(scene,
                Triangle{glm::vec3(rectWidth, -1.0f * rectWidth2, 1.0f * rectWidth2),
                         glm::vec3(rectWidth, 1.0f * rectWidth2, 1.0f * rectWidth2),
                         glm::vec3(rectWidth, 1.0f * rectWidth2, 0.0f)},
                matId);

    size_t fixedMaterials = scene.materials.size();

    // Generate materials
    for (uint32_t i = 0; i < materialCount - fixedMaterials; ++i)
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

    constexpr float width = 3.0f;
    constexpr float minRadiuswidth = 0.1f;
    constexpr float maxRadiuswidth = 0.5f;
    // constexpr float maxHeight = 3.0f;

    // Generate spheres
    for (uint32_t i = 0; i < sphereCount - 1; ++i)
    {
        glm::vec3 position;
        position.x = (floatGenerator.Generate() * width * 2.0f) - width;
        position.y = (floatGenerator.Generate() * width * 2.0f) - width;
        float radius = floatGenerator.Generate() * (maxRadiuswidth - minRadiuswidth) + minRadiuswidth;
        position.z = radius; // glm::max(radius, floatGenerator.Generate() * maxHeight);

        AddSphere(scene, Sphere{position, radius}, (rand() % (materialCount - fixedMaterials)) + fixedMaterials);
    }
}

void CornellBox(Scene& scene)
{
    constexpr float boxWidth = 555.0f;
    constexpr float boxHalfWidth = boxWidth / 2.0f;

    glm::vec3 cameraPosition(boxHalfWidth, -800.0f, boxHalfWidth);
    glm::vec3 at(boxHalfWidth, 0.0f, boxHalfWidth);
    glm::vec3 up(0.0f, 0.0f, 1.0f);

    const float aspectRatio = float(scene.settings.imageSize.x) / scene.settings.imageSize.y;
    const float fov = 40.0f;
    scene.camera = CameraFromView(cameraPosition, at, up, fov, aspectRatio);

    size_t red = AddMaterial(scene, Material{MaterialType::Diffuse, glm::vec3(0.65f, 0.05f, 0.05f), 0});
    size_t white = AddMaterial(scene, Material{MaterialType::Diffuse, glm::vec3(0.73f, 0.73f, 0.73f), 0});
    size_t green = AddMaterial(scene, Material{MaterialType::Diffuse, glm::vec3(0.12f, 0.45f, 0.15f), 0});
    size_t light = AddMaterial(scene, Material{MaterialType::Light, glm::vec3(15.0f, 15.0f, 15.0f), 0});

    // bottom
    AddXYRect(scene, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(boxWidth, boxWidth, 0.0f), white);
    // top
    AddXYRect(scene, glm::vec3(0.0f, 0.0f, boxWidth), glm::vec3(boxWidth, boxWidth, boxWidth), white, true);
    // left
    AddYZRect(scene, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, boxWidth, boxWidth), green);
    // right
    AddYZRect(scene, glm::vec3(boxWidth, 0.0f, 0.0f), glm::vec3(boxWidth, boxWidth, boxWidth), red, true);
    // back
    AddXZRect(scene, glm::vec3(0.0f, boxWidth, 0.0f), glm::vec3(boxWidth, boxWidth, boxWidth), white);

    // Add light
    constexpr float lightWidth = 200.0f;
    constexpr float lowCorner = (boxWidth - lightWidth) / 2.0f;
    constexpr float HighCorner = (boxWidth + lightWidth) / 2.0f;
    AddXYRect(scene, glm::vec3(lowCorner, lowCorner, boxWidth - 0.1f),
              glm::vec3(HighCorner, HighCorner, boxWidth - 0.1f), light, true);

    constexpr float radius = 80.0f;
    AddSphereAndMaterial(scene, Sphere{glm::vec3(boxHalfWidth - 10.0f, boxHalfWidth + 100.0f, radius), radius},
                         Material{MaterialType::Metal, glm::vec3(0.73f, 0.73f, 0.73f), 0.0f});

    AddSphereAndMaterial(scene, Sphere{glm::vec3(boxHalfWidth + 120.0f, boxHalfWidth - 40.0f, radius), radius},
                         Material{MaterialType::Diffuse, glm::vec3(0.73f, 0.73f, 0.73f), 0.0f});

    AddSphereAndMaterial(scene, Sphere{glm::vec3(radius + 100.0f, boxHalfWidth - 120.0f, radius), radius},
                         Material{MaterialType::Dielectric, glm::vec3(0.73f, 0.73f, 0.73f), 0.97f});
}