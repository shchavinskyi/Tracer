#include "utils.h"

#include "logging.h"
#include "random.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <sstream>

namespace {
float AspectRatio(const ImageSize& size)
{
    return float(size.width) / float(size.height);
}

bool CanPlaceSphere(const Scene& scene, const Sphere& sphere)
{
    for (const Sphere& existSphere : scene.spheresGeometry)
    {
        float distance = glm::distance(existSphere.center, sphere.center);
        if (distance < existSphere.radius + sphere.radius)
        {
            return false;
        }
    }

    return true;
}
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

RenderBuffer CreateImageBuffer(const ImageSize& size)
{
    const uint32_t pixelCount = size.width * size.height;
    return RenderBuffer{static_cast<Color*>(malloc(sizeof(Color) * pixelCount)), 0, pixelCount};
}

void ReleaseBuffer(RenderBuffer& buffer)
{
    free(buffer.buffer);
}

void GenerateRandomScene(Scene& scene, uint32_t sphereCount, uint32_t materialCount)
{
    assert(materialCount > 0);

    RandomFloatGenerator floatGenerator;
    RandomVectorGenerator colorGenerator(0.0f, 1.0f);

    glm::vec3 cameraPosition(-6.0f, 0.0f, 2.5f);
    glm::vec3 at(0.0f, 0.0f, 0.5f);
    glm::vec3 up(0.0f, 0.0f, 1.0f);

    const float aspectRatio = AspectRatio(scene.settings.imageSize);
    const float fov = 45.0f;
    scene.camera = CameraFromView(cameraPosition, at, up, fov, aspectRatio);

    scene.backgroundColor = glm::vec3(0.25f, 0.25f, 0.5f);

    AddSphereAndMaterial(scene, Sphere{glm::vec3(1.0f, 0.0f, 7.5f), 3.0f},
                         Material::CreateLight(glm::vec3(4.0f, 4.0f, 4.0f)));

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

    const uint32_t fixedMaterials = static_cast<uint32_t>(scene.materials.size());

    // Generate materials
    for (uint32_t i = 0; i < materialCount; ++i)
    {
        const glm::vec3 randomColor = colorGenerator.Generate();

        const float materialFactor = floatGenerator.Generate();

        Material randomMaterial;
        if (materialFactor < 0.45f)
        {
            randomMaterial = Material::CreateDiffuse(randomColor);
        }
        else if (materialFactor > 0.55f)
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
    constexpr float minRadiuswidth = 0.2f;
    constexpr float maxRadiuswidth = 0.65f;

    // Generate spheres
    for (uint32_t i = 0; i < sphereCount;)
    {
        glm::vec3 position;
        position.x = (floatGenerator.Generate() * width * 2.0f) - width;
        position.y = (floatGenerator.Generate() * width * 2.0f) - width;
        float radius = floatGenerator.Generate() * (maxRadiuswidth - minRadiuswidth) + minRadiuswidth;
        position.z = radius;

        Sphere sphere{position, radius};
        if (CanPlaceSphere(scene, sphere))
        {
            const uint32_t randomMatId = uint32_t(rand() % int(materialCount)) + fixedMaterials;
            AddSphere(scene, sphere, randomMatId);
            ++i;
        }
    }
}

void GenerateBox(Scene& scene, float boxWidth)
{
    const float boxHalfWidth = boxWidth / 2.0f;

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
    uint32_t light = AddMaterial(scene, Material::CreateLight(glm::vec3(12.0f, 12.0f, 12.0f)));

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
    constexpr float lightWidth = 180.0f;
    constexpr float offset = -100.0f;
    const float lowCorner = (boxWidth - lightWidth) / 2.0f;
    const float HighCorner = (boxWidth + lightWidth) / 2.0f;
    AddXYRect(scene, glm::vec3(lowCorner, lowCorner + offset, boxWidth - 2.0f),
              glm::vec3(HighCorner, HighCorner + offset, boxWidth - 2.0f), light, true);
}

void GenerateCornellBox(Scene& scene)
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
    constexpr float lightWidth = 180.0f;
    constexpr float lowCorner = (boxWidth - lightWidth) / 2.0f;
    constexpr float HighCorner = (boxWidth + lightWidth) / 2.0f;
    AddXYRect(scene, glm::vec3(lowCorner, lowCorner, boxWidth - 2.0f),
              glm::vec3(HighCorner, HighCorner, boxWidth - 2.0f), light, true);

    constexpr float radius = 80.0f;
    AddSphereAndMaterial(scene, Sphere{glm::vec3(boxHalfWidth + 5.0f, boxHalfWidth + 100.0f, radius), radius},
                         Material::CreateMetal(glm::vec3(0.73f, 0.73f, 0.73f), 0.0f));

    AddSphereAndMaterial(scene, Sphere{glm::vec3(boxHalfWidth + 120.0f, boxHalfWidth - 40.0f, radius), radius},
                         Material::CreateDiffuse(glm::vec3(0.73f, 0.73f, 0.73f)));

    AddSphereAndMaterial(scene, Sphere{glm::vec3(radius + 100.0f, boxHalfWidth - 120.0f, radius), radius},
                         Material::CreateDielectric(glm::vec3(0.73f, 0.73f, 0.73f), 0.97f));
}

std::string SettingToString(const Settings& setting)
{
    std::stringstream ss;
    ss << setting.imageSize.width << 'x' << setting.imageSize.height;
    ss << '_' << setting.samplesPerPixel << '_' << setting.maxBounces;
    return ss.str();
}

bool LoadMeshToScene(Scene& scene, float scale, const glm::vec3& offset, const uint32_t matId,
                     const std::string& filename)
{
    using namespace tinyobj;

    std::string warn;
    std::string err;
    attrib_t attrib;
    std::vector<shape_t> shapes;
    std::vector<material_t> materials;
    bool ret = LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

    if (!warn.empty())
    {
        WARN("%s", warn.c_str());
    }
    if (!err.empty())
    {
        ERROR("%s", err.c_str());
    }
    if (!ret)
    {
        ERROR("Failed to load %s", filename.c_str());
        return ret;
    }

    LOG_INFO("# of vertices  = %d", (int)(attrib.vertices.size()) / 3);
    LOG_INFO("# of normals   = %d", (int)(attrib.normals.size()) / 3);
    LOG_INFO("# of texcoords = %d", (int)(attrib.texcoords.size()) / 2);
    LOG_INFO("# of materials = %d", (int)materials.size());
    LOG_INFO("# of shapes    = %d", (int)shapes.size());

    // TODO read material from mesh object

    for (shape_t& shape : shapes)
    {
        mesh_t& mesh = shape.mesh;

        for (std::size_t i = 0; i < mesh.indices.size() - 2; i += 3)
        {
            index_t& a = mesh.indices[i];
            index_t& b = mesh.indices[i + 1];
            index_t& c = mesh.indices[i + 2];

            real_t ax = attrib.vertices[3 * a.vertex_index];
            real_t ay = attrib.vertices[3 * a.vertex_index + 1];
            real_t az = attrib.vertices[3 * a.vertex_index + 2];

            real_t bx = attrib.vertices[3 * b.vertex_index];
            real_t by = attrib.vertices[3 * b.vertex_index + 1];
            real_t bz = attrib.vertices[3 * b.vertex_index + 2];

            real_t cx = attrib.vertices[3 * c.vertex_index];
            real_t cy = attrib.vertices[3 * c.vertex_index + 1];
            real_t cz = attrib.vertices[3 * c.vertex_index + 2];

            glm::vec3 va{ax * scale, ay * scale, az * scale};
            glm::vec3 vb{bx * scale, by * scale, bz * scale};
            glm::vec3 vc{cx * scale, cy * scale, cz * scale};

            AddTriangle(scene, Triangle{offset + va, offset + vb, offset + vc}, matId);
        }
    }

    return ret;
}
