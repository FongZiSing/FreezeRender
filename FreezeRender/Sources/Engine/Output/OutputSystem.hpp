#pragma once

#include <Common.hpp>



class OutputSystem final
{
	friend class Engine;

	//~ Begin Draw Text
public:
	struct Text
	{
		const wchar_t* data;
		int length;
		int offsetX;
		int offsetY;
		int rectWidth;
		int rectHeight;
	};

	void PushText(const Text& text);

	decltype(PushText) DrawText = PushText;

private:
	void PopText();

	//~ End Draw Text
};