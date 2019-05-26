#include<Windows.h>
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
char szClassName[] = "windowclass";//���ڽṹ�������
char szAppTitle[] = "API��������ʵ��";//���ڵı���

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

	ShowWindow(hMainWnd,SW_SHOWNORMAL);
	UpdateWindow(hMainWnd);//���´���

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
		//MessageBox(hMainwnd,"���¼���","Keyboard",MB_OK);
		if(wParam==VK_ESCAPE)
			::DestroyWindow(hMainwnd);
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