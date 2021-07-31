#include <Utility/Math.hpp>



#ifndef COLOR_INL_COLOR_IMPL
#define COLOR_INL_COLOR_IMPL

	inline Color Color::FromLinearVector(const float& linearR, const float& linearG, const float& linearB, const float& linearA)
	{
		return
		{
			static_cast<unsigned char>(Math::Clamp(0.f, linearR * 255, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, linearG * 255, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, linearB * 255, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, linearA * 255, 255.f))
		};
	}

	inline Color Color::FromLinearVector(const float& linearR, const float& linearG, const float& linearB)
	{
		return
		{
			static_cast<unsigned char>(Math::Clamp(0.f, linearR * 255, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, linearG * 255, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, linearB * 255, 255.f)),
			255
		};
	}

	force_inline Color Color::FromLinearVector(const Vector4& linearColor)
	{
		return FromLinearVector(linearColor.x, linearColor.y, linearColor.z, linearColor.w);
	}

	force_inline Color Color::FromLinearVector(const Vector3& linearColor)
	{
		return FromLinearVector(linearColor.x, linearColor.y, linearColor.z);
	}

	inline Color Color::FromClampedVector(const Vector4& color)
	{
		return
		{
			static_cast<unsigned char>(color.x),
			static_cast<unsigned char>(color.y),
			static_cast<unsigned char>(color.z),
			static_cast<unsigned char>(color.w)
		};
	}

	inline Color Color::FromClampedVector(const Vector3& color)
	{
		return
		{
			static_cast<unsigned char>(color.x),
			static_cast<unsigned char>(color.y),
			static_cast<unsigned char>(color.z)
		};
	}

	inline Color Color::FromVector(const Vector4& color)
	{
		return
		{
			static_cast<unsigned char>(Math::Clamp(0.f, color.x, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, color.y, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, color.z, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, color.w, 255.f))
		};
	}

	inline Color Color::FromVector(const Vector3& color)
	{
		return
		{
			static_cast<unsigned char>(Math::Clamp(0.f, color.x, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, color.y, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, color.z, 255.f))
		};
	}

#endif // !COLOR_INL_COLOR_IMPL
