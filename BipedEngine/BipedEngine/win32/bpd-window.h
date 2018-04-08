#ifndef _BPD_WINDOW_H_
#define _BPD_WINDOW_H_

#ifdef BPD_ONLY_D3D12
#define BPD_NO_D3D11
#define BPD_NO_D3D9
#define BPD_NO_OPENGL
#define BPD_NO_VULKAN
#endif

#ifdef BPD_ONLY_D3D11
#define BPD_NO_D3D12
#define BPD_NO_D3D9
#define BPD_NO_OPENGL
#define BPD_NO_VULKAN
#endif

#ifdef BPD_ONLY_D3D9
#define BPD_NO_D3D12
#define BPD_NO_D3D11
#define BPD_NO_OPENGL
#define BPD_NO_VULKAN
#endif

#ifdef BPD_ONLY_VULKAN
#define BPD_NO_D3D12
#define BPD_NO_D3D11
#define BPD_NO_D3D9
#define BPD_NO_OPENGL
#endif

#ifdef BPD_ONLY_OPENGL
#define BPD_NO_D3D12
#define BPD_NO_D3D11
#define BPD_NO_D3D9
#define BPD_NO_VULKAN
#endif




LRESULT CALLBACK WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	PAINTSTRUCT ps;
	bpd::dll::HBPDD3D hD3D;

	switch(uMsg) {
	case WM_PAINT:
	{

		//dll_PassBuffers(hD3D,NULL,NULL);
		//dll_UpdateGraphics(hD3D, NULL);

		HDC hdc = BeginPaint(hwnd,&ps);
		EndPaint(hwnd,&ps);
		break;
	}
	case WM_CHAR:
		if(wParam == VK_ESCAPE) {
			DestroyWindow(hwnd);
			FreeConsole();
		}
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcA(hwnd,uMsg,wParam,lParam);
}


namespace bpd{
	class Window {
	public:
		Window(){
			y = x = 1;

		}
		~Window(){
			
		}

		
		void initalize(
			const char * Title,
			unsigned int X,
			unsigned int Y,
			unsigned int Width,
			unsigned int Height) {
#if defined _WIN64
#if !defined BPD_NO_D3D12
			if(dll_Load(hDll,"dll_d3d12_x64.dll") == false)
#endif
#if !defined BPD_NO_D3D11
				if(dll_Load(hDll,"dll_d3d11_x64.dll") == false)
#endif
#if !defined BPD_NO_D3D9
					if(dll_Load(hDll,"dll_d3d9_x64.dll") == false)
#endif
#if !defined BPD_NO_VULKAN
						if(dll_Load(hDll,"dll_vulklan_x64.dll") == false)
#endif
#if !defined BPD_NO_OPENGL
							if(dll_Load(hDll,"dll_opengl_x64.dll") == false)
#endif
							{
								ErrMsg("Could not load dll's");
								return;
							}
#else
#if !defined BPD_NO_D3D12
			if(dll_Load(hDll, "dll_d3d12.dll") == false)
#endif
#if !defined BPD_NO_D3D11
				if(dll_Load(hDll, "dll_d3d11.dll") == false)
#endif
#if !defined BPD_NO_D3D9
					if(dll_Load(hDll, "dll_d3d9.dll") == false)
#endif
#if !defined BPD_NO_VULKAN
						if (dll_Load(hDll, "dll_vulklan.dll") == false)
#endif
#if !defined BPD_NO_OPENGL
							if (dll_Load(hDll, "dll_opengl.dll") == false)
#endif
							{
								ErrMsg("Could not load dll's");
								return;
							}
#endif


			CreatWindowClass();
			MakeWindow(dll_Info(), X, Y, Width, Height);

#if defined _DEBUG
			CreateConsole();
#endif

			ShowWindow(hWnd,SW_SHOW);
			UpdateWindow(hWnd);

			dll_CreateGraphics(hD3D, hWnd);
			dll_ReloadBuffers(hD3D, cubeBuffer, 1);
			dll_SetView(hD3D,Vec4{ 0.0f, 3.0f, -8.0f, 0.0f },Vec4{ 0.0f, 0.0f, 0.0f, 0.0f },60.0f);

			dll_CreateObj(hD3D,Vec4{ 0.0f, 0.0f, 0.0f, 0.0f });
			dll_CreateObj(hD3D,Vec4{ 3.0f, 0.0f, 0.0f, 0.0f });
			dll_CreateObj(hD3D,Vec4{-3.0f, 0.0f, 0.0f, 0.0f });

		}

		void CreatWindowClass(){
			ZeroMemory(&windowClass,sizeof(WNDCLASS));

			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			windowClass.hCursor = LoadCursor(NULL,IDC_ARROW);
			windowClass.hInstance = hInstance;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = sizeof(void*);
			windowClass.lpfnWndProc = WndProc;
			windowClass.lpszMenuName = NULL;
			windowClass.lpszClassName = "BpdClassWindow";
			windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
			RegisterClass(&windowClass);
		}

		void MakeWindow(const char * Title, int X, int Y, int Width, int Height) {
			hWnd = CreateWindow(
				"BpdClassWindow",Title,
				WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				X,Y,Width,Height,
				NULL,NULL,hInstance,NULL
			);
			Check_(hWnd,"Failed to create window");
		}

		void RunWindow(){
			while((&msg)->message != WM_QUIT) {
				if(PeekMessage(&msg,hWnd,0,0,PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				} else {
					float clear_color[] = { 0.0f,0.0f,0.0f,1.0f };

					//dll_SetView(hD3D,Vec4{ 0.0, 3.0f, -8.0f, 0.0f },Vec4{ 0.0f, 0.0f, 0.0f, 0.0f },60.0f);
					
					//dll_TransformObj(hD3D,0,Vec4{ 0.0f, 0.0f, 0.0f, 0.0f },Vec4{ 0.01f, 0.03f, 0.02f, 0.0f },Vec4{ 1.0f, 1.0f, 1.0f, 0.0f });
					//dll_TransformObj(hD3D,1,Vec4{ 3.0f, 0.0f, 0.0f, 0.0f },Vec4{ 0.00f, 0.00f, 0.00f, 0.0f },Vec4{ 1.0f, 1.0f, 1.0f, 0.0f });
					//dll_TransformObj(hD3D,2,Vec4{-3.0f, 0.0f, 0.0f, 0.0f },Vec4{ 0.00f, 0.00f, 0.00f, 0.0f },Vec4{ 1.0f, 1.0f, 1.0f, 0.0f });

					dll_Update(hD3D);
					dll_Draw(hD3D,cubeBuffer,3,clear_color);
				}
			}

			dll_Release(hD3D);
			dll_Unload(hDll);
		}

		void CreateConsole(){
			AllocConsole();
			AttachConsole(GetCurrentProcessId());
			freopen("CONIN$","r",stdin);
			freopen("CONOUT$","w",stdout);
			freopen("CONOUT$","w",stderr);

			printf("Graphics Info: %s\n", dll_Info());
		}
		
		bool running;


	private:
		MSG msg;
		HWND hWnd;
		WNDCLASS windowClass;
		HINSTANCE hDll;
		HINSTANCE hInstance;
		dll::HBPDD3D hD3D;

		float x;
		float y;
	};
}

#endif