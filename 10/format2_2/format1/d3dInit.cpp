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
//
// Globals
//

IDirect3DDevice9* Device = 0; 

IDirect3DVertexBuffer9* VB = 0;
IDirect3DTexture9* tex1 = 0;
IDirect3DTexture9* tex2 = 0;
IDirect3DTexture9* tex3 = 0;

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


bool Setup()
{
	Device->CreateVertexBuffer(
		36 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	Vertex* v;

	VB->Lock(0,0,(void**)&v,0);

	//V0(-1,0,-1) V1(-1,0,1) V2(1,0,1) V3(1,0,-1)
	//V4(-1,1,-1) V5(-1,1,1) V6(1,1,1) V7(1,1,-1)
	
	v[0] = Vertex(-1.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,1.0f);//前面 V0 V4 V7 
	v[1] = Vertex(-1.0f,2.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f);
	v[2] = Vertex(1.0f, 2.0f, -1.0f,0.0f,0.0f,-1.0f,1.0f,0.0f);

	v[3] = Vertex(-1.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,1.0f);//V0 V7 V3 
	v[4] = Vertex(1.0f, 2.0f, -1.0f,0.0f,0.0f,-1.0f,1.0f,0.0f);
	v[5] = Vertex(1.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,1.0f,1.0f);

	v[6] = Vertex(-1.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,1.0f);//背面 V1 V6 V5
	v[7] = Vertex(1.0f,2.0f,1.0f,0.0f,0.0f,1.0f,1.0f,0.0f);
	v[8] = Vertex(-1.0f,2.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f);

	v[9] = Vertex(-1.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,1.0f);//V1 V2 V6
	v[10] = Vertex(1.0f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f);
	v[11] = Vertex(1.0f,2.0f,1.0f,0.0f,0.0f,1.0f,1.0f,0.0f);

	v[12] = Vertex(1.0f,0.0f,-1.0f,1.0f,0.0f,0.0f,0.0f,1.0f);//右面 V3 V7 V6
	v[13] = Vertex(1.0f,2.0f, -1.0f,1.0f,0.0f,0.0f,0.0f,0.0f);
	v[14] = Vertex(1.0f,2.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f);

	v[15] = Vertex(1.0f,0.0f,-1.0f,1.0f,0.0f,0.0f,0.0f,1.0f);//V3 V6 V2
	v[16] = Vertex(1.0f,2.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f);
	v[17] = Vertex(1.0f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f,1.0f);

	v[18] = Vertex(-1.0f,0.0f,-1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f);//左面 V0 V5 V4
	v[19] = Vertex(-1.0f,2.0f,1.0f,-1.0f,0.0f,0.0f,1.0f,0.0f);
	v[20] = Vertex(-1.0f,2.0f,-1.0f,-1.0f,0.0f,0.0f,0.0f,0.0f);

	v[21] = Vertex(-1.0f,0.0f,-1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f);//V0 V1 V5
	v[22] = Vertex(-1.0f,0.0f,1.0f,-1.0f,0.0f,0.0f,1.0f,1.0f);
	v[23] = Vertex(-1.0f,2.0f,1.0f,-1.0f,0.0f,0.0f,1.0f,0.0f);

	v[24] = Vertex(-1.0f,2.0f,-1.0f,0.0f,1.0f,0.0f,0.0f,1.0f);//顶面V4 V5 V6
	v[25] = Vertex(-1.0f,2.0f,1.0f,0.0f,1.0f,0.0f,0.0f,0.0f);
	v[26] = Vertex(1.0f,2.0f,1.0f,0.0f,1.0f,0.0f,1.0f,0.0f);

	v[27] = Vertex(-1.0f,2.0f,-1.0f,0.0f,1.0f,0.0f,0.0f,1.0f);//V4 V6 V7
	v[28] = Vertex(1.0f,2.0f,1.0f,0.0f,1.0f,0.0f,1.0f,0.0f);
	v[29] = Vertex(1.0f,2.0f, -1.0f,0.0f,1.0f,0.0f,1.0f,1.0f);

	v[30] = Vertex(-1.0f,0.0f,-1.0f,0.0f,-1.0f,0.0f,0.0f,1.0f);//底面 V0 V2 V1
	v[31] = Vertex(1.0f,0.0f,1.0f,0.0f,-1.0f,0.0f,1.0f,0.0f);
	v[32] = Vertex(-1.0f,0.0f,1.0f,0.0f,-1.0f,0.0f,0.0f,0.0f);

	v[33] = Vertex(-1.0f,0.0f,-1.0f,0.0f,-1.0f,0.0f,0.0f,1.0f);//V0 V3 V2
	v[34] = Vertex(1.0f,0.0f,-1.0f,0.0f,-1.0f,0.0f,1.0f,1.0f);
	v[35] = Vertex(1.0f,0.0f,1.0f,0.0f,-1.0f,0.0f,1.0f,0.0f);

	VB->Unlock();

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, false);

	D3DXCreateTextureFromFile(
		Device,
		"red.jpg",
		&tex1);

	D3DXCreateTextureFromFile(
		Device,
		"green.jpg",
		&tex2);

	D3DXCreateTextureFromFile(
		Device,
		"blue.jpg",
		&tex3);

	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
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

	Device->SetRenderState(D3DRS_LIGHTING,false);

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(VB);
	d3d::Release<IDirect3DTexture9*>(tex1);
	d3d::Release<IDirect3DTexture9*>(tex2);
	d3d::Release<IDirect3DTexture9*>(tex3);
}

bool Display(float timeDelta)
{
	if( Device ) // Only use Device methods if we have a valid device.
	{
		// Instruct the device to set each pixel on the back buffer black -
		// D3DCLEAR_TARGET: 0x00000000 (black) - and to set each pixel on
		// the depth buffer to a value of 1.0 - D3DCLEAR_ZBUFFER: 1.0f.
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		
		D3DXMATRIX Ry,Rx;
		static float y = 0.0f;
		D3DXMatrixRotationY(&Ry,y);
		y+=timeDelta;
		D3DXMatrixRotationX(&Rx,3.14f/4.0f);
		D3DXMATRIX p = Rx * Ry;
		Device->SetTransform(D3DTS_WORLD,&p);

		Device->BeginScene();

		D3DXMATRIX W;

		Device->SetStreamSource(0,VB,0,sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);
		Device->SetTexture(0, tex1);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,4);

		Device->SetTexture(0, tex2);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,12,4);

		Device->SetTexture(0, tex3);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,24,4);
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