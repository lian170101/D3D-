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

//
// Globals
//

IDirect3DDevice9* Device = 0; 

IDirect3DVertexBuffer9* VB = 0;


const int Width = 640;
const int Height = 480;

//定义一个顶点结构体
struct ColorVertex
{
	ColorVertex(){}
	ColorVertex(float x, float y, float z,D3DCOLOR color)
	{
		_x = x; _y = y; _z = z; _color = color;
	}
	float _x, _y, _z;
	D3DCOLOR _color;
	static const DWORD FVF;
};
const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

bool Setup()
{
	
	Device -> CreateVertexBuffer(
		12 * sizeof(ColorVertex),
		D3DUSAGE_WRITEONLY,
		ColorVertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);
	
	ColorVertex* v;
	VB -> Lock(0, 0, (void**)&v, 0);
	//V0(-1,0,-1)
	//V1(0,0,1)
	//V2(1,0,-1)
	//V3(0,1,0)
	v[0] = ColorVertex(-1.0f,0.0f,-1.0f,d3d::RED);//前面 V0 V3 V2 
	v[1] = ColorVertex(0.0f,1.0f,0.0f,d3d::GREEN);
	v[2] = ColorVertex(1.0f, 0.0f, -1.0f,d3d::BLUE);

	v[3] = ColorVertex(1.0f,0.0f,-1.0f,d3d::YELLOW);//右面 V2 V3 V1
	v[4] = ColorVertex(0.0f, 1.0f, 0.0f,d3d::RED);
	v[5] = ColorVertex(0.0f,0.0f,1.0f,d3d::GREEN);

	v[6] = ColorVertex(-1.0f,0.0f,-1.0f,d3d::BLUE);//左面 V0 V1 V3
	v[7] = ColorVertex(0.0f, 0.0f, 1.0f,d3d::YELLOW);
	v[8] = ColorVertex(0.0f, 1.0f, 0.0f,d3d::RED);

	v[9] = ColorVertex(-1.0f, 0.0f, -1.0f,d3d::GREEN);//底面 V0 V2 V1
	v[10] = ColorVertex(1.0f,0.0f,-1.0f,d3d::BLUE);
	v[11] = ColorVertex(0.0f, 0.0f, 1.0f,d3d::YELLOW);
	VB -> Unlock();


	D3DXVECTOR3 position(0.0f, 0.0f, -4.0f);
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

	Device -> SetRenderState(D3DRS_LIGHTING,false);

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(VB);
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
		Device->SetStreamSource(0,VB,0,sizeof(ColorVertex));
		Device->SetFVF(ColorVertex::FVF);

		D3DXMATRIX Tz,Rx,Ry;
		D3DXMatrixTranslation(&Tz,0.0f,0.0f,-1.0f);
		D3DXMatrixRotationX(&Rx,3.14f/1.05f);
		static float y = 0.0f;
		D3DXMatrixRotationY(&Ry,y);
		y+=timeDelta;
		if(y>=6.28f)
			y = 0.0f;
		D3DXMATRIX p = Tz * Rx * Ry;
		Device->SetTransform(D3DTS_WORLD,&p);
		
		Device->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_FLAT);

		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,4);

		Device->EndScene();
		// Swap the back and front buffers.
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