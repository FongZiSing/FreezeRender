#include "Meshlet.hpp"



namespace Pluto
{
	void Meshlet::BuildBVH()
	{
		bvh.Build(data);
	}
}
