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
		}
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);

		printf("- Window closed -");
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
			dll_Release(hD3D);
			dll_Unload(hDll);
		}

		
		void initalize(
			const char * Title,
			unsigned int X,
			unsigned int Y,
			unsigned int Width,
			unsigned int Height) {

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


			CreatWindowClass();
			MakeWindow(dll_Info(), X, Y, Width, Height);

#if defined _DEBUG
			CreateConsole();
#endif

			ShowWindow(hWnd,SW_SHOW);
			UpdateWindow(hWnd);

			dll_CreateGraphics(hD3D, hWnd);

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

					if (x > 0)
						y = x += 0.00001f;


					Vertex_buffer v[] = {
						{ Vec3{-0.5f / x,  0.5f / y, 0.0f}, Vec4{1.0f, 0.0f, 0.0f, 1.0f} }, // top left
						{ Vec3{ 1.0f / x, -1.0f / y, 0.0f}, Vec4{0.0f, 1.0f, 0.0f, 1.0f} }, // bottom right
						{ Vec3{-1.0f / x, -1.0f / y, 0.0f}, Vec4{0.0f, 0.0f, 1.0f, 1.0f} }, // bottom left
						{ Vec3{ 0.5f / x,  0.5f / y, 0.0f}, Vec4{0.0f, 0.0f, 0.0f, 1.0f} }, // top right
					};
					WORD i[] =
					{
						0,3,2,
						2,3,1,
					};


					Vertex_buffer v2[] = {
						{ Vec3{-1.0f,  1.0f, 0.0f }, Vec4{ 1.0f, 1.0f, 1.0f, 1.0f } }, // top left
						{ Vec3{ 0.0f,  1.0f, 0.0f }, Vec4{ 1.0f, 0.0f, 0.0f, 1.0f } }, // top right
						{ Vec3{-1.0f,  0.0f, 0.0f }, Vec4{ 1.0f, 0.0f, 0.0f, 1.0f } }, // bottom left
					};
					WORD i2[] =
					{
						0,1,2,
					};


					// index goes clockwise



					Buffer b[] = {
						{ v,  4, i,  6 },
						{ v2, 4, i2, 3 },
					};

					dll_Draw(hD3D,b,2,clear_color);
					dll_Update(hD3D);
				}
			}
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