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
char* x = "您还没有按下任何键";

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
	strcpy(df.FaceName,"宋体");

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
		/*提示相应按键及功能*/
		Font1->DrawText(NULL,x,-1,&rect,DT_TOP|DT_CENTER,0xff000000);
		/*所有的按键提示，不会变*/
		Font2->DrawText(NULL,
			"\n\n\n1、'I'键球变大，'O'键球变小\n2、'pageup'、'pagedown'、'home'、'end'，球能够上下左右移动\n3、'C'键球能够旋转\n4、'R'、'G'、'B'，可以有不同颜色的光照到球上；\n5、'P'、'D'、'S'，分别选择点光源，方向灯光源和聚光灯光源。",
			-1,
			&rect,
			DT_TOP|DT_LEFT,
			0xff000000);
		/*绘制球体*/
		sphere->DrawSubset(0);

		/*光源*/
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
			x = "您按下了I键，球变大";
			zm += 0.1f;
		}
		if( ::GetAsyncKeyState('O') )
		{
			x = "您按下了O键，球变小";
			zm -= 0.1f;
			if(zm < 0.0f)
				zm = 0.0f;
		}

		if( wParam == VK_UP )//pageup0x33
		{
			x = "您按下了pageup键，球上移";
			spherePos.y += 0.1f;
		}
		if( wParam == VK_DOWN )//PageDown0x34
		{
			x = "您按下了pagedown键，球下移";
			spherePos.y -= 0.1f;
		}
		if( wParam == VK_LEFT )//Home0x36
		{
			x = "您按下了home键，球左移";
			spherePos.x -= 0.1f;
		}
		if( wParam == VK_RIGHT )//End0x35
		{
			x = "您按下了end键，球右移";
			spherePos.x += 0.1f;
		}

		if( ::GetAsyncKeyState('C') )
		{
			x = "您按下了C键，球旋转";
			y += 1.0f;
		}

		if( ::GetAsyncKeyState('R') )
		{
			x = "您按下了R键，球为红色";
			c = d3d::RED;
			light = d3d::InitDirectionalLight(&dir, &c);
		}
		if( ::GetAsyncKeyState('G') )
		{
			x = "您按下了G键，球为绿色";
			c = d3d::GREEN;
			light = d3d::InitDirectionalLight(&dir, &c);
		}
		if( ::GetAsyncKeyState('B') )
		{
			x = "您按下了B键，球为蓝色";
			c = d3d::BLUE;
			light = d3d::InitDirectionalLight(&dir, &c);
		}

		if( ::GetAsyncKeyState('P') )
		{
			x = "您按下了P键，点光源";
			D3DXVECTOR3 pos(10.0f,0.0f,0.0f);
			light = d3d::InitPointLight(&pos, &c);
		}
		if( ::GetAsyncKeyState('D') )
		{
			x = "您按下了D键，方向光";
			D3DXVECTOR3 dir(1.0f, 0.0f,0.0f);
			light = d3d::InitDirectionalLight(&dir, &c);
		}
		if( ::GetAsyncKeyState('S') )
		{
			x = "您按下了S键，聚光灯";
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