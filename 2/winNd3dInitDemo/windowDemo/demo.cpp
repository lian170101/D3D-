#include<Windows.h>
#include<d3d9.h> //Direct3Dͷ�ļ�
#include<d3dx9.h>//D3DX��ͷ�ļ�

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HRESULT InitDirect3D(HWND hWnd);//��ʼ��Direct3D

char szClassName[] = "windowclass";//���ڽṹ�������
char szAppTitle[] = "API��������ʵ��";//���ڵı���
IDirect3DDevice9* g_pd3dDevice = NULL;//Direct3D�豸�ӿ�

INT	PASCAL WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,INT nCmdShow)//WinMain()�����Ķ���
{
	HWND hMainWnd;//���ھ��
	MSG msg;//��Ϣ�ṹ��
	WNDCLASS winclass;//���ڽṹ��

	//���崰��
	if(!hPrevInstance)
	{
		winclass.style = CS_HREDRAW|CS_VREDRAW;
		winclass.lpfnWndProc = WndProc;
		winclass.cbClsExtra=0;//���Ӳ���
		winclass.cbWndExtra=0;//���Ӳ���
		winclass.hInstance=hInstance;//��ǰӦ�ó���ʵ�����
		winclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
		winclass.hCursor=LoadCursor(NULL,IDC_ARROW);
		winclass.hbrBackground=(HBRUSH)GetStockObject(LTGRAY_BRUSH);
		winclass.lpszMenuName=NULL;
		winclass.lpszClassName=szClassName;//�����ڽṹ������
		RegisterClass(&winclass);//ע�ᴰ��
	}

	//��������
	hMainWnd=CreateWindow(
		szClassName,//���ڽṹ�������
		szAppTitle,//���ڵı���
		WS_OVERLAPPEDWINDOW,//���ڷ��Ϊ���ص�
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	ShowWindow(hMainWnd,SW_SHOWNORMAL);//��ʾ��󻯴���
	UpdateWindow(hMainWnd);//���´���

	InitDirect3D(hMainWnd);//��ʼ��Direct3D

	//������Ϣѭ��
	while(GetMessage(&msg,NULL,0,0))//��ȡ��Ϣ�����msg�ṹ��
	{
		TranslateMessage(&msg);//���������Ϣ
		DispatchMessage(&msg);//�򴰿ں���������Ϣ
	}

	return msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hMainwnd,UINT message,WPARAM wParam,LPARAM lParam)//���ں�������
{
	switch(message)
	{
	case WM_KEYDOWN:
		if(wParam==VK_ESCAPE)
			::DestroyWindow(hMainwnd);
		else
			MessageBox(hMainwnd,"���¼���","Keyboard",MB_OK);
		break;
	case WM_LBUTTONDOWN:
		MessageBox(hMainwnd,"����������","Keyboard",MB_OK);
		break;
	case WM_RBUTTONDOWN:
		MessageBox(hMainwnd,"��������Ҽ�","Keyboard",MB_OK);
		break;
	default:
		break;
	}

	return DefWindowProc(hMainwnd,message,wParam,lParam);
}

HRESULT InitDirect3D(HWND hWnd)
{
	//����IDirect3D�ӿ�
	IDirect3D9* pD3D = NULL;//IDirect3D9�ӿ�
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);//����IDirect3D9�ӿ�
	if(pD3D == NULL)
		return E_FAIL;
	//��ȡӲ���豸��Ϣ
	D3DCAPS9 caps;
	int vp = 0;
	pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//����Direct3D�豸�ӿ�
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