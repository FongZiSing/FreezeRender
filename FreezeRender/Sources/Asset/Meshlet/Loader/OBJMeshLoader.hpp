#pragma once

#include "MeshLoader.hpp"
#include <filesystem>



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
	/// Initialize.
	OBJMeshLoader(const std::filesystem::path& filepath) : filepath(filepath) { this->status = Verify(); }

	OBJMeshLoader(std::filesystem::path&& filepath) : filepath(std::move(filepath)) { this->status = Verify(); }

	OBJMeshLoader(const wchar_t* const filepath) noexcept : filepath(filepath) { this->status = Verify(); };
	/// Initialize.

	/// Inline function.
	WideString GetNativeName() const override { return filepath.native(); }

	WideString GetAbsolutePath() const override { return std::filesystem::canonical(filepath).wstring(); }

	WideString GetName() const override { return filepath.filename().wstring(); }

	Extension GetExtension() const override { return Extension::OBJ; }
	/// Inline function.
	
	Status Verify() const override;

	Status Load(Meshlet* result) override;
};