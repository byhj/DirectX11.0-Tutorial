#include <windows.h>

int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd )
{
   MessageBox(NULL, L"Hello World!", L"Just another Hello World Program!",
	          MB_ICONEXCLAMATION | MB_OK);

   return 0;
}