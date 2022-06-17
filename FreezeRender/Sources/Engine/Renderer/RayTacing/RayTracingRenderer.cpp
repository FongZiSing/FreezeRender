#include "RayTracingRenderer.hpp"
#include <World/RenderWorld.hpp>
#include <Core/Base/Thread/ParallelFor.hpp>



RayTracingRenderer::RayTracingRenderer()
	: width(2)
	, height(2)
	, scene(2, 2)
	, depth(2, 2)
	, primitiveRay(2, 2)
{
}

void RayTracingRenderer::ScreenResize(int inWidth, int inHeight)
{
	width = inWidth;
	height = inHeight;

	scene.Resize(inWidth, inHeight);
	depth.Resize(inWidth, inHeight);
	primitiveRay.Invalidate(inWidth, inHeight);
}

ColorRenderTarget& RayTracingRenderer::Render(const RenderWorld* Scene)
{
	const Camera& viewBuffer = Scene->render.cameras[0];
	const Array<Meshlet>& meshletBuffer = Scene->render.meshlets;
	const Array<PointLight>& lightBuffer = Scene->render.pointlights;

	primitiveRay.TryRegenerateRay(viewBuffer.halfFOV, viewBuffer.data.aspectRatio);
	scene.Clear();
	depth.Clear();

	ParallelFor(0u, (unsigned int)width * height, [&](const unsigned int& worklistIndex)
	{
		const Matrix& view = viewBuffer.view;
		const Ray& ray = primitiveRay[worklistIndex];

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

					const float rayLength = (ray.direction * neark).Length();

					if (rayLength > 0 && depth.GetPixel(worklistIndex) > rayLength)
					{
						depth.SetPixel(worklistIndex, rayLength);
						const float& beta = result.beta;
						const float& gamma = result.gamma;
						const float& alpha = 1 - beta - gamma;
						Vector2 uv = triangle.vertices[0].localspace.uv * alpha
									+ triangle.vertices[1].localspace.uv * beta
									+ triangle.vertices[2].localspace.uv * gamma;
						scene.SetPixel(worklistIndex, triangle.material->diffuse.sampler(uv));
					}
				}
			}
		}
	});
	return scene;

}