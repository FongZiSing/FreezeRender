#include "RayTracingRenderer.hpp"
#include <World/RenderWorld.hpp>
#include <Core/Base/Thread/ParallelFor.hpp>



using namespace Shading;

RayTracingRenderer::RayTracingRenderer()
    : width(2)
    , height(2)
    , scene(2, 2)
    , primitiveRay(2, 2)
    , bRegeneratePrimitiveRay(false)
{
}

void RayTracingRenderer::ScreenResize(int inWidth, int inHeight)
{
    width = inWidth;
    height = inHeight;

    scene.Resize(inWidth, inHeight);
    primitiveRay.Resize(inWidth, inHeight);
    bRegeneratePrimitiveRay = true;
}

ColorRenderTarget& RayTracingRenderer::Render(const RenderWorld* Scene)
{
    const Camera& viewBuffer = Scene->render.cameras[0];
    const Array<Meshlet>& meshletBuffer = Scene->render.meshlets;
    const Array<PointLight>& lightBuffer = Scene->render.pointlights;

    if (bRegeneratePrimitiveRay)
    {
        int c = 0;
        float scale = std::tan(viewBuffer.halfFOV);
        float aspectRatio = viewBuffer.data.aspectRatio;
        for (int h = 0; h < height; ++h)
        {
            for (int w = 0; w < width; ++w)
            {
                float x = (2.f * (w + 0.5f) / (float)width - 1.f) * aspectRatio * scale;
                float y = (1 - 2 * (h + 0.5f) / (float)height) * scale;
                primitiveRay.SetPixel(c++, Ray { viewBuffer.data.location, Vector3(x, y, -1).Normalize() });
            }
        }
        bRegeneratePrimitiveRay = false;
    }

    scene.Clear();
    ParallelFor(0u, (unsigned int)width * height, [&](const unsigned int& worklistIndex)
    {
		const Matrix& view = viewBuffer.view;
        Ray& ray = primitiveRay.GetPixel(worklistIndex);

        for (auto& perMeshlet : meshletBuffer)
        {
            branch_unlikely if (!perMeshlet.IsValid())
            {
                continue;
            }

            AMeshlet& meshlet = perMeshlet.data;
            const Matrix mv = view * meshlet.transform;

            for (Meshlet::Iterator It = perMeshlet.CreateIterator(); It; ++It)
            {
                // Init triangle.
                ShadingTriangle triangle = It.Assembly();

                // Model-View-Projection.
                triangle.vertices[0].viewspace.position = mv * triangle.vertices[0].viewspace.position;
                triangle.vertices[1].viewspace.position = mv * triangle.vertices[1].viewspace.position;
                triangle.vertices[2].viewspace.position = mv * triangle.vertices[2].viewspace.position;

                float neark = std::numeric_limits<float>::max();
                HitResult result = ray.Intersect(triangle);
                if (result == HitStatus::Success)
                {
                    neark = result.t;
                    scene.SetPixel(worklistIndex, Color::Blue);
                    break;
                }
            }
        }
    });
    return scene;

}
