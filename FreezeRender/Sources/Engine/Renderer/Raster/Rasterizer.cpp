#include "Rasterizer.hpp"

#include <Thread/RunnableTask.hpp>
#include <Thread/ParallelFor.hpp>
#include <Pattern/Singleton.hpp>
#include <World/RenderWorld.hpp>



inline namespace ParallelRaster
{
	// global rasterizer setting.
	namespace Config
	{
		static constexpr const bool bEnableVertexShader = false;
		static constexpr const bool bEnableHomogeneousClipping = true;
		static constexpr const bool bEnableBackFaceCulling = true;
		static constexpr const bool bEnableAdaptiveHalfSpaceRaster = true;
	}


	// Used only for clipping.
	namespace Clipping
	{
		ShadingTriangle gClippingTriangleBuffer[8];
		ShadingVertex gClippingVertexBuffer[16];
		constexpr const static float CLIPPING_PLANE = -0.000001f;
	}


	/**
	* @brief The Async task to clear or re-allocate render target.
	*/
	class DoubleBufferingTask final : public Singleton<DoubleBufferingTask>, TinyRunnableTask
	{
		// background render target.
		struct
		{
			MaterialIdBufferRenderTarget materialid{ 2, 2 };
			FloatRenderTarget depth{ 2, 2 };
			ColorRenderTarget scene{ 2, 2 };
		} background;

		std::atomic<int> width = 2;
		std::atomic<int> height = 2;

	private:
		template<typename RenderTarget>
		force_inline void ResizeOrReallocate(RenderTarget& target, const int& width, const int& height)
		{
			if (target.Width() != width || target.Height() != height)
			{
				target.Resize(width, height);
			}
			else
			{
				target.Clear();
			}
		}

		template<typename RenderTarget>
		force_inline void TryResize(RenderTarget& target, const int& width, const int& height)
		{
			if (target.Width() != width || target.Height() != height)
			{
				target.Resize(width, height);
			}
		}

		force_inline void Sync(VisibilityBuffer* VBuffer, GeometryBuffer* GBuffer, ColorRenderTarget* scene)
		{
			VBuffer->materialid.Swap(background.materialid);
			GBuffer->depth.Swap(background.depth);
			scene->Swap(background.scene);
		}

		force_inline void TryWork(const int& width, const int& height)
		{
			auto& [materialid, depth, scene] = background;
			TryResize(materialid, width, height);
			TryResize(depth, width, height);
			TryResize(scene, width, height);
		}

	protected:
		void DoWork() override
		{
			const int width = this->width.load(std::memory_order::consume);
			const int height = this->height.load(std::memory_order::consume);

			auto& [materialid, depth, scene] = background;
			ResizeOrReallocate(materialid, width, height);
			ResizeOrReallocate(depth, width, height);
			ResizeOrReallocate(scene, width, height);
		}

	public:
		DoubleBufferingTask(Token) { Start(); }

		void TrySync(VisibilityBuffer* VBuffer, GeometryBuffer* GBuffer, ColorRenderTarget* scene)
		{
			if (nullptr == VBuffer || nullptr == GBuffer)
				return;

			if (Timeout == Wait())
			{
				VBuffer->materialid.Clear();
				GBuffer->depth.Clear();
				scene->Clear();
			}
			else
			{
				const int w = VBuffer->materialid.Width();
				const int h = VBuffer->materialid.Height();

				TryWork(w, h);
				Sync(VBuffer, GBuffer, scene);

				this->width.store(w, std::memory_order::release);
				this->height.store(h, std::memory_order::release);
			}
			Start();
		}
	};
}



Rasterizer::Rasterizer()
	: width(2)
	, height(2)
	, VBuffer(2, 2)
	, WBuffer(2, 2)
	, GBuffer(2, 2)
	, scene(2, 2)
{
}

void Rasterizer::ScreenResize(int inWidth, int inHeight)
{
	width = inWidth;
	height = inHeight;
	VBuffer.Resize(inWidth, inHeight);
	WBuffer.Resize(inWidth, inHeight);
	GBuffer.Resize(inWidth, inHeight);
	scene.Resize(inWidth, inHeight);
}

