#include "FreezeRender.hpp"

#include <Renderer/ParallelRasterizer.hpp>
#include <Core/Camera.hpp>
#include <Loader/Texture/TextureLoaderLibrary.hpp>
#include <Loader/Mesh/MeshLoaderLibrary.hpp>



FreezeRender::FreezeRender()
{
}

FreezeRender::~FreezeRender()
{
}

void FreezeRender::InitScene()
{
	auto& meshBuffer = rasterizer->GetMeshBuffer();
	meshBuffer.clear();

	Meshlet mesh;
	
	const wchar_t* obj = L"../FreezeRender/Test/bull/spot_triangulated_good.OBJ";
	const wchar_t* tex = L"../FreezeRender/Test/bull/spot_texture.png";
	//const wchar_t* obj = L"../FreezeRender/Test/box/box.obj";
	//const wchar_t* tex = L"../FreezeRender/Test/box/Herringbone_2x1.png";
	if (Success(MeshLoaderLibrary::Load(obj, &mesh)))
	{
		const float Angle = Math::Degrees2Radians(140.f);
		const Matrix Rotation =
		{
			{  std::cos(Angle),  0.f,  std::sin(Angle),  0.f },
			{      0.f,          1.f,      0.f,          0.f },
			{ -std::sin(Angle),  0.f,  std::cos(Angle),  0.f },
			{      0.f,          0.f,      0.f,          1.f }
		};
		const Matrix Scale = 2;
		const Matrix& Translate = Matrix::Identity;
		
		mesh.transform = Translate * Rotation * Scale;
		meshBuffer.emplace_back(std::move(mesh));

		auto& material = meshBuffer[0].materials.emplace_back();
		TextureLoaderLibrary::Load(tex, material.ReallocateDiffuse());
	}

	auto& pointLightBuffer = rasterizer->GetPointLightBuffer();
	pointLightBuffer.emplace_back( PointLight{ 500, { 20, 20, 20 } } );
	pointLightBuffer.emplace_back( PointLight{ 500, { -20, 20, 0 } } );
}

HRESULT FreezeRender::HandleCreateEvent(UINT width, UINT height)
{
	rasterizer.reset(new ParallelRasterizer(width, height));
	camera.reset(new Camera(width, height));

	camera->handleUpdated.Bind(&ParallelRasterizer::UpdateViewState, rasterizer.get(), std::placeholders::_1);
	camera->Update();
	InitScene();
	return S_OK;
}

HPAINTRESULT FreezeRender::HandlePaintEvent(const float deltaTime)
{
	ColorRenderTarget& Scene = rasterizer->Draw();
	return { Scene.Data(), (UINT)Scene.Width(), (UINT)Scene.Height() };
}

static int LastX;
static int LastY;
static Rotator rotation;

void FreezeRender::HandleLeftMouseDownEvent(WPARAM nFlags, int x, int y)
{
	LastX = x;
	LastY = y;
	rotation = camera->viewState.rotation;
}

void FreezeRender::HandleMouseMoveEvent(WPARAM nFlags, int x, int y)
{
	if (!appData.bResizing && appData.bLeftMousePressing)
	{
		float offsetX = (LastX - x) * (camera->viewState.fieldOfView / appData.width);
		float offsetY = (LastY - y) * (camera->viewState.fieldOfView / appData.height);

		camera->UpdateRotation((rotation + Rotator(offsetX, offsetY)).Normalize());
	}
}

void FreezeRender::HandleMouseWheelEvent(UINT nFlags, short zDelta, int x, int y)
{
	if (zDelta > 0)
	{
		camera->MoveForword();
	}
	else
	{
		camera->MoveBack();
	}
}

void FreezeRender::HandleResizeEvent(UINT width, UINT height)
{
	rasterizer->Resize(width, height);
	camera->Resize(width, height);
}

void FreezeRender::Tick(const float deltaTime)
{
	if (appData.keys.IsValid())
	{
		if (appData.keys.Match(VK_Q))
			camera->MoveUp();
		if (appData.keys.Match(VK_E))
			camera->MoveDown();

		if (appData.keys.Match(VK_A))
			camera->MoveLeft();
		if (appData.keys.Match(VK_D))
			camera->MoveRight();

		if (appData.keys.Match(VK_W))
			camera->MoveForword();
		if (appData.keys.Match(VK_S))
			camera->MoveBack();
	}
}