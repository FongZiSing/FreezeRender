#pragma once

#include <Container/String.hpp>
#include <memory>
#include <vector>
#include "Texture.hpp"



class Material
{
	using TextureReference = std::unique_ptr<Texture>;
	
	WideString id { L"Native" };

	WideString name { L"Native" };

	TextureReference diffuse { nullptr };

	TextureReference normal { nullptr };

public:		
	inline Texture* Diffuse() const { return diffuse.get(); }
	
	inline Texture* ReallocateDiffuse()
	{
		diffuse = std::make_unique<Texture>();
		return diffuse.get();
	}
};
