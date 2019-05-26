//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: d3dinit.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Demonstrates how to initialize Direct3D, how to use the book's framework
//       functions, and how to clear the screen to black.  Note that the Direct3D
//       initialization code is in the d3dUtility.h/.cpp files.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
const int Width  = 640;
const int Height = 480;

IDirect3DDevice9* Device = 0; 

ID3DXMesh* sphere;
D3DXCOLOR c = d3d::RED;
D3DLIGHT9 light;
D3DXVECTOR3 dir(1.0f, 0.0f, 0.0f );

float zm = 1.0f;

D3DXVECTOR3 spherePos(0.0f,0.0f,0.0f);

float y = 0.0f;

ID3DXFont* Font1 = 0;
ID3DXFont* Font2 = 0;
RECT rect = {0, 0, Width, Height};
char* x = "����û�а����κμ�";

bool Setup()
{
	D3DXFONT_DESC df;
	ZeroMemory(&df,sizeof(D3DXFONT_DESC));

	df.Height = 15;
	df.Width = 10;
	df.Weight = 200;
	df.MipLevels = D3DX_DEFAULT;
	df.Italic = false;
	df.CharSet = DEFAULT_CHARSET;
	df.OutputPrecision = 0;
	df.Quality = 0;
	df.PitchAndFamily = 0;
	strcpy(df.FaceName,"����");

	D3DXCreateFontIndirect(Device,&df,&Font1);
	D3DXCreateFontIndirect(Device,&df,&Font2);

	D3DXCreateSphere(Device,2.0f,20,20,&sphere,0);
	
	D3DMATERIAL9 mtrl;
	mtrl = d3d::WHITE_MTRL;
	Device->SetMaterial(&mtrl);

	light = d3d::InitDirectionalLight(&dir, &c);
	
	Device->SetRenderState(D3DRS_NORMALIZENORMALS,true);

	D3DXVECTOR3 position(0.0f, 0.0f, -6.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);
	Device -> SetTransform(D3DTS_VIEW, &V);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f,
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	Device -> SetTransform(D3DTS_PROJECTION, &proj);

	Device -> SetRenderState(D3DRS_LIGHTING,true);

	return true;
}

void Cleanup()
{
	d3d::Release<ID3DXMesh*>(sphere);
	d3d::Release<ID3DXFont*>(Font1);
	d3d::Release<ID3DXFont*>(Font2);
}

bool Display(float timeDelta)
{
	if( Device ) // Only use Device methods if we have a valid device.
	{
		// Instruct the device to set each pixel on the back buffer black -
		// D3DCLEAR_TARGET: 0x00000000 (black) - and to set each pixel on
		// the depth buffer to a value of 1.0 - D3DCLEAR_ZBUFFER: 1.0f.
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		Device->BeginScene();
		/*��ʾ��Ӧ����������*/
		Font1->DrawText(NULL,x,-1,&rect,DT_TOP|DT_CENTER,0xff000000);
		/*���еİ�����ʾ�������*/
		Font2->DrawText(NULL,
			"\n\n\n1��'I'������'O'�����С\n2��'pageup'��'pagedown'��'home'��'end'�����ܹ����������ƶ�\n3��'C'�����ܹ���ת\n4��'R'��'G'��'B'�������в�ͬ��ɫ�Ĺ��յ����ϣ�\n5��'P'��'D'��'S'���ֱ�ѡ����Դ������ƹ�Դ�;۹�ƹ�Դ��",
			-1,
			&rect,
			DT_TOP|DT_LEFT,
			0xff000000);
		/*��������*/
		sphere->DrawSubset(0);

		/*��Դ*/
		Device->SetLight(0,&light);
		Device->LightEnable(0,true);


		D3DXMATRIX Ts,Zoom,Ry;
		D3DXMatrixTranslation(&Ts,spherePos.x,spherePos.y,spherePos.z);
		D3DXMatrixScaling(&Zoom,zm,zm,zm);
		D3DXMatrixRotationY(&Ry,y);
		//y += timeDelta;
		D3DXMATRIX p = Ts * Zoom * Ry;
		Device->SetTransform(D3DTS_WORLD,&p);
		
		Device->EndScene();

		Device->Present(0, 0, 0, 0);
	}
	return true;
}

//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);

		if( ::GetAsyncKeyState('I') )
		{
			x = "��������I��������";
			zm += 0.1f;
		}
		if( ::GetAsyncKeyState('O') )
		{
			x = "��������O�������С";
			zm -= 0.1f;
			if(zm < 0.0f)
				zm = 0.0f;
		}

		if( wParam == VK_UP )//pageup0x33
		{
			x = "��������pageup����������";
			spherePos.y += 0.1f;
		}
		if( wParam == VK_DOWN )//PageDown0x34
		{
			x = "��������pagedown����������";
			spherePos.y -= 0.1f;
		}
		if( wParam == VK_LEFT )//Home0x36
		{
			x = "��������home����������";
			spherePos.x -= 0.1f;
		}
		if( wParam == VK_RIGHT )//End0x35
		{
			x = "��������end����������";
			spherePos.x += 0.1f;
		}

		if( ::GetAsyncKeyState('C') )
		{
			x = "��������C��������ת";
			y += 1.0f;
		}

		if( ::GetAsyncKeyState('R') )
		{
			x = "��������R������Ϊ��ɫ";
			c = d3d::RED;
			light = d3d::InitDirectionalLight(&dir, &c);
		}
		if( ::GetAsyncKeyState('G') )
		{
			x = "��������G������Ϊ��ɫ";
			c = d3d::GREEN;
			light = d3d::InitDirectionalLight(&dir, &c);
		}
		if( ::GetAsyncKeyState('B') )
		{
			x = "��������B������Ϊ��ɫ";
			c = d3d::BLUE;
			light = d3d::InitDirectionalLight(&dir, &c);
		}

		if( ::GetAsyncKeyState('P') )
		{
			x = "��������P�������Դ";
			D3DXVECTOR3 pos(10.0f,0.0f,0.0f);
			light = d3d::InitPointLight(&pos, &c);
		}
		if( ::GetAsyncKeyState('D') )
		{
			x = "��������D���������";
			D3DXVECTOR3 dir(1.0f, 0.0f,0.0f);
			light = d3d::InitDirectionalLight(&dir, &c);
		}
		if( ::GetAsyncKeyState('S') )
		{
			x = "��������S�����۹��";
			D3DXVECTOR3 pos(0.0f,0.0f,-15.0f);
			D3DXVECTOR3 dir(0.0f,0.0f,1.0f);
			light = d3d::InitSpotLight(&pos,&dir, &c);
		}

		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance, 
	PSTR cmdLine,
	int showCmd)
{
	if(!d3d::InitD3D(hinstance,
		640, 480, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop( Display );

	Cleanup();

	Device->Release();

	return 0;
}