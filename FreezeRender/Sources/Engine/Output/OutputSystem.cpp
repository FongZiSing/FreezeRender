#include "OutputSystem.hpp"
#include <concurrent_queue.h>



namespace Pluto
{
	// TODO move to `Platform` folder.
	Concurrency::concurrent_queue<OutputSystem::Text> textQueue;

	void OutputSystem::PrintText(Text text)
	{
		textQueue.push(std::move(text));
	}

	bool OutputSystem::PopTextImpl(Text& text)
	{
		return textQueue.try_pop(text);
	}
}