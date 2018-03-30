#ifndef _BPD_WINDOW_H_
#define _BPD_WINDOW_H_

LRESULT CALLBACK WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	PAINTSTRUCT ps;


	switch(uMsg) {
	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hwnd,&ps);

		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcA(hwnd,uMsg,wParam,lParam);
}

namespace bpd{
	class Window {
	public:
		Window(){

		}
		~Window(){
			dll_Unload(hDll);
		}

		
		void initalize(
			const char * Title,
			unsigned int X,
			unsigned int Y,
			unsigned int Width,
			unsigned int Height) {

#if !defined BPD_NO_D3D
#if !defined BPD_NO_D3D12
			if(dll_Load(hDll, "dll_d3d12.dll") == false)
#endif
#if !defined BPD_NO_D3D11
				if(dll_Load(hDll, "dll_d3d11.dll") == false)
#endif
#if !defined BPD_NO_D3D9
					if(dll_Load(hDll, "dll_d3d9.dll") == false)
#endif
					{
						ErrMsg("Could not load DirectX dll");
						return;
					}
#else
#if !defined BPD_NO_OPENGL
			if (dll_Load(hDll, "dll_vulklan.dll") == false)
#endif
#if !defined BPD_NO_VULKAN
				if (dll_Load(hDll, "dll_opengl.dll") == false)
#endif
				{
					ErrMsg("Could not load open graphics dll");
					return;
				}
#endif

			CreatWindowClass();

			hWnd = CreateWindow(
				"BpdClassWindow", "Title",
				WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				X,Y,Width,Height,
				NULL,NULL,hInstance,NULL
			);
			Check_(hWnd,"Failed to create window");

			dll_CreateGraphics(hD3D, hWnd);

			ShowWindow(hWnd,SW_SHOW);

#if defined _DEBUG
			CreateConsole();
#endif
		}

		void CreatWindowClass(){
			ZeroMemory(&windowClass,sizeof(WNDCLASS));

			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			windowClass.hCursor = LoadCursor(NULL,IDC_ARROW);
			windowClass.hInstance = hInstance;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.lpfnWndProc = WndProc;
			windowClass.lpszMenuName = NULL;
			windowClass.lpszClassName = "BpdClassWindow";
			windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
			RegisterClass(&windowClass);
		}

		void RunWindow(){
			while(msg.message != WM_QUIT) {
				if(PeekMessage(&msg,hWnd,0,0,PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				} else {
					float clear_color[] = { 0.0f,0.0f,0.0f,1.0f };

					Vertex_buffer v[] = {
						{ Vec3{-0.5f,  0.5f, 0.0f}, Vec4{1.0f, 0.0f, 0.0f, 1.0f} }, // top left
						{ Vec3{ 1.0f, -1.0f, 0.0f}, Vec4{0.0f, 1.0f, 0.0f, 1.0f} }, // bottom right
						{ Vec3{-1.0f, -1.0f, 0.0f}, Vec4{0.0f, 0.0f, 1.0f, 1.0f} }, // bottom left
						{ Vec3{ 0.5f,  0.5f, 0.0f}, Vec4{1.0f, 1.0f, 1.0f, 1.0f} }, // top right
					};

					WORD i[] =
					{
						0,3,2,
						2,3,1,
					};

					dll_PassBuffers(hD3D,v,4,i,6);
					dll_UpdateGraphics(hD3D, clear_color, 12);
				}
			}
		}

		void CreateConsole(){
			AllocConsole();
			AttachConsole(GetCurrentProcessId());
			freopen("CONIN$","r",stdin);
			freopen("CONOUT$","w",stdout);
			freopen("CONOUT$","w",stderr);

			printf("Graphics Info: %s", dll_DllInfo());
		}

	private:
		HWND hWnd;
		WNDCLASS windowClass;
		HINSTANCE hInstance;
		HINSTANCE hDll;
		MSG msg;
		dll::HBPDD3D hD3D;
	};
}

#endif