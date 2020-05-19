#include "utils.h"

#include "random.h"

namespace {
float AspectRatio(const ImageSize& size) { return float(size.width) / float(size.height); }
} // namespace

Camera CameraFromView(const glm::vec3& lookFrom, const glm::vec3& lookAt, const glm::vec3& up,
                      float vfov, // top to bottom, in degrees
                      float aspect)
{
    float radians = glm::radians(vfov);
    constexpr float half = 0.5f;
    float halfHeight = glm::tan(radians * half);
    float halfWidth = aspect * halfHeight;
    glm::vec3 w = glm::normalize(lookFrom - lookAt);
    glm::vec3 u = glm::normalize(glm::cross(up, w));
    glm::vec3 v = glm::cross(u, w);

    constexpr float two = 2.0f;
    return Camera{lookFrom, lookFrom - halfWidth * u - halfHeight * v - w, two * halfWidth * u, two * halfHeight * v};
}

void GenerateRandomScene(Scene& scene, uint32_t sphereCount, uint32_t materialCount)
{
    RandomFloatGenerator floatGenerator;
    RandomVectorGenerator colorGenerator(0.0f, 1.0f);

    glm::vec3 cameraPosition(-6.0f, 0.0f, 2.5f);
    glm::vec3 at(0.0f, 0.0f, 0.5f);
    glm::vec3 up(0.0f, 0.0f, 1.0f);

    const float aspectRatio = AspectRatio(scene.settings.imageSize);
    const float fov = 45.0f;
    scene.camera = CameraFromView(cameraPosition, at, up, fov, aspectRatio);

    scene.backgroundColor = glm::vec3(0.5f, 0.7f, 1.0f);

    AddSphereAndMaterial(scene, Sphere{glm::vec3(0.0f, 0.0f, 5.0f), 2.0f},
                         Material::CreateLight(glm::vec3(1.0f, 1.0f, 1.0f)));

    uint32_t matId = AddMaterial(scene, Material::CreateDiffuse(glm::vec3(0.8f, 0.8f, 0.0f)));
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

    matId = AddMaterial(scene, Material::CreateMetal(glm::vec3(0.9f, 1.0f, 1.0f), 0.01f));
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

    uint32_t fixedMaterials = static_cast<uint32_t>(scene.materials.size());

    // Generate materials
    for (uint32_t i = 0; i < materialCount - fixedMaterials; ++i)
    {
        glm::vec3 randomColor = colorGenerator.Generate();

        float materialFactor = floatGenerator.Generate();

        Material randomMaterial;
        if (materialFactor < 0.4f)
        {
            randomMaterial = Material::CreateDiffuse(randomColor);
        }
        else if (materialFactor > 0.6f)
        {
            randomMaterial = Material::CreateMetal(randomColor, floatGenerator.Generate());
        }
        else
        {
            randomMaterial = Material::CreateDielectric(randomColor, floatGenerator.Generate());
        }

        AddMaterial(scene, randomMaterial);
    }

    constexpr float width = 3.0f;
    constexpr float minRadiuswidth = 0.1f;
    constexpr float maxRadiuswidth = 0.5f;

    uint32_t randomMaterailsCount = static_cast<uint32_t>(scene.materials.size()) - fixedMaterials;
    assert(randomMaterailsCount > 0);

    // Generate spheres
    for (uint32_t i = 0; i < sphereCount - 1; ++i)
    {
        glm::vec3 position;
        position.x = (floatGenerator.Generate() * width * 2.0f) - width;
        position.y = (floatGenerator.Generate() * width * 2.0f) - width;
        float radius = floatGenerator.Generate() * (maxRadiuswidth - minRadiuswidth) + minRadiuswidth;
        position.z = radius;

        uint32_t randomMatId = uint32_t(rand() % int(randomMaterailsCount)) + fixedMaterials;

        AddSphere(scene, Sphere{position, radius}, randomMatId);
    }
}

void CornellBox(Scene& scene)
{
    constexpr float boxWidth = 555.0f;
    constexpr float boxHalfWidth = boxWidth / 2.0f;

    glm::vec3 cameraPosition(boxHalfWidth, -800.0f, boxHalfWidth);
    glm::vec3 at(boxHalfWidth, 0.0f, boxHalfWidth);
    glm::vec3 up(0.0f, 0.0f, 1.0f);

    const float aspectRatio = AspectRatio(scene.settings.imageSize);
    const float fov = 40.0f;
    scene.camera = CameraFromView(cameraPosition, at, up, fov, aspectRatio);

    scene.backgroundColor = glm::vec3(0.0f, 0.0f, 0.0f);

    uint32_t red = AddMaterial(scene, Material::CreateDiffuse(glm::vec3(0.65f, 0.05f, 0.05f)));
    uint32_t white = AddMaterial(scene, Material::CreateDiffuse(glm::vec3(0.73f, 0.73f, 0.73f)));
    uint32_t green = AddMaterial(scene, Material::CreateDiffuse(glm::vec3(0.12f, 0.45f, 0.15f)));
    uint32_t light = AddMaterial(scene, Material::CreateLight(glm::vec3(15.0f, 15.0f, 15.0f)));

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
                         Material::CreateMetal(glm::vec3(0.73f, 0.73f, 0.73f), 0.0f));

    AddSphereAndMaterial(scene, Sphere{glm::vec3(boxHalfWidth + 120.0f, boxHalfWidth - 40.0f, radius), radius},
                         Material::CreateDiffuse(glm::vec3(0.73f, 0.73f, 0.73f)));

    AddSphereAndMaterial(scene, Sphere{glm::vec3(radius + 100.0f, boxHalfWidth - 120.0f, radius), radius},
                         Material::CreateDielectric(glm::vec3(0.73f, 0.73f, 0.73f), 0.97f));
}
