#include "RayTracingRenderer.hpp"
#include <World/RenderWorld.hpp>
#include <Core/Base/Thread/ParallelFor.hpp>


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
bool rayTriangleIntersect(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& orig,
    const Vector3& dir, float& tnear, float& u, float& v)
{
    // TODO: Implement this function that tests whether the triangle
    // that's specified bt v0, v1 and v2 intersects with the ray (whose
    // origin is *orig* and direction is *dir*)
    // Also don't forget to update tnear, u and v.
    constexpr float EPSILON = 0.000001;

    const Vector3 E1 = v1 - v0;
    const Vector3 E2 = v2 - v0;
    const Vector3 S = orig - v0;
    const Vector3 S1 = dir ^ E2;
    const Vector3 S2 = S ^ E1;

    const float Coeff = S1 | E1;
    if (std::abs(Coeff) < EPSILON)
    {
        // This ray is parallel to this triangle.
        return false;
    }

    const float Inv = 1.f / Coeff;

    u = Inv * (S1 | S);
    if (u < 0 || u > 1.f)
    {
        return false;
    }

    v = Inv * (S2 | dir);
    if (v < 0 || u + v > 1.f)
    {
        return false;
    }

    tnear = Inv * (S2 | E2);
    if (tnear > EPSILON)
    {
        return true;
    }

    // This means that there is a line intersection but not a ray intersection.
    return false;
}
ColorRenderTarget& RayTracingRenderer::Render(const RenderWorld* Scene)
{
    const ShadingCamera& viewBuffer = Scene->render.cameras[0];
    const Array<ShadingMeshlet>& meshletBuffer = Scene->render.meshlets;
    const Array<ShadingPointLight>& lightBuffer = Scene->render.pointlights;

    if (bRegeneratePrimitiveRay)
    {
        int c = 0;
        float scale = std::tan(viewBuffer.halfFOV);
        float aspectRatio = viewBuffer.camera.aspectRatio;
        for (int h = 0; h < height; ++h)
        {
            for (int w = 0; w < width; ++w)
            {
                float x = (2.f * (w + 0.5f) / (float)width - 1.f) * aspectRatio * scale;
                float y = (1 - 2 * (h + 0.5) / (float)height) * scale;
                primitiveRay.SetPixel(c++, Ray { viewBuffer.camera.location, Vector3(x, y, -1).Normalize() });
            }
        }
        bRegeneratePrimitiveRay = false;
    }

    scene.Clear();
    ParallelFor(0u, (unsigned int)width * height, [&](const unsigned int& worklistIndex)
    {
		const Matrix& view = viewBuffer.view;
        Ray& ray = primitiveRay.GetPixel(worklistIndex);

        for (auto& meshletEntity : meshletBuffer)
        {
            branch_unlikely if (!meshletEntity.IsValid())
            {
                continue;
            }

            Meshlet& meshlet = meshletEntity.meshlet;
            const Matrix mv = view * meshlet.transform;

            for (ShadingMeshlet::Iterator It = meshletEntity.CreateIterator(); It; ++It)
            {
                // Init triangle.
                ShadingTriangle triangle = It.Assembly();

                // Model-View-Projection.
                triangle.vertices[0].viewspace.position = mv * triangle.vertices[0].viewspace.position;
                triangle.vertices[1].viewspace.position = mv * triangle.vertices[1].viewspace.position;
                triangle.vertices[2].viewspace.position = mv * triangle.vertices[2].viewspace.position;

                float neark = std::numeric_limits<float>::max();
                float tnear, u, v;
                if (rayTriangleIntersect(
                    triangle.vertices[0].viewspace.position,
                    triangle.vertices[1].viewspace.position,
                    triangle.vertices[2].viewspace.position,
                    ray.origin,
                    ray.direction,
                    tnear,
                    u, v))
                {
                    neark = tnear;
                    scene.SetPixel(worklistIndex, Color::Blue);
                    break;
                }
            }
        }
    });
    return scene;

}
