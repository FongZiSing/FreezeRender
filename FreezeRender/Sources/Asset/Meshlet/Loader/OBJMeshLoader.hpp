//
// OBJMeshLoader.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Utility of load OBJ mesh.
//
#pragma once

#include "MeshLoader.hpp"
#include <filesystem>



namespace Pluto
{
	/**
	 * @biref Load meshlet according to the specified .obj file.
	 */
	class OBJMeshLoader final : public MeshLoader
	{
		// Store the last result of operation.
		Status status = Status::Uninitialized;

		// Store the mesh file path.
		std::filesystem::path filepath;

	public:

		//--------------------------------
		//~ Begin initialize.
		
		OBJMeshLoader(const std::filesystem::path& filepath) : filepath(filepath) { this->status = Verify(); }

		OBJMeshLoader(std::filesystem::path&& filepath) : filepath(std::move(filepath)) { this->status = Verify(); }

		OBJMeshLoader(const wchar_t* const filepath) noexcept : filepath(filepath) { this->status = Verify(); };
		
		//~ End initialize.
		//--------------------------------


		//--------------------------------
		//~ Begin inline function.
		
		WideString GetNativeName() const override { return filepath.native(); }

		WideString GetAbsolutePath() const override { return std::filesystem::canonical(filepath).wstring(); }

		WideString GetName() const override { return filepath.filename().wstring(); }

		Extension GetExtension() const override { return Extension::OBJ; }
		
		//~ End inline function.
		//--------------------------------


		Status Verify() const override;

		Status Load(AMeshlet* result) override;
	};
}