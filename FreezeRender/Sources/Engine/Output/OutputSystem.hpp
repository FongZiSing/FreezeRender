//
// OutputSystem.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of output system.
//
#pragma once

#include <Common.hpp>
#include <Container/String.hpp>
#include <Pattern/Singleton.hpp>



namespace Pluto
{
	class OutputSystem final
	{
		friend class Engine;

		//~ Begin Draw Text
	public:
		struct Text
		{
			WideString context;
			int offsetX;
			int offsetY;
			int rectWidth;
			int rectHeight;
		};

		void PrintText(Text text);

		//~ End define DrawText

	private:
		template <typename Predicate>
		void PopText(Predicate pred)
		{
			Text text;
			while (PopTextImpl(text))
			{
				pred(std::move(text));
			}
		}

		bool PopTextImpl(Text& text);

		//~ End Draw Text
	};

	extern UniqueResource<OutputSystem> GOutput;
}