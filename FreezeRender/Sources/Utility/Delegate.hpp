#pragma once

#include <functional>



template<typename FunctionDef>
class SingleDelegate
{
	std::function<FunctionDef> privateFunction;

public:
	template <class FunctionDec, class ...Types>
	void Bind(FunctionDec&& func, Types&& ...args)
	{
		privateFunction = std::bind<FunctionDec, Types...>(std::forward<FunctionDec>(func), std::forward<Types>(args)...);
	}

	template <class ReturnDec, class FunctionDec, class ...Types>
	void Bind(FunctionDec&& func, Types&&... args)
	{
		privateFunction = std::bind<ReturnDec, FunctionDec, Types...>(std::forward<FunctionDec>(func), std::forward<Types>(args)...);
	}

	template <class... Types>
	void Execute(Types&&... args)
	{
		if (privateFunction)
		{
			privateFunction(std::forward<Types>(args)...);
		}
	}
};