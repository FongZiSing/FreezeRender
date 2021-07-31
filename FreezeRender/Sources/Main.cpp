#include "FreezeRender.hpp"


#include <Container/String.hpp>
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
		FreezeRender render;
		if (SUCCEEDED(render.Initialize(hInstance, nCmdShow)))
		{
			// スタープラチナ·ザ・ワールド
			render.Run();
		}
		::CoUninitialize();
	}

	return 0;
}