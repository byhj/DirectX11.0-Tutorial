#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10.h>

#pragma  comment (lib, "d3d11.lib")
#pragma  comment (lib, "d3dx11.lib")
#pragma  comment (lib, "d3dx10.lib")

//global declarations
IDXGISwapChain *pSwapChain = NULL;
ID3D11Device *pDevice = NULL;
ID3D11DeviceContext *pDeviceContext = NULL;
ID3D11RenderTargetView *backBuffer = NULL;

void Init(HWND hWnd);
void Clean(void);
void Render();

//window callback function
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//Entry point
int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
					_In_ LPWSTR lpCmdLine, _In_ int nShowCmd )
{
	//handle for the window
	HWND hWnd;

	//window information class
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WinProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass";
	RegisterClassEx(&wc);

	//client area 
	RECT wr = {0, 0, 800, 600};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	//create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		L"WindowClass",  //class name
		L"Our First Windowed Program", //title
		WS_OVERLAPPEDWINDOW,   //window style
		0,           //posx
		0,            //posy
		wr.right - wr.left,          //window width
		wr.bottom - wr.top,       //window height
		NULL,       //parent
		NULL,        //menus
		hInstance,  //hinstance
		NULL        //multi window
		);

	ShowWindow(hWnd, nShowCmd);

	Init(hWnd);
	//enter the main loop
	MSG msg;
	while (TRUE)
		{
		//Check to see if any message are waiting in the queue
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
			}
		else 
			{
			  Render();
			}
		}
	Clean();
	return msg.wParam;
	}


LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
	switch (message)
		{
		case WM_DESTROY:
			{
			PostQuitMessage(0);
			return 0;
			}
			break;
		}

	//handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Init(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	//Fill the swap chain description struct
	scd.BufferCount = 1;                                //one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //notice the format !use 32 bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  
	scd.OutputWindow = hWnd;         //Window to render
	scd.SampleDesc.Count = 1;       // MSAA
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;            //Full window

	//create a device, device context and swap chain 
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&pSwapChain,
		&pDevice,
		NULL,
		&pDeviceContext
		);

    //get the address of the back buffer
	ID3D11Texture2D *pBackBuffer = NULL;
	//pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	//use the back buffer address to create the render target
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
	pBackBuffer->Release();
    pDeviceContext->OMSetRenderTargets(1, &backBuffer, NULL);

	//set the viewport
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = 800;
	vp.Height = 600;

	pDeviceContext->RSSetViewports(1, &vp);
}

void Render(void)
{
   pDeviceContext->ClearRenderTargetView(backBuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
   pSwapChain->Present(0, 0);
}

void Clean(void)
{
	pSwapChain->Release();
	pDevice->Release();
	pDeviceContext->Release();
}