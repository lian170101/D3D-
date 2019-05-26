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

IDirect3DVertexBuffer9 * VB=0;
IDirect3DTexture9 * Tex=0;

//定义一个顶点结构体
struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		_x = x; _y = y; _z = z; _nx = nx; _ny = ny; _nz = nz; _u = u; _v = v;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u,_v;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

//D3DXVECTOR3 ComputeNormal(D3DXVECTOR3* p0, D3DXVECTOR3* p1, D3DXVECTOR3* p2)
//{
//	D3DXVECTOR3 p3;
//	D3DXVECTOR3 u = *p1 - *p0;
//	D3DXVECTOR3 v = *p2 - *p0;
//	D3DXVec3Cross(&p3, &u, &v);
//	D3DXVec3Normalize(&p3, &p3);
//	return p3;
//}

bool Setup()
{

	/*D3DXVECTOR3 p0(1.0f,0.0f,-1.0f);
	D3DXVECTOR3 p1(1.0f, 1.0f, -1.0f);
	D3DXVECTOR3 p2(1.0f,1.0f,1.0f);
	D3DXVECTOR3 p3 = ComputeNormal(&p0,&p1,&p2);*/

	Device->CreateVertexBuffer(
		12 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	Vertex* v;

	VB->Lock(0,0,(void**)&v,0);

	// front face
	v[0] = Vertex(-1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f,0.0f,1.0f);
	v[1] = Vertex( 0.0f, 1.0f,  0.0f, 0.0f, 0.707f, -0.707f,0.5f,0.5f);
	v[2] = Vertex( 1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f,1.0f,1.0f);

	// left face
	v[3] = Vertex(-1.0f, 0.0f,  1.0f, -0.707f, 0.707f, 0.0f,0.0f,1.0f);
	v[4] = Vertex( 0.0f, 1.0f,  0.0f, -0.707f, 0.707f, 0.0f,0.5f,0.5f);
	v[5] = Vertex(-1.0f, 0.0f, -1.0f, -0.707f, 0.707f, 0.0f,0.0f,0.0f);

	// right face
	v[6] = Vertex( 1.0f, 0.0f, -1.0f, 0.707f, 0.707f, 0.0f,1.0f,1.0f);
	v[7] = Vertex( 0.0f, 1.0f,  0.0f, 0.707f, 0.707f, 0.0f,0.5f,0.5f);
	v[8] = Vertex( 1.0f, 0.0f,  1.0f, 0.707f, 0.707f, 0.0f,1.0f,0.0f);

	// back face
	v[9]  = Vertex( 1.0f, 0.0f,  1.0f, 0.0f, 0.707f, 0.707f,1.0f,0.0f);
	v[10] = Vertex( 0.0f, 1.0f,  0.0f, 0.0f, 0.707f, 0.707f,0.5f,0.5f);
	v[11] = Vertex(-1.0f, 0.0f,  1.0f, 0.0f, 0.707f, 0.707f,0.0f,0.0f);

	VB->Unlock();

	D3DXCreateTextureFromFile(
		Device,
		"1.jpg",
		&Tex);

	
	Device->SetRenderState(D3DRS_LIGHTING,false);

	D3DXVECTOR3 position(0.0f, 0.0f, -3.0f);
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

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(VB);
	d3d::Release<IDirect3DTexture9*>(Tex);
}

bool Display(float timeDelta)
{
	if( Device ) // Only use Device methods if we have a valid device.
	{
		// Instruct the device to set each pixel on the back buffer black -
		// D3DCLEAR_TARGET: 0x00000000 (black) - and to set each pixel on
		// the depth buffer to a value of 1.0 - D3DCLEAR_ZBUFFER: 1.0f.

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		D3DXMATRIX Ry;
		static float y = 0.0f;
		D3DXMatrixRotationY(&Ry,y);
		y+=timeDelta;
		Device->SetTransform(D3DTS_WORLD,&Ry);

		Device->BeginScene();

		Device->SetStreamSource(0,VB,0,sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);

		Device->SetTexture(0, Tex);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,4);

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