ColorRenderTarget& Rasterizer::Render(const RenderWorld* Scene)
{
	const ShadingCamera& viewBuffer = Scene->render.cameras[0];
	const Array<ShadingMeshlet>& meshletBuffer = Scene->render.meshlets;
	const Array<ShadingPointLight>& lightBuffer = Scene->render.pointlights;

	PrePass(viewBuffer, meshletBuffer);
	BasePass(viewBuffer, lightBuffer);

	return scene;
}

void Rasterizer::PrePass(const ShadingCamera& viewBuffer, const Array<ShadingMeshlet>& meshletBuffer)
{
	// Clear last frame.
	DoubleBufferingTask::Get().TrySync(&VBuffer, &GBuffer, &scene);
	WBuffer.Clear();


	//****************************************************************
	// stage 1: Geometry process.
	//****************************************************************
	using namespace Clipping;

	const Matrix& projection = viewBuffer.projection;
	const Matrix& view = viewBuffer.view;
	const Matrix vp = projection * view;
	const Vector2 ndc2screen = viewBuffer.GetNdcToScreen();

	for (auto& meshletEntity : meshletBuffer)
	{
		branch_unlikely if (!meshletEntity.IsValid())
		{
			continue;
		}

		Meshlet& meshlet = meshletEntity.meshlet;
		const Matrix mvp = vp * meshlet.transform;
		const Matrix mv = view * meshlet.transform;
		const Matrix _mv = mv.Inverse().Transpose();

		for (ShadingMeshlet::Iterator It = meshletEntity.CreateIterator(); It; ++It)
		{
			// Init triangle.
			ShadingTriangle triangle = It.Assembly();

			// Disable vertex shader during compilation.
			if constexpr (Config::bEnableVertexShader)
			{
				// execute vertex shader.
				vertexShader({ triangle, mv, _mv, mvp });
			}

			// Model-View-Projection.
			triangle.vertices[0].screenspace.position = mvp * triangle.vertices[0].screenspace.position;
			triangle.vertices[1].screenspace.position = mvp * triangle.vertices[1].screenspace.position;
			triangle.vertices[2].screenspace.position = mvp * triangle.vertices[2].screenspace.position;

			// Homogeneous clip.
			int triangleNum = 0;
			HomogeneousClipping(triangle, gClippingTriangleBuffer, triangleNum);

			while (triangleNum --> 0)
			{
				ShadingTriangle& clippedTriangle = gClippingTriangleBuffer[triangleNum];
				for (ShadingVertex& vertex : clippedTriangle.vertices)
				{
					Vector4& position = vertex.screenspace.position;
					Vector3& location = vertex.viewspace.position;
					Vector3& normal = vertex.viewspace.normal;

					// Perspective division.
					// homogeneous clip space to normalized device coordinates(NDC) space.
					position.x /= position.w;
					position.y /= position.w;
					position.z /= position.w;

					// Viewport transformation (screen mapping).
					// NDC-space to screen space, y-axis up.
					position.x = 0.5f * width * (position.x + 1.f);
					position.y = 0.5f * height * (position.y + 1.f);
					position.z = position.z * ndc2screen.x + ndc2screen.y;

					// view transformation.
					// local space to view space.
					location = mv * location;
					normal = (_mv * Vector4(normal, 0.f)).XYZ().Normalize();
				}

				if (BackFaceCulling(clippedTriangle))
				{
					continue;
				}

				clippedTriangle.materialEntity = triangle.materialEntity;
				RasterizeTriangle(clippedTriangle);
			}
		}
	}

	//****************************************************************
	// stage 2: Triangle setup.
	//****************************************************************
	const int count = width * height;
	for (int i = 0; i < count; ++i)
	{
		// Triangle Traversal.
		if (VBuffer.materialid.GetPixel(i))
		{
			WBuffer.Push(VBuffer, i);
		}
	}

	//****************************************************************
	// stage 3: Generate geometry buffer.
	//****************************************************************
	ParallelFor(0u, WBuffer.number, [this](const unsigned int& worklistIndex)
		{
			const int& screenIndex = WBuffer.screen.GetPixel(worklistIndex);
			auto& interpolation = WBuffer.interpolation.GetPixel(worklistIndex);

			R256 result;
			const R256 attr1 = Register8LoadAligned(&WBuffer.vertex1.GetPixel(worklistIndex));
			const R256 attr2 = Register8LoadAligned(&WBuffer.vertex2.GetPixel(worklistIndex));
			const R256 attr3 = Register8LoadAligned(&WBuffer.vertex3.GetPixel(worklistIndex));
			const R256 depth = MakeRegister8(1.f / interpolation.oneOverDepth);
			const R256 alpha = MakeRegister8(interpolation.alpha);
			const R256 beta = MakeRegister8(interpolation.beta);
			const R256 gamma = MakeRegister8(interpolation.gamma);
			result = Register8MultiplyAddMultiply(attr1, alpha, attr2, beta);
			result = Register8MultiplyAdd(attr3, gamma, result);
			result = Register8Multiply(depth, result);

			ShadingPoint point;
			Register8StoreAligned(result, &point);

			const Color diffuseColor = WBuffer.materialid.GetPixel(worklistIndex)->diffuse.sampler(point.uv);
			GBuffer.position.SetPixel(screenIndex, { point.position, 0.f });
			GBuffer.normal.SetPixel(screenIndex, { point.normal, 0.f });
			GBuffer.diffuse.SetPixel(screenIndex, diffuseColor);
		});
}

