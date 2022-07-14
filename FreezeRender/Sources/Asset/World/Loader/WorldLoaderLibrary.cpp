#include "WorldLoaderLibrary.hpp"
#include <Asset/Meshlet/Loader/MeshLoaderLibrary.hpp>
#include <Asset/Texture/Loader/TextureLoaderLibrary.hpp>



namespace Pluto
{
	void WorldLoaderLibrary::InitializeDefaultWorld(AWorld& world)
	{
		world.allCamera.Clear();
		world.allMeshlet.Clear();
		world.allPointLight.Clear();

		//--------------------------------
		//~ Init camera.
		//--------------------------------
		world.allCamera.Emplace();



		//--------------------------------
		//~ Init meshlet.
		//--------------------------------
		AMeshlet& mesh = world.allMeshlet.Emplace();
		//const wchar_t* obj = L"../FreezeRender/Test/bull/spot_triangulated_good.OBJ";
		//const wchar_t* tex = L"../FreezeRender/Test/bull/spot_texture.png";
		const wchar_t* obj = L"../FreezeRender/Test/box/box.obj";
		const wchar_t* tex = L"../FreezeRender/Test/box/Herringbone_2x1.png";
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

			auto& material = mesh.materials.Emplace();
			material.diffuse = std::make_unique<ATexture>();
			TextureLoaderLibrary::Load(tex, material.diffuse.get());

			auto& cluster = mesh.clusters.Emplace();
			cluster.beginVertexIndex = 0;
			cluster.endVertexIndex = cluster.beginVertexIndex + (unsigned int)mesh.indices.Size();
			cluster.triangleNumber = (cluster.endVertexIndex - cluster.beginVertexIndex) / 3;
			cluster.materialIndex = 0;
		}



		//--------------------------------
		//~ Init light.
		//--------------------------------
		world.allPointLight.Emplace(APointLight{ 500, { 20, 20, 20 } });
		world.allPointLight.Emplace(APointLight{ 500, { -20, 20, 0 } });
	}
}