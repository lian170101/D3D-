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
IDirect3DIndexBuffer9* IB = 0;
float zmX = 0.0f;
float zm = 1.0f;
float Rx = 0.0f;
float Ry = 0.0f;
float Rz = 0.0f;

const int Width = 640;
const int Height = 480;

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
		5 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);
	Device -> CreateIndexBuffer(
		18 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IB,
		0);

	Vertex* vertices;
	VB -> Lock(0, 0, (void**)&vertices, 0);
	vertices[0] = Vertex(-1.0f,0.0f,-1.0f);
	vertices[4] = Vertex(0.0f, 2.0f, 0.0f);
	vertices[1] = Vertex(-1.0f, 0.0f, 1.0f);
	vertices[2] = Vertex(1.0f,0.0f,1.0f);
	vertices[3] = Vertex(1.0f, 0.0f, -1.0f);
	VB -> Unlock();

	WORD* indices = 0;
	IB -> Lock(0, 0, (void**)&indices, 0);
	indices[0] = 0; indices[1] = 4; indices[2] = 3;//前
	indices[3] = 2; indices[4] = 4; indices[5] = 1; //后
	indices[6] = 0; indices[7] = 1; indices[8] = 4; //左（顺时针？）
	indices[9] = 3; indices[10] = 4;indices[11] = 2; //右
	indices[12] = 0;indices[13] = 3;indices[14] = 2;//底
	indices[15] = 0;indices[16] = 2;indices[17] = 1;//底
	IB -> Unlock();

	D3DXVECTOR3 position(0.0f, 5.0f, 0.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 0.0f, 1.0f);

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

	/*D3DVIEWPORT9 vp = {0,0,640,480,0,1};
	Device -> SetViewport(&vp);
	*/
	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(VB);
	d3d::Release<IDirect3DIndexBuffer9*>(IB);
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
		Device->SetStreamSource(0,VB,0,sizeof(Vertex));
		Device->SetIndices(IB);
		Device->SetFVF(Vertex::FVF);

		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,5,0,6);
		
		D3DXMATRIX zoom,zoom1,zoom2,zoom3,zoom4;
		
		D3DXMatrixTranslation(&zoom,zmX,0,0);
		D3DXMatrixScaling(&zoom1,zm,zm,zm);
		D3DXMatrixRotationX(&zoom2, Rx);//1°= PI /180°
		D3DXMatrixRotationY(&zoom3, Ry);
		D3DXMatrixRotationZ(&zoom4, Rz);

		D3DXMATRIX p = zoom * zoom1 * zoom2 * zoom3 * zoom4;
		
		Device->SetTransform(D3DTS_WORLD,&p);

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
		if(wParam == 0x52)
			zmX += 0.1f;
		if(wParam == 0x4C)
			zmX -= 0.1f;
		if(wParam == 0x49)
			zm += 0.1f;
		if(wParam == 0x4F)
			zm -= 0.1f;
		if(wParam == 0x58)
			Rx += 0.1f;
		if(wParam == 0x59)
			Ry += 0.1f;
		if(wParam == 0x5A)
			Rz += 0.1f;
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