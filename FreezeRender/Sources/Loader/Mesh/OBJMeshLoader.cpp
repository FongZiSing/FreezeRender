#include "OBJMeshLoader.hpp"

#include <Core/Meshlet.hpp>
#include <Utility/Math.hpp>
#include <string_view>
#include <fstream>
#include <array>
#include "../LoaderUitlity.hpp"



/**
 * @todo
 *    1. faster number parser than standard liabrary. std::strtof(), std::strtoul()
 *    2. faster file iostream.
 */
namespace Detail
{
	namespace Loader
	{
		template <class Type> using Array = std::vector<Type>;
		template <class Type, size_t Size> using ConstArray = std::array<Type, Size>;

		// The components of vertex index.
		struct alignas(16) VertexAttributeIndex
		{
			unsigned int position = UINT_MAX;
			unsigned int uv = UINT_MAX;
			unsigned int normal = UINT_MAX;
			unsigned int reserved = UINT_MAX;
		};

		// Splits polygon index to multiple triangle index.
		struct VertexAssembler
		{
			Array<unsigned int>& output;
			ConstArray<unsigned int, 4> buffer = { INT_MAX, INT_MAX, INT_MAX, INT_MAX };
			int prev = 0;
			int last = 0;
			bool bHasThree = false;

			VertexAssembler(Array<unsigned int>& positionIndices) : output(positionIndices) {}
			void push(const unsigned int& index)
			{
				buffer[last] = index;
				last = (last + 1) & 0x3;
				if (bHasThree || ((last - prev + 4) & 0x3) == 0x3)
				{
					bHasThree = true;
					output.push_back(buffer[prev]);
					prev = (prev + 1) & 0x3;
					output.push_back(buffer[prev]);
					output.push_back(index);
				}
			}
		};

		// Splits given number string.
		inline void Split(const StringView& src, char delims, Array<StringView>& out)
		{
			size_t head = 0, tail = 0;
			const std::string_view& in = src.Native();
			while (head < in.size())
			{
				tail = in.find_first_of(delims, head);
				if (head != tail) out.emplace_back(in.substr(head, tail - head));
				if (head == tail) out.emplace_back();
				if (tail == std::string_view::npos) break;
				head = tail + 1;
			}

			if (tail != std::string_view::npos)
			{
				out.emplace_back();
			}
		}

		// Splits given number string.
		inline void Split(const StringView& src, char delims, Array<StringView>& out, size_t size, bool& bBroken)
		{
			size_t head = 0, tail = 0;
			const std::string_view& in = src.Native();
			while (head < in.size())
			{
				tail = in.find_first_of(delims, head);
				if (head != tail) out.emplace_back(in.substr(head, tail - head));
				if (head == tail) { out.emplace_back("-1"); bBroken |= true; }
				if (tail == std::string_view::npos) break;
				head = tail + 1;
			}

			if (tail != std::string_view::npos)
			{
				out.emplace_back("-1");
				bBroken |= true;
			}

			while (out.size() < size)
			{
				out.emplace_back("-1");
				bBroken |= true;
			}
		}