void Rasterizer::BasePass(const ShadingCamera& viewBuffer, const Array<ShadingPointLight>& lightBuffer)
{
	//****************************************************************
	// stage 4: Parallel shading.
	//****************************************************************
	ParallelFor(0u, WBuffer.number, [this, &viewBuffer, &lightBuffer](const unsigned int& worklistIndex)
		{
			const int& screenIndex = WBuffer.screen.GetPixel(worklistIndex);
			Shader::DeferredFragmentPayload payload;

			payload.pointlights = &lightBuffer;
			payload.viewpoint = viewBuffer.camera.location;
			payload.shadingpoint = GBuffer.position.GetPixel(screenIndex).XYZRef();
			payload.normal = GBuffer.normal.GetPixel(screenIndex).XYZRef();
			payload.diffuse = GBuffer.diffuse.GetPixel(screenIndex);

			// execute fragment shader.
			scene.SetPixel(screenIndex, fragmentShader(payload));
		});
}

void Rasterizer::RasterizeTriangle(const ShadingTriangle& triangle)
{
	// [ Mileff P, Neh¨¦z K, Dudra J. 2015, "Accelerated Half-Space Triangle Rasterization" ]
	// Detail see http://acta.uni-obuda.hu//Mileff_Nehez_Dudra_63.pdf
	enum class Strategy : unsigned char
	{
		HalfSpace,      // half-space rasterization
		BlockHalfSpace, // Block-base half-space rasterization.
	};

	constexpr static const R128 R_HALF_SPACE_EPSILON = Number::MakeRegister(-1e-4f);

	const auto&& [v1, v2, v3] = triangle.ScreenspacePosition();
	const auto&& [l1, l2, l3] = triangle.ViewspacePosition();
	const auto&& [n1, n2, n3] = triangle.ViewspaceNormal();
	const auto&& [uv1, uv2, uv3] = triangle.LocalspaceUV();

	const auto&& [minx, maxx, miny, maxy] = triangle.BoundingBox(width, height);
	R128 startx = MakeRegister(minx + 0.5f);
	R128 starty = MakeRegister(miny + 0.5f);

	R128 v123x = MakeRegister(1.f, v1.x, v2.x, v3.x);
	R128 v123y = MakeRegister(1.f, v1.y, v2.y, v3.y);
	R128 v231x = RegisterSwizzle(v123x, 0, 2, 3, 1);
	R128 v231y = RegisterSwizzle(v123y, 0, 2, 3, 1);
	R128 v312w = MakeRegister(1.f, v3.w, v1.w, v2.w);

	R128 i = RegisterSubtract(v123y, v231y);
	R128 j = RegisterSubtract(v231x, v123x);
	R128 k = RegisterSubtract(RegisterMultiply(v123x, v231y), RegisterMultiply(v123y, v231x));

	// iteration-based (2 * area / depth).
	{
		R128 i0 = RegisterSum4(RegisterDivide(i, v312w));
		R128 j0 = RegisterSum4(RegisterDivide(j, v312w));
		R128 k0 = RegisterSum4(RegisterDivide(k, v312w));

		i = RegisterSelect(Number::R_XMASK, i0, i);
		j = RegisterSelect(Number::R_XMASK, j0, j);
		k = RegisterSelect(Number::R_XMASK, k0, k);
	}

	// iterator.
	const R128 invZ = MakeRegister(1.f, 1.f / v3.w, 1.f / v1.w, 1.f / v2.w);
	R128 f = RegisterAdd(RegisterMultiplyAddMultiply(i, startx, j, starty), k);
	{
		const R128 invArea2 = RegisterDivide(Number::R_ONE, MakeRegister(RegisterSum(RegisterSetX0(f))));

		// [ 1 / depth, gamma, alpha, beta ]
		f = RegisterMultiply(f, invArea2);

		// f[ d(1 / depth) / dx, d(gamma) / dx, d(alpha) / dx, d(beta) / dx ]
		i = RegisterMultiply(i, invArea2);

		// [ d(1 / depth) / dy, d(gamma) / dy, d(alpha) / dy, d(beta) / dy ]
		j = RegisterMultiply(j, invArea2);
	}

	if constexpr (Config::bEnableAdaptiveHalfSpaceRaster)
	{
		constexpr static const int block = 4;
		const int boxX = (maxx - minx);
		const int boxY = (maxy - miny);
		const float cost = boxX * 1.f / boxY;

		const bool adapt = cost > 0.40f && cost < 1.60f;
		const bool small = boxX < block && boxY < block;

		Strategy strategy = false ? Strategy::HalfSpace : Strategy::BlockHalfSpace;
		if (strategy == Strategy::HalfSpace)
		{
			R128 cy = f;
			for (int y = miny; y <= maxy; ++y)
			{
				R128 cx = cy;
				for (int x = minx; x <= maxx; ++x)
				{
					// if cx[1] > 0 and cx[2] > 0 and cx[3] > 0
					if ((RegisterMaskBits(RegisterGE(cx, R_HALF_SPACE_EPSILON)) & 0x0E) == 0x0E)
					{
						// (1 / depth, gamma, alpha, beta )
						const R128& zInverseAndInterpolation = cx;
						const float depth = 1.f / RegisterGetX(zInverseAndInterpolation);
						const int index = (height - y - 1) * width + x;

						// Z-depth testing.
						if (GBuffer.depth.GetPixel(index) < depth)
						{
							GBuffer.depth.SetPixel(index, depth);
							VBuffer.SetPixel(index, triangle, RegisterMultiply(zInverseAndInterpolation, invZ));
						}
					}
					cx = RegisterAdd(cx, i);
				}
				cy = RegisterAdd(cy, j);
			}
		}
		else
		{
			constexpr static const R128 R_OFFSET_X = Number::MakeRegister(0.f, block - 1.f, 0.f, block - 1.f);
			constexpr static const R128 R_OFFSET_Y = Number::MakeRegister(0.f, 0.f, block - 1.f, block - 1.f);
			constexpr static const R128 R_MOVE_STEP4 = Number::MakeRegister((float)block);

			const R128 repi1 = RegisterReplicate(i, 1);
			const R128 repi2 = RegisterReplicate(i, 2);
			const R128 repi3 = RegisterReplicate(i, 3);

			const R128 repj1 = RegisterReplicate(j, 1);
			const R128 repj2 = RegisterReplicate(j, 2);
			const R128 repj3 = RegisterReplicate(j, 3);

			const R128 offsetx1 = RegisterMultiply(repi1, R_OFFSET_X);
			const R128 offsety1 = RegisterMultiply(repj1, R_OFFSET_Y);
			const R128 offsetx2 = RegisterMultiply(repi2, R_OFFSET_X);
			const R128 offsety2 = RegisterMultiply(repj2, R_OFFSET_Y);
			const R128 offsetx3 = RegisterMultiply(repi3, R_OFFSET_X);
			const R128 offsety3 = RegisterMultiply(repj3, R_OFFSET_Y);

			const R128 movex = RegisterMultiply(i, R_MOVE_STEP4);
			const R128 movey = RegisterMultiply(j, R_MOVE_STEP4);
			const R128 movex1 = RegisterMultiply(repi1, R_MOVE_STEP4);
			const R128 movey1 = RegisterMultiply(repj1, R_MOVE_STEP4);
			const R128 movex2 = RegisterMultiply(repi2, R_MOVE_STEP4);
			const R128 movey2 = RegisterMultiply(repj2, R_MOVE_STEP4);
			const R128 movex3 = RegisterMultiply(repi3, R_MOVE_STEP4);
			const R128 movey3 = RegisterMultiply(repj3, R_MOVE_STEP4);

			R128 cy = f;
			R128 cy1 = RegisterAdd(RegisterReplicate(f, 1), RegisterAdd(offsetx1, offsety1));
			R128 cy2 = RegisterAdd(RegisterReplicate(f, 2), RegisterAdd(offsetx2, offsety2));
			R128 cy3 = RegisterAdd(RegisterReplicate(f, 3), RegisterAdd(offsetx3, offsety3));

			for (int y = miny; y <= maxy; y += block)
			{
				R128 cx = cy;
				R128 cx1 = cy1;
				R128 cx2 = cy2;
				R128 cx3 = cy3;
				for (int x = minx; x <= maxx; x += block)
				{
					const int checkw = RegisterMaskBits(RegisterGE(cx1, R_HALF_SPACE_EPSILON));
					const int checku = RegisterMaskBits(RegisterGE(cx2, R_HALF_SPACE_EPSILON));
					const int checkv = RegisterMaskBits(RegisterGE(cx3, R_HALF_SPACE_EPSILON));

					// Fully outside.
					if (checkw == 0x0 || checku == 0x0 || checkv == 0x0)
					{
						cx = RegisterAdd(cx, movex);
						cx1 = RegisterAdd(cx1, movex1);
						cx2 = RegisterAdd(cx2, movex2);
						cx3 = RegisterAdd(cx3, movex3);
						continue;
					}

					// Fully covered blocks.
					if (checkw == 0xF && checku == 0xF && checkv == 0xF)
					{
						R128 cyb = cx;
						for (int by = 0; by < block; ++by)
						{
							R128 cxb = cyb;
							for (int bx = 0; bx < block; ++bx)
							{
								// (1 / depth, gamma, alpha, beta )
								const R128& zInverseAndInterpolation = cxb;
								const float depth = 1.f / RegisterGetX(zInverseAndInterpolation);
								const int index = (height - (y + by) - 1) * width + (x + bx);

								// Z-depth testing.
								if (GBuffer.depth.IsValid(index) && GBuffer.depth.GetPixel(index) < depth)
								{
									GBuffer.depth.SetPixel(index, depth);
									VBuffer.SetPixel(index, triangle, RegisterMultiply(zInverseAndInterpolation, invZ));
								}
								cxb = RegisterAdd(cxb, i);
							}
							cyb = RegisterAdd(cyb, j);
						}
					}

					// Partially covered blocks.
					else
					{
						R128 cyb = cx;
						for (int by = 0; by < block; ++by)
						{
							R128 cxb = cyb;
							for (int bx = 0; bx < block; ++bx)
							{
								// if cx[1] > 0 and cx[2] > 0 and cx[3] > 0
								if ((RegisterMaskBits(RegisterGE(cxb, R_HALF_SPACE_EPSILON)) & 0x0E) == 0x0E)
								{
									// (1 / depth, gamma, alpha, beta )
									const R128& zInverseAndInterpolation = cxb;
									const float depth = 1.f / RegisterGetX(zInverseAndInterpolation);
									const int index = (height - (y + by) - 1) * width + (x + bx);

									// Z-depth testing.
									if (GBuffer.depth.IsValid(index) && GBuffer.depth.GetPixel(index) < depth)
									{
										GBuffer.depth.SetPixel(index, depth);
										VBuffer.SetPixel(index, triangle, RegisterMultiply(zInverseAndInterpolation, invZ));
									}
								}
								cxb = RegisterAdd(cxb, i);
							}
							cyb = RegisterAdd(cyb, j);
						}
					}

					cx = RegisterAdd(cx, movex);
					cx1 = RegisterAdd(cx1, movex1);
					cx2 = RegisterAdd(cx2, movex2);
					cx3 = RegisterAdd(cx3, movex3);
				}

				cy = RegisterAdd(cy, movey);
				cy1 = RegisterAdd(cy1, movey1);
				cy2 = RegisterAdd(cy2, movey2);
				cy3 = RegisterAdd(cy3, movey3);
			}
		}
	}
	else
	{
		R128 cy = f;
		for (int y = miny; y <= maxy; ++y)
		{
			R128 cx = cy;
			for (int x = minx; x <= maxx; ++x)
			{
				// if cx[1] > 0 and cx[2] > 0 and cx[3] > 0
				if ((RegisterMaskBits(RegisterGE(cx, R_HALF_SPACE_EPSILON)) & 0x0E) == 0x0E)
				{
					// (1 / depth, gamma, alpha, beta )
					const R128& zInverseAndInterpolation = cx;
					const float depth = 1.f / RegisterGetX(zInverseAndInterpolation);
					const int index = (height - y - 1) * width + x;

					// Z-depth testing.
					if (GBuffer.depth.GetPixel(index) < depth)
					{
						GBuffer.depth.SetPixel(index, depth);

						// update VBuffer.
						Register8Copy(&triangle.vertices[0], &VBuffer.vertex1.GetPixel(index));
						Register8Copy(&triangle.vertices[1], &VBuffer.vertex2.GetPixel(index));
						Register8Copy(&triangle.vertices[2], &VBuffer.vertex3.GetPixel(index));
						RegisterStoreAligned(RegisterMultiply(zInverseAndInterpolation, invZ), &VBuffer.interpolation.GetPixel(index));
						VBuffer.materialid.SetPixel(index, triangle.materialEntity);
					}
				}
				cx = RegisterAdd(cx, i);
			}
			cy = RegisterAdd(cy, j);
		}
	}
}

