#include "Rasterizer.hpp"
#include <Algorithm/KahanSummation.hpp>



inline namespace Raster
{
	// global rasterizer setting.
	namespace Config
	{
		static constexpr const bool bEnableSIMDRaster = true;
		static constexpr const bool bEnableVertexShader = false;
		static constexpr const bool bEnableHomogeneousClipping = true;
		static constexpr const bool bEnableBackFaceCulling = true;
	}

	// Used only for clipping.
	namespace Clipping
	{
		ShadingTriangle gClippingTriangleBuffer[8];
		ShadingVertex gClippingVertexBuffer[16];
		constexpr const static float CLIPPING_PLANE = -0.000001f;
	}
}



Rasterizer::Rasterizer(int inWidth, int inHeight)
	: sceneBuffer(inWidth, inHeight)
	, depthBuffer(inWidth, inHeight)
	, width(inWidth)
	, height(inHeight)
{
}

void Rasterizer::Resize(int inWidth, int inHeight)
{
	sceneBuffer.Resize(inWidth, inHeight);
	depthBuffer.Resize(inWidth, inHeight);
	width = inWidth;
	height = inHeight;
}

ColorRenderTarget& Rasterizer::Draw()
{
	using namespace Clipping;

 	const Matrix& projection = viewStateBuffer.projection;
	const Matrix& view = viewStateBuffer.view;
	const Matrix vp = projection * view;
	const float ndc2screen1 = (viewStateBuffer.farPlane - viewStateBuffer.nearPlane) / 2.f;
	const float ndc2screen2 = (viewStateBuffer.farPlane + viewStateBuffer.nearPlane) / 2.f;

	sceneBuffer.Clear();
	depthBuffer.Clear();
	for (auto& mesh : meshBuffer)
	{
		if (!mesh.IsValid())
		{
			continue;
		}

		const Matrix mvp = vp * mesh.transform;
		const Matrix mv = view * mesh.transform;
		const Matrix invMV = mv.Inverse().Transpose();
		
		for (ShadingMeshletIterator It(mesh); It; ++It)
		{
			// Init triangle.
			ShadingTriangle triangle = It.Assembly();

			// Disable vertex shader during compilation.
			if constexpr (Config::bEnableVertexShader)
			{
				// execute vertex shader.
				vertexShader({ triangle, mv, invMV, mvp });
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

					// Viewport transformation.
					// NDC-space to screen space, y-axis up.
					position.x = 0.5f * width * (position.x + 1.f);
					position.y = 0.5f * height * (position.y + 1.f);
					position.z = position.z * ndc2screen1 + ndc2screen2;

					// view transformation.
					// local space to view space.
					location = mv * location;
					normal = (invMV * Vector4(normal, 0.f)).XYZ().Normalize();
				}

				if (BackFaceCulling(clippedTriangle))
				{
					continue;
				}
				
				clippedTriangle.material = &mesh.materials[0];
				RasterizerTriangle(clippedTriangle);
			}
		}
	}

	return sceneBuffer;
}

bool Rasterizer::BackFaceCulling(const ShadingTriangle& triangle)
{
	//****************************************************************
	// Disable culling during compilation.
	//****************************************************************
	if constexpr (!Config::bEnableBackFaceCulling)
	{
		return false;
	}
	
	//****************************************************************
	// Back face culling in screen space.
	//****************************************************************
	const Vector4& a = triangle.vertices[0].screenspace.position;
	const Vector4& b = triangle.vertices[1].screenspace.position;
	const Vector4& c = triangle.vertices[2].screenspace.position;

	const Vector2 ab = b.XY() - a.XY();
	const Vector2 ac = c.XY() - a.XY();
	return (ab ^ ac) < 0;
}

