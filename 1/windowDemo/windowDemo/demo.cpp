#include<Windows.h>
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
char szClassName[] = "windowclass";//窗口结构体的名称
char szAppTitle[] = "API建立窗口实例";//窗口的标题

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

	ShowWindow(hMainWnd,SW_SHOWNORMAL);
	UpdateWindow(hMainWnd);//更新窗口

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
		//MessageBox(hMainwnd,"按下键盘","Keyboard",MB_OK);
		if(wParam==VK_ESCAPE)
			::DestroyWindow(hMainwnd);
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