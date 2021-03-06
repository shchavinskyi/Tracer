#include "render.h"

#include "logging.h"
#include "random.h"
#include "scene.h"

#include <chrono>
#include <future>
#include <glm/gtx/intersect.hpp>
#include <list>
#include <variant>

namespace {

inline Ray GetRay(const Camera& camera, float u, float v)
{
    return Ray{camera.origin,
               glm::normalize(camera.lowerLeft + u * camera.horizontal + v * camera.vertical - camera.origin)};
}

glm::vec3 TracePath(const Ray& ray, uint32_t maxDepth, const Scene& scene)
{
    if (maxDepth == 0)
    {
        return glm::vec3(0, 0, 0);
    }

    bool isHit = false;
    float distanceToClosest = std::numeric_limits<float>::max();

    HitResult hitResult{glm::vec3(0.0f), glm::vec3(0.0f), 0};

    // trace spheres
    for (size_t i = 0; i < scene.spheresGeometry.size(); ++i)
    {
        const Sphere& sphere = scene.spheresGeometry[i];

        glm::vec3 intersectionPosition;
        glm::vec3 intersectionNormal;

        if (glm::intersectRaySphere(ray.origin, ray.direction, sphere.center, sphere.radius, intersectionPosition,
                                    intersectionNormal))
        {
            const float lenghtToIntersection = glm::length(intersectionPosition - ray.origin);
            if (lenghtToIntersection < distanceToClosest && lenghtToIntersection > 0.001f)
            {
                distanceToClosest = lenghtToIntersection;
                hitResult.normal = intersectionNormal;
                hitResult.position = intersectionPosition;
                hitResult.materialId = scene.spheresMaterial[i];
                isHit = true;
            }
        }
    }

    // trace triangles
    for (size_t i = 0; i < scene.trianglesGeometry.size(); ++i)
    {
        const Triangle& triangle = scene.trianglesGeometry[i];
        float distance = std::numeric_limits<float>::min();
        glm::vec2 baryPosition;
        if (glm::intersectRayTriangle(ray.origin, ray.direction, triangle.v1, triangle.v2, triangle.v3, baryPosition,
                                      distance))
        {
            if (distance < distanceToClosest && distance > 0.001f)
            {
                const glm::vec3 edge1 = triangle.v2 - triangle.v1;
                const glm::vec3 edge2 = triangle.v3 - triangle.v1;
                const glm::vec3 position = triangle.v1 + baryPosition.x * edge1 + baryPosition.y * edge2;

                glm::vec3 normal(glm::normalize(glm::cross(edge1, edge2)));

                distanceToClosest = distance;
                hitResult.normal = normal;
                hitResult.position = position;
                hitResult.materialId = scene.trianglesMaterial[i];
                isHit = true;
            }
        }
    }

    if (isHit)
    {

        const Material& material = scene.materials[hitResult.materialId];
        if (const Material::LightData* data = std::get_if<Material::LightData>(&material.data))
        {
            return data->emissive;
        }

        if (std::optional<ScatterResult> scatterResult = Scatter(ray, hitResult, material))
        {
            return scatterResult->attenuation * TracePath(scatterResult->ray, maxDepth - 1, scene);
        }
    }

    return scene.backgroundColor;
}

} // namespace

void RenderScene(const Scene& scene, RenderBuffer renderBuffer)
{
    RandomFloatGenerator floatGenerator;

    uint32_t bufferLastIndex = renderBuffer.start + renderBuffer.length;

    assert(bufferLastIndex <= scene.settings.imageSize.width * scene.settings.imageSize.height);

    for (uint32_t i = renderBuffer.start; i < bufferLastIndex; ++i)
    {
        uint32_t x = i % scene.settings.imageSize.height;
        uint32_t y = i / scene.settings.imageSize.height;

        glm::vec3 acumulatedColor(0.0f, 0.0f, 0.0f);

        for (uint32_t j = 0; j < scene.settings.samplesPerPixel; ++j)
        {
            float u = (float(x) + floatGenerator.Generate()) / float(scene.settings.imageSize.width);
            float v = (float(y) + floatGenerator.Generate()) / float(scene.settings.imageSize.height);

            Ray ray = GetRay(scene.camera, u, v);

            acumulatedColor += TracePath(ray, scene.settings.maxBounces, scene);
        }

        acumulatedColor /= scene.settings.samplesPerPixel;
        acumulatedColor = glm::clamp(acumulatedColor, 0.0f, 1.0f);

        Color& outputPixel = renderBuffer.buffer[i];
        outputPixel.r = static_cast<uint8_t>(255.999f * acumulatedColor.r);
        outputPixel.g = static_cast<uint8_t>(255.999f * acumulatedColor.g);
        outputPixel.b = static_cast<uint8_t>(255.999f * acumulatedColor.b);
    }
}

void RenderSceneMT(const Scene& scene, RenderBuffer& renderBuffer, uint32_t threadCount)
{
    LOG_INFO("Rendering scene using [ %d ] threads.", threadCount);

    // 0 threads , 1 async
#if 1
    constexpr uint32_t pixelCountPerTask = 1024;

    uint32_t taskCount =
        renderBuffer.length / pixelCountPerTask + (renderBuffer.length % pixelCountPerTask == 0 ? 0 : 1);

    LOG_INFO("%d Pixels splitted into %d tasks", renderBuffer.length, taskCount);

    std::list<std::future<void>> futures;

    uint32_t availableThreads = threadCount;
    uint32_t availableTasks = taskCount;
    uint32_t progress = 0;

    while (availableTasks > 0)
    {

        while (availableThreads > 0 && availableTasks > 0)
        {
            uint32_t nextTaskIndex = taskCount - availableTasks;
            uint32_t pixelsToProcess = pixelCountPerTask;

            if (availableTasks == 1)
            {
                pixelsToProcess = renderBuffer.length - nextTaskIndex * pixelCountPerTask;
            }

            futures.emplace_back(
                std::async(std::launch::async, RenderScene, std::ref(scene),
                           RenderBuffer{renderBuffer.buffer, nextTaskIndex * pixelCountPerTask, pixelsToProcess}));

            --availableTasks;
            --availableThreads;
        }

        assert(futures.size() == threadCount);
        assert(availableThreads == 0);

        while (availableThreads <= 0 && availableTasks > 0)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1));

            for (auto it = futures.begin(); it != futures.end();)
            {
                if (std::future_status::ready == it->wait_for(std::chrono::nanoseconds(1)))
                {
                    it = futures.erase(it);
                    availableThreads++;
                }
                else
                {
                    ++it;
                }
            }
        }

        constexpr uint32_t progresDiv = 10;
        uint32_t percent = uint32_t(float(taskCount - availableTasks) / float(taskCount) * 100);
        if (percent % progresDiv == 0 && percent != progress)
        {
            progress = percent;
            LOG_INFO("Tracing porgress %d", progress);
        }
    }

#else
    uint32_t pixelCountPerThread = renderBuffer.length / threadCount;

    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    for (unsigned int i = 0; i < threadCount; ++i)
    {
        threads.emplace_back(
            std::thread(RenderScene, std::ref(scene),
                        RenderBuffer{renderBuffer.buffer, pixelCountPerThread * i, pixelCountPerThread}));
    }

    for (unsigned int i = 0; i < threadCount; ++i)
    {
        threads[i].join();
    }
#endif
}
