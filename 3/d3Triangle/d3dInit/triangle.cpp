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

const int Width = 640;
const int Height = 480;

IDirect3DVertexBuffer9* Triangle = 0;
//定义一个顶点结构体
struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z)
	{
		_x = x; _y = y; _z = z;
	}
	float _x, _y, _z;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ;

bool Setup()
{
	
	Device -> CreateVertexBuffer(
		3 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&Triangle,
		0);

	Vertex* vertices;
	Triangle -> Lock(0, 0, (void**)&vertices, 0);
	vertices[0] = Vertex(-5.0f, 0.0f, 0.0f);
	vertices[1] = Vertex(0.0f, 5.0f, 0.0f);
	vertices[2] = Vertex(5.0f, 0.0f, 0.0f);
	Triangle -> Unlock();

	D3DXVECTOR3 position(0.0f, 0.0f, -15.0f);
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

	Device -> SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);

	D3DVIEWPORT9 vp = {0,0,640,480,0,1};
	Device -> SetViewport(&vp);
	
	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(Triangle);

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
		Device->SetStreamSource(0,Triangle,0,sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);
		D3DXMATRIX matrix;
		D3DXMatrixTranslation(&matrix,0,0,-8);
		Device->SetTransform(D3DTS_WORLD,&matrix);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,1);
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