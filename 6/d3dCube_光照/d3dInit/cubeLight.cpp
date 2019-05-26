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
	ColorVertex(float x, float y, float z, float nx, float ny, float nz)
	{
		_x = x; _y = y; _z = z; 
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	static const DWORD FVF;
};
const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

D3DXVECTOR3 ComputeNormal(D3DXVECTOR3* p0, D3DXVECTOR3* p1, D3DXVECTOR3* p2)
{
	D3DXVECTOR3 p3;
	D3DXVECTOR3 u = *p1 - *p0;
	D3DXVECTOR3 v = *p2 - *p0;
	D3DXVec3Cross(&p3, &u, &v);
	D3DXVec3Normalize(&p3, &p3);
	return p3;
}

bool Setup()
{
	
	Device -> CreateVertexBuffer(
		36 * sizeof(ColorVertex),
		D3DUSAGE_WRITEONLY,
		ColorVertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	/*D3DXVECTOR3 p0(1.0f,0.0f,-1.0f);
	D3DXVECTOR3 p1(1.0f, 1.0f, -1.0f);
	D3DXVECTOR3 p2(1.0f,1.0f,1.0f);
	D3DXVECTOR3 p3 = ComputeNormal(&p0,&p1,&p2);*/

	ColorVertex* v;
	VB -> Lock(0, 0, (void**)&v, 0);
	//V0(-1,0,-1) V1(-1,0,1) V2(1,0,1) V3(1,0,-1)
	//V4(-1,1,-1) V5(-1,1,1) V6(1,1,1) V7(1,1,-1)
	
	v[0] = ColorVertex(-1.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f);//前面 V0 V4 V7 
	v[1] = ColorVertex(-1.0f,2.0f,-1.0f,0.0f,0.0f,-1.0f);
	v[2] = ColorVertex(1.0f, 2.0f, -1.0f,0.0f,0.0f,-1.0f);

	v[3] = ColorVertex(-1.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f);//V0 V7 V3 
	v[4] = ColorVertex(1.0f, 2.0f, -1.0f,0.0f,0.0f,-1.0f);
	v[5] = ColorVertex(1.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f);

	v[6] = ColorVertex(-1.0f,2.0f,-1.0f,0.0f,1.0f,0.0f);//顶面V4 V5 V6
	v[7] = ColorVertex(-1.0f,2.0f,1.0f,0.0f,1.0f,0.0f);
	v[8] = ColorVertex(1.0f,2.0f,1.0f,0.0f,1.0f,0.0f);

	v[9] = ColorVertex(-1.0f,2.0f,-1.0f,0.0f,1.0f,0.0f);//V4 V6 V7
	v[10] = ColorVertex(1.0f,2.0f,1.0f,0.0f,1.0f,0.0f);
	v[11] = ColorVertex(1.0f,2.0f, -1.0f,0.0f,1.0f,0.0f);

	v[12] = ColorVertex(1.0f,0.0f,-1.0f,1.0f,0.0f,0.0f);//右面 V3 V7 V6
	v[13] = ColorVertex(1.0f,2.0f, -1.0f,1.0f,0.0f,0.0f);
	v[14] = ColorVertex(1.0f,2.0f,1.0f,1.0f,0.0f,0.0f);

	v[15] = ColorVertex(1.0f,0.0f,-1.0f,1.0f,0.0f,0.0f);//V3 V6 V2
	v[16] = ColorVertex(1.0f,2.0f,1.0f,1.0f,0.0f,0.0f);
	v[17] = ColorVertex(1.0f,0.0f,1.0f,1.0f,0.0f,0.0f);

	v[18] = ColorVertex(-1.0f,0.0f,-1.0f,-1.0f,0.0f,0.0f);//左面 V0 V5 V4
	v[19] = ColorVertex(-1.0f,2.0f,1.0f,-1.0f,0.0f,0.0f);
	v[20] = ColorVertex(-1.0f,2.0f,-1.0f,-1.0f,0.0f,0.0f);

	v[21] = ColorVertex(-1.0f,0.0f,-1.0f,-1.0f,0.0f,0.0f);//V0 V1 V5
	v[22] = ColorVertex(-1.0f,0.0f,1.0f,-1.0f,0.0f,0.0f);
	v[23] = ColorVertex(-1.0f,2.0f,1.0f,-1.0f,0.0f,0.0f);

	v[24] = ColorVertex(-1.0f,0.0f,1.0f,0.0f,0.0f,1.0f);//背面 V1 V6 V5
	v[25] = ColorVertex(1.0f,2.0f,1.0f,0.0f,0.0f,1.0f);
	v[26] = ColorVertex(-1.0f,2.0f,1.0f,0.0f,0.0f,1.0f);

	v[27] = ColorVertex(-1.0f,0.0f,1.0f,0.0f,0.0f,1.0f);//V1 V2 V6
	v[28] = ColorVertex(1.0f,0.0f,1.0f,0.0f,0.0f,1.0f);
	v[29] = ColorVertex(1.0f,2.0f,1.0f,0.0f,0.0f,1.0f);

	v[30] = ColorVertex(-1.0f,0.0f,-1.0f,0.0f,-1.0f,0.0f);//底面 V0 V2 V1
	v[31] = ColorVertex(1.0f,0.0f,1.0f,0.0f,-1.0f,0.0f);
	v[32] = ColorVertex(-1.0f,0.0f,1.0f,0.0f,-1.0f,0.0f);

	v[33] = ColorVertex(-1.0f,0.0f,-1.0f,0.0f,-1.0f,0.0f);//V0 V3 V2
	v[34] = ColorVertex(1.0f,0.0f,-1.0f,0.0f,-1.0f,0.0f);
	v[35] = ColorVertex(1.0f,0.0f,1.0f,0.0f,-1.0f,0.0f);
	VB -> Unlock();

	D3DMATERIAL9 mtrl;
	mtrl = d3d::WHITE_MTRL;
	Device->SetMaterial(&mtrl);

	D3DLIGHT9 dir;
	//D3DXVECTOR3 dir(0.0f,-1.0f,0.0f);
	::ZeroMemory(&dir, sizeof(dir));
	dir.Type = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse = d3d::GREEN;
	dir.Specular = d3d::GREEN * 0.3f;
	dir.Ambient = d3d::GREEN * 0.6f;
	dir.Direction = D3DXVECTOR3(0.0f,-1.0f,0.0f);
	//D3DXCOLOR c = d3d::RED;
	//D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &c);
	Device->SetLight(0,&dir);
	//Device->SetLight(0,&light);
	Device->LightEnable(0,true);
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

	//Device -> SetRenderState(D3DRS_LIGHTING,false);

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
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

		Device->BeginScene();
		Device->SetStreamSource(0,VB,0,sizeof(ColorVertex));
		Device->SetFVF(ColorVertex::FVF);

		D3DXMATRIX Ts,Rx,Ry;
		D3DXMatrixRotationX(&Rx,3.14f/1.57f);
		D3DXMatrixTranslation(&Ts,0.0f,-1.0f,.0f);

		static float y = 0.0f;
		D3DXMatrixRotationY(&Ry,y);
		y+=timeDelta;
		if(y>=6.28f)
			y = 0.0f;

		D3DXMATRIX p = Ts * Rx * Ry;
		Device->SetTransform(D3DTS_WORLD,&p);
		Device->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_FLAT);

		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,12);

		
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