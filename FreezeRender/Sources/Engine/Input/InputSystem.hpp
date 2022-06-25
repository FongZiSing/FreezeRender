#pragma once

#include <Math/SIMD.hpp>
#include <Pattern/Singleton.hpp>



class InputSystem final
{
	friend class Engine;

	////////////////////////////////
	//~ Begin Key events.

private:
	union { alignas(32) unsigned __int64 PressingKeys[4] = { 0ull, 0ull, 0ull, 0ull }; };
	union { alignas(32) unsigned __int64 PressedKeys[4] = { 0ull, 0ull, 0ull, 0ull }; };
	union { alignas(32) unsigned __int64 ReleasedKeys[4] = { 0ull, 0ull, 0ull, 0ull }; };

	constexpr void PressedKey(unsigned char k) noexcept
	{
		PressingKeys[k >> 6] |= ( 1ull << (k & 63) );
		PressedKeys[k >> 6] |= ( 1ull << (k & 63) );
	}

	constexpr void ReleasedKey(unsigned char k) noexcept
	{
		PressingKeys[k >> 6] &= ~( 1ull << (k & 63) );
		ReleasedKeys[k >> 6] |= ( 1ull << (k & 63) );
	}

public:
	enum : unsigned char /* Virtual keys. */
	{
		VK_A = 0x41, VK_B = 0x42, VK_C = 0x43, VK_D = 0x44,
		VK_E = 0x45, VK_F = 0x46, VK_G = 0x47, VK_H = 0x48,
		VK_I = 0x49, VK_J = 0x4A, VK_K = 0x4B, VK_L = 0x4C,
		VK_M = 0x4D, VK_N = 0x4E, VK_O = 0x4F, VK_P = 0x50,
		VK_Q = 0x51, VK_R = 0x52, VK_S = 0x53, VK_T = 0x54,
		VK_U = 0x55, VK_V = 0x56, VK_W = 0x57, VK_X = 0x58,
		VK_Y = 0x59, VK_Z = 0x5A
	};

	constexpr bool IsKeyPressing(const unsigned char& k) const noexcept
	{
		return PressingKeys[k >> 6] & ( 1ull << (k & 63) );
	}
	
	constexpr bool IsKeyPressed(const unsigned char& k) const noexcept
	{ 
		return PressingKeys[k >> 6] & ( 1ull << (k & 63) );
	}
	
	constexpr bool IsKeyReleased(const unsigned char& k) const noexcept
	{
		return ReleasedKeys[k >> 6] & ( 1ull << (k & 63) );
	}

	//~ End key events.
	////////////////////////////////



	////////////////////////////////
	//~ Begin Mouse events.

public:
	enum : unsigned char /** Virtual mouse */
	{
		VM_L = 0x0, // left
		VM_M = 0x1, // middle
		VM_R = 0x2, // right
		VM_MAX
	};

private:
	struct
	{
		union
		{
			struct
			{
				unsigned char Pressing : 4;
				unsigned char Pressed  : 4;
				unsigned char Released : 4;
				unsigned char Wheeling : 2;
				unsigned char Moving   : 2;
			};
			unsigned short state = 0;
		};
		
		int x, y, z;
	} Mouses;

	inline void PressedMouse(unsigned char m, int x, int y) noexcept
	{
		m = m % VM_MAX;
		Mouses.Pressed |= ( 1 << m );
		Mouses.Pressing |= ( 1 << m );
		Mouses.x = x;
		Mouses.y = y;
	}
	
	inline void ReleasedMouse(unsigned char m, int x, int y) noexcept
	{
		m = m % VM_MAX;
		Mouses.Released |= ( 1 << m );
		Mouses.Pressing &= ~( 1 << m );
		Mouses.x = x;
		Mouses.y = y;
	}

	constexpr void WheelingMouse(int x, int y, int z) noexcept
	{
		Mouses.Wheeling = 1;
		Mouses.x = x;
		Mouses.y = y;
		Mouses.z = z;
	}

	constexpr void MovingMouse(int x, int y) noexcept
	{
		Mouses.Moving = 1;
		Mouses.x = x;
		Mouses.y = y;
	}

public:
	constexpr bool IsMousePressed(const unsigned char& m) const noexcept
	{
		return Mouses.Pressed & (1 << (m % VM_MAX));
	}

	constexpr bool IsMousePressed(const unsigned char& m, int& x, int& y) const noexcept
	{
		if (Mouses.Pressed & (1 << (m % VM_MAX)))
		{
			x = Mouses.x;
			y = Mouses.y;
			return true;
		}
		return false;
	}

	constexpr bool IsMousePressing(const unsigned char& m) const noexcept
	{
		return Mouses.Pressing & (1 << (m % VM_MAX));
	}

	constexpr bool IsMousePressing(const unsigned char& m, int& x, int& y) const noexcept
	{
		if (Mouses.Pressing & (1 << (m % VM_MAX)))
		{
			x = Mouses.x;
			y = Mouses.y;
			return true;
		}
		return false;
	}

	constexpr bool IsMouseReleased(const unsigned char& m) const noexcept
	{
		return Mouses.Released & (1 << (m % VM_MAX));
	}

	constexpr bool IsMouseReleased(const unsigned char& m, int& x, int& y) const noexcept
	{
		if (Mouses.Released & (1 << (m % VM_MAX)))
		{
			x = Mouses.x;
			y = Mouses.y;
			return true;
		}
		return false;
	}

	constexpr bool IsWheelingMouse(int& x, int& y, int& zDelta) const noexcept
	{
		if (Mouses.Wheeling)
		{
			x = Mouses.x;
			y = Mouses.y;
			zDelta = Mouses.z;
			return true;
		}
		return false;
	}

	constexpr bool IsMovingMouse(int& x, int& y) const noexcept
	{
		if (Mouses.Moving)
		{
			x = Mouses.x;
			y = Mouses.y;
			return true;
		}
		return false;
	}

	//~ End Mouse events.
	////////////////////////////////



	////////////////////////////////
	//~ Begin Window events.

private:
	struct
	{
		unsigned char Resizing : 1 = 0;
		unsigned char Maximum : 1 = 0;
		unsigned char Minimum : 1 = 0;
	} Window;

public:
	constexpr auto& WindowState() const noexcept { return Window; }
	
	//~ End Window events.
	////////////////////////////////

private:
	inline void Reset() noexcept
	{
		Register8SetZero(&PressedKeys);
		Register8SetZero(&ReleasedKeys);
		Mouses.state &= 0x000Fu;
	}

public:
	InputSystem();
	~InputSystem();
};

extern UniqueResource<InputSystem> GInput;