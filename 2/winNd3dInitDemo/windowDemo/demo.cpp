#include<Windows.h>
#include<d3d9.h> //Direct3D头文件
#include<d3dx9.h>//D3DX库头文件

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HRESULT InitDirect3D(HWND hWnd);//初始化Direct3D

char szClassName[] = "windowclass";//窗口结构体的名称
char szAppTitle[] = "API建立窗口实例";//窗口的标题
IDirect3DDevice9* g_pd3dDevice = NULL;//Direct3D设备接口

INT	PASCAL WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,INT nCmdShow)//WinMain()函数的定义
{
	HWND hMainWnd;//窗口句柄
	MSG msg;//消息结构体
	WNDCLASS winclass;//窗口结构体

	//定义窗口
	if(!hPrevInstance)
	{
		winclass.style = CS_HREDRAW|CS_VREDRAW;
		winclass.lpfnWndProc = WndProc;
		winclass.cbClsExtra=0;//附加参数
		winclass.cbWndExtra=0;//附加参数
		winclass.hInstance=hInstance;//当前应用程序实例句柄
		winclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
		winclass.hCursor=LoadCursor(NULL,IDC_ARROW);
		winclass.hbrBackground=(HBRUSH)GetStockObject(LTGRAY_BRUSH);
		winclass.lpszMenuName=NULL;
		winclass.lpszClassName=szClassName;//给窗口结构体命名
		RegisterClass(&winclass);//注册窗口
	}

	//创建窗口
	hMainWnd=CreateWindow(
		szClassName,//窗口结构体的名称
		szAppTitle,//窗口的标题
		WS_OVERLAPPEDWINDOW,//窗口风格为可重叠
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	ShowWindow(hMainWnd,SW_SHOWNORMAL);//显示最大化窗口
	UpdateWindow(hMainWnd);//更新窗口

	InitDirect3D(hMainWnd);//初始化Direct3D

	//建立消息循环
	while(GetMessage(&msg,NULL,0,0))//获取消息，填充msg结构体
	{
		TranslateMessage(&msg);//翻译键盘消息
		DispatchMessage(&msg);//向窗口函数发送消息
	}

	return msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hMainwnd,UINT message,WPARAM wParam,LPARAM lParam)//窗口函数定义
{
	switch(message)
	{
	case WM_KEYDOWN:
		if(wParam==VK_ESCAPE)
			::DestroyWindow(hMainwnd);
		else
			MessageBox(hMainwnd,"按下键盘","Keyboard",MB_OK);
		break;
	case WM_LBUTTONDOWN:
		MessageBox(hMainwnd,"按下鼠标左键","Keyboard",MB_OK);
		break;
	case WM_RBUTTONDOWN:
		MessageBox(hMainwnd,"按下鼠标右键","Keyboard",MB_OK);
		break;
	default:
		break;
	}

	return DefWindowProc(hMainwnd,message,wParam,lParam);
}

HRESULT InitDirect3D(HWND hWnd)
{
	//创建IDirect3D接口
	IDirect3D9* pD3D = NULL;//IDirect3D9接口
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);//创建IDirect3D9接口
	if(pD3D == NULL)
		return E_FAIL;
	//获取硬件设备信息
	D3DCAPS9 caps;
	int vp = 0;
	pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//创建Direct3D设备接口
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.BackBufferWidth = 640;
	d3dpp.BackBufferHeight = 480;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,vp,&d3dpp,&g_pd3dDevice);
	pD3D->Release();
	return S_OK;
}