bool Rasterizer::BackFaceCulling(const ShadingTriangle& triangle)
{
	// Disable culling during compilation.
	if constexpr (!Config::bEnableBackFaceCulling)
	{
		return false;
	}

	// Back face culling in screen space.
	const Vector4& a = triangle.vertices[0].screenspace.position;
	const Vector4& b = triangle.vertices[1].screenspace.position;
	const Vector4& c = triangle.vertices[2].screenspace.position;

	const Vector2 ab = b.XY() - a.XY();
	const Vector2 ac = c.XY() - a.XY();
	return (ab ^ ac) < 0;
}

void Rasterizer::HomogeneousClipping(const ShadingTriangle& triangle, ShadingTriangle* outTriangles, int& triangleNum)
{
	// refer to https://fabiensanglard.net/polygon_codec/
	using namespace Clipping;

	// Disable Clipping during compilation.
	if constexpr (!Config::bEnableHomogeneousClipping)
	{
		outTriangles[0] = triangle;
		triangleNum = 1;
		return;
	}

	// The function that determines whether triangle is in the canonical view volume,
	// call it before perspective division.
	auto PolygonVisible = [](const ShadingTriangle& triangle) -> bool
	{
		const Vector4& v1 = triangle.vertices[0].screenspace.position;
		const Vector4& v2 = triangle.vertices[1].screenspace.position;
		const Vector4& v3 = triangle.vertices[2].screenspace.position;

		return
			std::fabs(v1.x) <= -v1.w && std::fabs(v1.y) <= -v1.w && std::fabs(v1.z) <= -v1.w &&
			std::fabs(v2.x) <= -v2.w && std::fabs(v2.y) <= -v2.w && std::fabs(v2.z) <= -v2.w &&
			std::fabs(v3.x) <= -v3.w && std::fabs(v3.y) <= -v3.w && std::fabs(v3.z) <= -v3.w;
	};

	// The function that clip polygon in the `Axis-W` plane.
	auto ClipPolygonForAxisW = [](int& inOutVertexNum, ShadingVertex* inVertices, ShadingVertex* outVertices) -> void
	{
		int newVertexNum = 0;
		const ShadingVertex* vertex1 = &inVertices[inOutVertexNum - 1]; // edge tail
		const ShadingVertex* vertex2 = &inVertices[0];					// edge head

		while (inOutVertexNum-- > 0)
		{
			const Vector4& position1 = vertex1->screenspace.position;
			const Vector4& position2 = vertex2->screenspace.position;
			const float& axis1W = position1.w;
			const float& axis2W = position2.w;

			const int out1 = axis1W > CLIPPING_PLANE ? -1 : 1;
			const int out2 = axis2W > CLIPPING_PLANE ? -1 : 1;

			if ((out1 ^ out2) < 0)
			{
				// NewVertex = vertex1 + t * (vertex2 - vertex1)
				float t = (axis1W - CLIPPING_PLANE) / (axis1W - axis2W);
				outVertices[newVertexNum++] = Lerp(t, *vertex1, *vertex2);
			}

			if (out2 > 0)
			{
				outVertices[newVertexNum++] = *vertex2;
			}

			vertex1 = vertex2;
			vertex2++;
		}
		inOutVertexNum = newVertexNum;
	};

	// The function that clip polygon in the `Axis` plane.
	enum ClipAxis : unsigned int { X = 0, Y = 1, Z = 2, w = 3 };
	auto ClipPolygonForAxis = [](ClipAxis&& axis, int&& sign, int& inOutVertexNum, ShadingVertex* inVertices, ShadingVertex* outVertices) -> void
	{
		// Clip against first plane if sign == 1
		// Clip against opposite plane if sign == -1
		if (inOutVertexNum <= 2) return;

		int newVertexNum = 0;
		const ShadingVertex* vertex1 = &inVertices[inOutVertexNum - 1]; // edge tail
		const ShadingVertex* vertex2 = &inVertices[0];					// edge head

		while (inOutVertexNum-- > 0)
		{
			const Vector4& position1 = vertex1->screenspace.position;
			const Vector4& position2 = vertex2->screenspace.position;

			const float& axis1W = position1.w;
			const float& axis2W = position2.w;
			const float& axis1 = *(&position1.x + axis) * sign;
			const float& axis2 = *(&position2.x + axis) * sign;

			const int out1 = axis1 >= axis1W ? 1 : -1;
			const int out2 = axis2 >= axis2W ? 1 : -1;

			if ((out1 ^ out2) < 0)
			{
				// NewVertex = vertex1 + t * (vertex2 - vertex1)
				float t = (axis1W - axis1) / ((axis1W - axis1) - (axis2W - axis2));
				outVertices[newVertexNum++] = Lerp(t, *vertex1, *vertex2);
			}

			if (out2 > 0)
			{
				outVertices[newVertexNum++] = *vertex2;
			}

			vertex1 = vertex2;
			vertex2++;
		}
		inOutVertexNum = newVertexNum;
	};


	// start to clip
	if (PolygonVisible(triangle))
	{
		// The triangle is in the canonical view volume completely.
		outTriangles[0] = triangle;
		triangleNum = 1;
	}
	else
	{
		// Clip triangle.
		ShadingVertex* vertices1 = gClippingVertexBuffer;
		ShadingVertex* vertices2 = gClippingVertexBuffer + (sizeof(gClippingVertexBuffer) / sizeof(ShadingVertex) / 2);
		int vertexNum = 0;

		vertices1[vertexNum++] = triangle.vertices[0];
		vertices1[vertexNum++] = triangle.vertices[1];
		vertices1[vertexNum++] = triangle.vertices[2];

		ClipPolygonForAxisW(vertexNum, vertices1, vertices2);

		ClipPolygonForAxis(ClipAxis::X, +1, vertexNum, vertices2, vertices1);
		ClipPolygonForAxis(ClipAxis::X, -1, vertexNum, vertices1, vertices2);

		ClipPolygonForAxis(ClipAxis::Y, +1, vertexNum, vertices2, vertices1);
		ClipPolygonForAxis(ClipAxis::Y, -1, vertexNum, vertices1, vertices2);

		ClipPolygonForAxis(ClipAxis::Z, +1, vertexNum, vertices2, vertices1);
		ClipPolygonForAxis(ClipAxis::Z, -1, vertexNum, vertices1, vertices2);

		// triangle assembly.
		if (vertexNum >= 3)
		{
			triangleNum = vertexNum - 2;
			for (int index = 0; index < triangleNum; ++index)
			{
				new (outTriangles + index) ShadingTriangle(vertices2[0], vertices2[index + 1], vertices2[index + 2]);
			}
		}
		else
		{
			triangleNum = 0;
		}
	}
}