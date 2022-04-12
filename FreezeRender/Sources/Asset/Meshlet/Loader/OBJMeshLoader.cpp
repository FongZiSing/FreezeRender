#include "OBJMeshLoader.hpp"
#include <Asset/Utility/LoaderUitlity.hpp>
#include <Math/Math.hpp>
#include <fstream>



/**
 * @todo
 *    1. faster number parser than standard liabrary. std::strtof(), std::strtoul()
 *    2. faster file iostream.
 */
namespace Detail
{
	namespace Loader
	{
		// The components of vertex index.
		struct alignas(16) VertexAttributeIndex
		{
			unsigned int position = UINT_MAX;
			unsigned int uv       = UINT_MAX;
			unsigned int normal   = UINT_MAX;
			unsigned int reserved = UINT_MAX;
		};

		// Splits polygon index to multiple triangle index.
		struct VertexAssembler
		{
			Array<unsigned int>& output;
			StaticArray<unsigned int, 4> buffer = { INT_MAX, INT_MAX, INT_MAX, INT_MAX };
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
					output.Push(buffer[prev]);
					prev = (prev + 1) & 0x3;
					output.Push(buffer[prev]);
					output.Push(index);
				}
			}
		};

		// Splits given number string.
		inline void Split(const StringView& src, char delims, Array<StringView>& out)
		{
			size_t head = 0, tail = 0;
			const std::string_view& in = *src;
			while (head < in.size())
			{
				tail = in.find_first_of(delims, head);
				if (head != tail) out.Emplace(in.substr(head, tail - head));
				if (head == tail) out.Emplace();
				if (tail == std::string_view::npos) break;
				head = tail + 1;
			}

			if (tail != std::string_view::npos)
			{
				out.Emplace();
			}
		}

		// Splits given number string.
		inline void Split(const StringView& src, char delims, Array<StringView>& out, size_t size, bool& bBroken)
		{
			size_t head = 0, tail = 0;
			const std::string_view& in = *src;
			while (head < in.size())
			{
				tail = in.find_first_of(delims, head);
				if (head != tail) out.Emplace(in.substr(head, tail - head));
				if (head == tail) { out.Emplace("-1"); bBroken |= true; }
				if (tail == std::string_view::npos) break;
				head = tail + 1;
			}

			if (tail != std::string_view::npos)
			{
				out.Emplace("-1");
				bBroken |= true;
			}

			while (out.Size() < size)
			{
				out.Emplace("-1");
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
			polygons.Reserve(128);

			Array<Vector3> positions;
			Array<Vector3> normals;
			Array<Vector2> uvs;
			positions.Reserve(128);
			normals.Reserve(128);
			uvs.Reserve(128);

			String line;
			while (std::getline(stream, *line))
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
					if (part.Size() != 3 && part.Size() != 4 && part.Size() != 7)
					{
						return Status::MeshParseFailed;
					}

					// Parse "v", that specifies vertex position.
					if (part[0].Size() == 1)
					{
						positions.Emplace(
							part[1].ToFloat(),
							part[2].ToFloat(),
							part[3].ToFloat()
						);

						// that specifies vertex color.
						if (part.Size() == 7)
						{
							// TODO
						}
					}
					// Parse "vn", that specifies vertex normal.
					else if (part[0][1] == 'n')
					{
						normals.Emplace(
							part[1].ToFloat(),
							part[2].ToFloat(),
							part[3].ToFloat()
						);
					}
					// Parse "vt", that specifies vertex texture-coords.
					else if (part[0][1] == 't')
					{
						uvs.Emplace(
							part[1].ToFloat(),
							part[2].ToFloat()
						);
					}
				}
				// Parse index buffer.
				else if (part[0] == "f")
				{
					// Unknown tag, or less than three vertex cannot form a polygon.
					if (part[0].Size() != 1 || part.Size() < 4)
					{
						continue;
					}

					Array<VertexAttributeIndex>& wedge = polygons.Emplace();
					bool bBroken = false;

					// Parse index of vertex postion, coordinate, and normal.
					for (size_t i = 1; i < part.Size(); ++i)
					{
						Array<StringView> details;
						Split(part[i], '/', details, 3, bBroken);

						VertexAttributeIndex& index = wedge.Emplace();
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
			if (polygons.IsEmpty())
			{
				return Status::MeshParseFailed;
			}

			// Map the vertices at the same position
			//    to that one with different attributes(uv, texture coordinate, normal etc...)
			Array<Array<unsigned int>> remap;
			remap.Resize(positions.Size());

			// Target mesh data.
			Array<unsigned int> positionIndices;
			Array<Vertex> vertices;
			positionIndices.Reserve(polygons.Size() * polygons[0].Size());
			vertices.Reserve(positions.Size());

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
						const unsigned int index = static_cast<unsigned int>(vertices.Size());
						vertices.Push(std::move(vertex));
						assembler.push(index);
						remapper.Push(index);
					}
				}
			}

			// Fill meshlet.
			mesh.vertices.Swap(vertices);
			Vertex* vertex = mesh.vertices.Data();
			for (auto& index : positionIndices)
			{
				mesh.indices.Push({ index, vertex + index });
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
		decltype(result->vertices)().Swap(result->vertices);
		decltype(result->indices)().Swap(result->indices);
	}
	else
	{
		// Generate naive mesh information.
		static std::atomic<long long> id = 0;
		result->id = WideString(L"OBJLoaderMesh_").Append(GenerateId(id));
		result->name = filepath.filename();
	}

	return status;
}