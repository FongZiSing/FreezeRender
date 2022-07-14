//
// MeshLoader.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Mesh loader interface.
//
#pragma once

#include "../Meshlet.hpp"



namespace Pluto
{
	struct interface_as MeshLoader
	{
		/**
		 * @brief Specifies status of loading a mesh file.
		 */
		enum class Status : unsigned char
		{
			Ok = 0,
			Uninitialized,
			initFailed,
			initSuccess,
			InvalidInput,
			FileNotExisted,
			FileOpenFailed,
			MeshParseFailed,
			OutOfMemory,

			FormatNotSupported = 128,
		};

		/**
		 * @brief Specifies supported mesh file.
		 */
		enum class Extension : unsigned char
		{
			Unknown = 0,
			OBJ,
		};

		/**
		 * @brief virtual destructor.
		 */
		virtual ~MeshLoader() noexcept = default;

		/**
		 * @brief Determines whether the loading is successful.
		 */
		friend constexpr bool Success(Status status)
		{
			return status == Status::Ok;
		}

		/**
		 * @brief Returns the native path of the specified mesh file.
		 */
		virtual WideString GetNativeName() const = 0;

		/**
		 * @brief Returns the absolute path of the specified mesh file.
		 */
		virtual WideString GetAbsolutePath() const = 0;

		/**
		 * @brief Returns the name of the specified mesh file.
		 */
		virtual WideString GetName() const = 0;

		/**
		 * @brief Returns the type of mesh.
		 */
		virtual Extension GetExtension() const = 0;

		/**
		 * @brief Determine whether the specified mesh file has a valid extension or exists, or everything you like.
		 *        It should be defined as pure function (Referential Transparency Principle) if inherit this class.
		 */
		virtual Status Verify() const = 0;

		/**
		 * @brief Try to load the specified mesh file.
		 */
		virtual Status Load(AMeshlet* result) = 0;
	};
}