		// This function is only focus on parsing mesh file, manually clean up the wrong data if parse failed.
		OBJMeshLoader::Status Load(std::ifstream* pStream, Meshlet* result)
		{
			using Status = OBJMeshLoader::Status;

			std::ifstream& stream = *pStream;
			Meshlet& mesh = *result;

			// All supported data.
			Array<Array<VertexAttributeIndex>> polygons;
			polygons.reserve(128);

			Array<Vector3> positions;
			Array<Vector3> normals;
			Array<Vector2> uvs;
			positions.reserve(128);
			normals.reserve(128);
			uvs.reserve(128);

			String line;
			while (std::getline(stream, line.Native()))
			{
				// Ignore empty line.
				if (line.IsEmpty())
				{
					continue;
				}

				// Remove `\space` and `\tab`.
				StringView substring = line.Trim();

				// Ignore comment.
				if (substring.StartWith("#"))
				{
					continue;
				}

				// TODO: material system.
				if (substring.StartWith("mtllib", false))
				{
					continue;
				}

				// Parse vertex buffer.
				Array<StringView> part;
				Split(substring, ' ', part);
				if (part[0][0] == 'v')
				{
					if (part.size() != 3 && part.size() != 4 && part.size() != 7)
					{
						return Status::MeshParseFailed;
					}

					// Parse "v", that specifies vertex position.
					if (part[0].Size() == 1)
					{
						positions.emplace_back(
							part[1].ToFloat(),
							part[2].ToFloat(),
							part[3].ToFloat()
						);

						// that specifies vertex color.
						if (part.size() == 7)
						{
							// TODO
						}
					}
					// Parse "vn", that specifies vertex normal.
					else if (part[0][1] == 'n')
					{
						normals.emplace_back(
							part[1].ToFloat(),
							part[2].ToFloat(),
							part[3].ToFloat()
						);
					}
					// Parse "vt", that specifies vertex texture-coords.
					else if (part[0][1] == 't')
					{
						uvs.emplace_back(
							part[1].ToFloat(),
							part[2].ToFloat()
						);
					}
				}
				// Parse index buffer.
				else if (part[0] == "f")
				{
					// Unknown tag, or less than three vertex cannot form a polygon.
					if (part[0].Size() != 1 || part.size() < 4)
					{
						continue;
					}

					Array<VertexAttributeIndex>& wedge = polygons.emplace_back();
					bool bBroken = false;

					// Parse index of vertex postion, coordinate, and normal.
					for (size_t i = 1; i < part.size(); ++i)
					{
						Array<StringView> details;
						Split(part[i], '/', details, 3, bBroken);

						VertexAttributeIndex& index = wedge.emplace_back();
						index.position = details[0].ToUint() - 1;
						index.uv = details[1].ToUint() - 1;
						index.normal = details[2].ToUint() - 1;
					}

					// Missing index.
					if (bBroken)
					{
						return Status::MeshParseFailed;
					}
				}
			}

			// Without any polygon.
			if (polygons.empty())
			{
				return Status::MeshParseFailed;
			}

			// Map the vertices at the same position
			//    to that one with different attributes(uv, texture coordinate, normal etc...)
			Array<Array<unsigned int>> remap;
			remap.resize(positions.size());

			// Target mesh data.
			Array<unsigned int> positionIndices;
			Array<Vertex> vertices;
			positionIndices.reserve(polygons.size() * polygons[0].size());
			vertices.reserve(positions.size());

			// triangle assembly.
			for (const Array<VertexAttributeIndex>& wedges : polygons)
			{
				VertexAssembler assembler(positionIndices);
				for (const VertexAttributeIndex& corner : wedges)
				{
					Vertex vertex;
					vertex.position = positions[corner.position];
					vertex.uv = uvs[corner.uv];
					vertex.normal = normals[corner.normal];

					// Determines whether the vertex exists.
					bool bMatch = false;
					Array<unsigned int>& remapper = remap[corner.position];
					for (const unsigned int& index : remapper)
					{
						// If it exists, only the index buffer is considered.
						const Vertex& target = vertices[index];
						if (target == vertex)
						{
							assembler.push(index);
							bMatch = true;
							break;
						}
					}

					// Otherwise, allocate a new vertex and index, and cache the index in `remap` array.
					if (!bMatch)
					{
						const unsigned int index = static_cast<unsigned int>(vertices.size());
						vertices.push_back(std::move(vertex));
						assembler.push(index);
						remapper.push_back(index);
					}
				}
			}

			// Fill meshlet.
			mesh.vertices.swap(vertices);
			Vertex* vertex = mesh.vertices.data();
			for (auto& index : positionIndices)
			{
				mesh.indices.push_back({ index, vertex + index });
			}
			return Status::Ok;
		}
	}
}


OBJMeshLoader::Status OBJMeshLoader::Verify() const
{
	if (filepath.empty())
	{
		return Status::initFailed;
	}

	if (!std::filesystem::exists(filepath))
	{
		return Status::initFailed;
	}

	if (!IsExtesionEqual(filepath, L".obj"))
	{
		return Status::initFailed;
	}
	return Status::initSuccess;
}

OBJMeshLoader::Status OBJMeshLoader::Load(Meshlet* result)
{
	if (status == Status::initFailed)
	{
		return status;
	}

	if (result == nullptr)
	{
		status = Status::InvalidInput;
		return status;
	}

	std::ifstream file(filepath.native());
	if (!file.is_open())
	{
		status = Status::FileOpenFailed;
		return status;
	}

	status = Detail::Loader::Load(&file, result);
	if (status != Status::Ok)
	{
		decltype(result->vertices)().swap(result->vertices);
		decltype(result->indices)().swap(result->indices);
	}
	else
	{
		// Generate naive mesh information.
		static std::atomic<long long> id = 0;
		result->id = WideStringView(L"OBJLoaderMesh_").Append(GenerateId(id));
		result->name = filepath.filename();
	}

	return status;
}