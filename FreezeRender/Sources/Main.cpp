#include "Engine.hpp"



/**
 * @brief Start your journey of FreezeRender.
 */
int APIENTRY wWinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPWSTR    lpCmdLine,
	_In_     int       nCmdShow
)
{
	::HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);
	if (SUCCEEDED(::CoInitialize(nullptr)))
	{
		Engine FreezeRender;
		if (SUCCEEDED(FreezeRender.Initialize(hInstance, nCmdShow)))
		{
			// スタープラチナ·ザ・ワールド
			FreezeRender.Run();
		}
		::CoUninitialize();
	}

	return 0;
}