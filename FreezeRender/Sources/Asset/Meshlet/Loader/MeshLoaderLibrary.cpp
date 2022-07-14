#include "MeshLoaderLibrary.hpp"
#include <Asset/Utility/LoaderUitlity.hpp>
#include "OBJMeshLoader.hpp"



namespace Pluto
{
#pragma region utility

	inline without_globalvar MeshLoader::Extension GetExtension(const std::filesystem::path& filepath)
	{
		if (IsExtesionEqual(filepath, L".obj"))
		{
			return MeshLoader::Extension::OBJ;
		}
		// TODO: Support more format.

		return MeshLoader::Extension::Unknown;
	};

#pragma endregion utility



	MeshLoader::Status MeshLoaderLibrary::Load(const wchar_t* const filename, AMeshlet* result)
	{
		const std::filesystem::path filepath = filename;
		switch (GetExtension(filepath))
		{
		case MeshLoader::Extension::OBJ:
		{
			OBJMeshLoader loader(filepath);
			return loader.Load(result);
		}
		// TODO: Support more format.
		}

		return MeshLoader::Status::FormatNotSupported;
	}
}