void Rasterizer::HomogeneousClipping(const ShadingTriangle& triangle, ShadingTriangle* outTriangles, int& triangleNum)
{
	using namespace Clipping;

	//****************************************************************
	// refer to https://fabiensanglard.net/polygon_codec/
	//****************************************************************

	//****************************************************************
	// Disable Clipping during compilation.
	//****************************************************************
	if constexpr (!Config::bEnableHomogeneousClipping)
	{
		outTriangles[0] = triangle;
		triangleNum = 1;
		return;
	}
	
	//****************************************************************
	// The function that determines whether triangle is in the canonical view volume, call it before perspective division.
	//****************************************************************
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

	//****************************************************************
	// The function that clip polygon in the `Axis-W` plane.
	//****************************************************************
	auto ClipPolygonForAxisW = [](int& inOutVertexNum, ShadingVertex* inVertices, ShadingVertex* outVertices) -> void
	{
		int newVertexNum = 0;
		const ShadingVertex* vertex1 = &inVertices[inOutVertexNum - 1]; // edge tail
		const ShadingVertex* vertex2 = &inVertices[0];					// edge head

		while (inOutVertexNum --> 0)
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
				float t = (axis1W - CLIPPING_PLANE) / (axis1W  - axis2W);
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

	//****************************************************************
	// The function that clip polygon in the `Axis` plane.
	//****************************************************************
	enum ClipAxis : unsigned int { X = 0, Y = 1, Z = 2, w = 3 };
	auto ClipPolygonForAxis = [](ClipAxis&& axis, int&& sign, int& inOutVertexNum, ShadingVertex* inVertices, ShadingVertex* outVertices) -> void
	{
		// Clip against first plane if sign == 1
		// Clip against opposite plane if sign == -1
		if (inOutVertexNum <= 2) return;

		int newVertexNum = 0;
		const ShadingVertex* vertex1 = &inVertices[inOutVertexNum - 1]; // edge tail
		const ShadingVertex* vertex2 = &inVertices[0];					// edge head

		while (inOutVertexNum --> 0)
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

void Rasterizer::RasterizerTriangle(const ShadingTriangle& triangle)
{
	//****************************************************************
	// [ Mileff P, Nehéz K, Dudra J. 2015, "Accelerated Half-Space Triangle Rasterization" ] http://acta.uni-obuda.hu//Mileff_Nehez_Dudra_63.pdf
	// TODO: Block-based Bisector Half-Space Rasterization.
	//****************************************************************
	const Vector4& v1 = triangle.vertices[0].screenspace.position;
	const Vector4& v2 = triangle.vertices[1].screenspace.position;
	const Vector4& v3 = triangle.vertices[2].screenspace.position;

	const Vector3& l1 = triangle.vertices[0].viewspace.position;
	const Vector3& l2 = triangle.vertices[1].viewspace.position;
	const Vector3& l3 = triangle.vertices[2].viewspace.position;

	const Vector3& n1 = triangle.vertices[0].viewspace.normal;
	const Vector3& n2 = triangle.vertices[1].viewspace.normal;
	const Vector3& n3 = triangle.vertices[2].viewspace.normal;

	const Vector2& uv1 = triangle.vertices[0].localspace.uv;
	const Vector2& uv2 = triangle.vertices[1].localspace.uv;
	const Vector2& uv3 = triangle.vertices[2].localspace.uv;

	// Axis-aligned bounding box.
	const int minX = std::max(      0,     static_cast<int>( std::min({ v1.x, v2.x, v3.x }) ) );
	const int maxX = std::min( width - 1,  static_cast<int>( std::max({ v1.x, v2.x, v3.x }) ) );
	const int minY = std::max(      0,     static_cast<int>( std::min({ v1.y, v2.y, v3.y }) ) );
	const int maxY = std::min( height - 1, static_cast<int>( std::max({ v1.y, v2.y, v3.y }) ) );

	if constexpr (Config::bEnableSIMDRaster)
	{

		R128 mX = MakeRegister(static_cast<float>(minX));
		R128 mY = MakeRegister(static_cast<float>(minY));

		// d( area / depth, ab x ap, bc x bp, ca x cp ) / dx, d( area / depth, ab x ap, bc x bp, ca x cp ) / dy
		R128 v1v2v3x = MakeRegister(1.f, v1.x, v2.x, v3.x);
		R128 v2v3v1x = RegisterSwizzle(v1v2v3x, 0, 2, 3, 1);
		R128 v1v2v3y = MakeRegister(1.f, v1.y, v2.y, v3.y);
		R128 v2v3v1y = RegisterSwizzle(v1v2v3y, 0, 2, 3, 1);
		R128 v3v1v2w = MakeRegister(1.f, v3.w, v1.w, v2.w);
		R128 i = RegisterSubtract(v1v2v3y, v2v3v1y);
		R128 j = RegisterSubtract(v2v3v1x, v1v2v3x);
		R128 k = RegisterSubtract(RegisterMultiply(v1v2v3x, v2v3v1y), RegisterMultiply(v1v2v3y, v2v3v1x));
		R128 i0 = RegisterSum4(RegisterDivide(i, v3v1v2w));
		R128 j0 = RegisterSum4(RegisterDivide(j, v3v1v2w));
		R128 k0 = RegisterSum4(RegisterDivide(k, v3v1v2w));
		i = RegisterSelect(Number::R_XMASK, i0, i);
		j = RegisterSelect(Number::R_XMASK, j0, j);
		k = RegisterSelect(Number::R_XMASK, k0, k);

		// ( 2 * area / depth, ab x ap, bc x bp, ca x cp ) at position ( minX, minY ).
		const R128 f = RegisterAdd(RegisterMultiplyAddMultiply(i, mX, j, mY), k);
		const R128 invDelta = RegisterDivide(Number::R_ONE, MakeRegister(RegisterSum(RegisterSetX0(f))));

		// vertex attribute.
		const R256 attr1 = Register8Multiply(MakeRegister8(l1.x, l1.y, l1.z, n1.x, n1.y, n1.z, uv1.x, uv1.y), MakeRegister8(1.f / v1.w));
		const R256 attr2 = Register8Multiply(MakeRegister8(l2.x, l2.y, l2.z, n2.x, n2.y, n2.z, uv2.x, uv2.y), MakeRegister8(1.f / v2.w));
		const R256 attr3 = Register8Multiply(MakeRegister8(l3.x, l3.y, l3.z, n3.x, n3.y, n3.z, uv3.x, uv3.y), MakeRegister8(1.f / v3.w));


		Shader::FragmentPayload shaderPayload(triangle.material->Diffuse(), viewStateBuffer.location, &pointLightBuffer);

		KahanSum4 fy = f;
		for (int y = minY; y <= maxY; ++y)
		{
			KahanSum4 fx = fy;
			for (int x = minX; x <= maxX; ++x)
			{
				if ((RegisterMaskBits(RegisterGE(fx.Value(), Number::R_ZERO)) & 0x0E) == 0x0E)
				{
					// (1 / depth, gamma, alpha, beta )
					R128 zInverseAndInterpolation = RegisterMultiply(fx.Value(), invDelta);
					const float screenspaceDepth = 1.f / RegisterGetX(zInverseAndInterpolation);

					// Z-depth testing.
					if (const int index = (height - y - 1) * width + x; depthBuffer.GetPixel(index) < screenspaceDepth)
					{
						depthBuffer.SetPixel(index, screenspaceDepth);

						// Interpolate attributes.
						R256 result;
						const R256 depth = MakeRegister8(screenspaceDepth);
						const R256 alpha = Register8Replicate<2>(zInverseAndInterpolation);
						const R256 beta = Register8Replicate<3>(zInverseAndInterpolation);
						const R256 gamma = Register8Replicate<1>(zInverseAndInterpolation);
						result = Register8MultiplyAddMultiply(attr1, alpha, attr2, beta);
						result = Register8MultiplyAdd(attr3, gamma, result);
						result = Register8Multiply(depth, result);
						shaderPayload.SetShadingPoint(&result);

						// execute fragment shader.
						sceneBuffer.SetPixel(index, fragmentShader(shaderPayload));
					}
				}
				fx += i;
			}
			fy += j;
		}
	}
	else
	{
		const float i01 = v1.y - v2.y; // d(ab x ap) / dx
		const float i02 = v2.y - v3.y; // d(bc x bp) / dx
		const float i03 = v3.y - v1.y; // d(ca x cp) / dx

		const float j01 = v2.x - v1.x; // d(ab x ap) / dy
		const float j02 = v3.x - v2.x; // d(bc x bp) / dy
		const float j03 = v1.x - v3.x; // d(ca x cp) / dy

		const float k01 = v1.x * v2.y - v1.y * v2.x;
		const float k02 = v2.x * v3.y - v2.y * v3.x;
		const float k03 = v3.x * v1.y - v3.y * v1.x;

		const float f01 = i01 * (minX)+j01 * (minY)+k01;
		const float f02 = i02 * (minX)+j02 * (minY)+k02;
		const float f03 = i03 * (minX)+j03 * (minY)+k03;
		const float delta = f01 + f02 + f03; // 2 * area

		Vector4 inc0(f01, f02, f03, 0);
		Vector4 incX(i01, i02, i03, 0);
		Vector4 incY(j01, j02, j03, 0);

		Shader::FragmentPayload shaderPayload(triangle.material->Diffuse(), viewStateBuffer.location, &pointLightBuffer);
		for (int y = minY; y <= maxY; ++y)
		{
			Vector4 inc = inc0;
			for (int x = minX; x <= maxX; ++x)
			{
				if (inc.x >= 0 && inc.y >= 0 && inc.z >= 0)
				{
					const int index = (height - y - 1) * width + x;
					float alpha = inc.y / delta;
					float beta = inc.z / delta;
					float gamma = (1 - alpha - beta);
					alpha /= v1.w;
					beta /= v2.w;
					gamma /= v3.w;

					float zInverse = 1.f / (alpha + beta + gamma);
					//float zp = (alpha * v1.z + beta * v2.z + gamma * v3.z) * zInverse;
					const float& screenspaceDepth = zInverse;

					// Z-depth testing.
					if (depthBuffer.GetPixel(index) < screenspaceDepth)
					{
						depthBuffer.SetPixel(index, screenspaceDepth);

						// Interpolate attributes.
						shaderPayload.shadingpoint.position = Math::Interpolate(alpha, beta, gamma, l1, l2, l3) * zInverse;
						shaderPayload.shadingpoint.normal = Math::Interpolate(alpha, beta, gamma, n1, n2, n3) * zInverse;
						shaderPayload.shadingpoint.uv = Math::Interpolate(alpha, beta, gamma, uv1, uv2, uv3) * zInverse;

						// execute fragment shader.
						sceneBuffer.SetPixel(index, fragmentShader(shaderPayload));
					}
				}
				inc += incX;
			}
			inc0 += incY;
		}
	}
}