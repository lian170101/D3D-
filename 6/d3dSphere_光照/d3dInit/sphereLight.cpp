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

ID3DXMesh* sphere;
D3DXCOLOR c = d3d::RED;
D3DLIGHT9 light;

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
	
	/*D3DXVECTOR3 p0(1.0f,0.0f,-1.0f);
	D3DXVECTOR3 p1(1.0f, 1.0f, -1.0f);
	D3DXVECTOR3 p2(1.0f,1.0f,1.0f);
	D3DXVECTOR3 p3 = ComputeNormal(&p0,&p1,&p2);*/

	D3DXCreateSphere(Device,1.0f,20,20,&sphere,0);
	
	D3DMATERIAL9 mtrl;
	mtrl = d3d::WHITE_MTRL;
	Device->SetMaterial(&mtrl);

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
	d3d::Release<ID3DXMesh*>(sphere);
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

		sphere->DrawSubset(0);

		//D3DLIGHT9 dir;
		//D3DXVECTOR3 dir(1.0f,0.0f,0.0f);

		//::ZeroMemory(&dir, sizeof(dir));
		//dir.Type = D3DLIGHT_DIRECTIONAL;
		//dir.Diffuse = d3d::RED;
		//dir.Specular = d3d::RED * 0.3f;
		//dir.Ambient = d3d::RED * 0.6f;
		//dir.Direction = D3DXVECTOR3(0.0f,-1.0f,0.0f);
		
		//D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &c);
		Device->SetLight(0,&light);
		Device->LightEnable(0,true);


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
		if(wParam == 0x41)
		{
			D3DXVECTOR3 dir(1.0f,0.0f,0.0f);
			light = d3d::InitDirectionalLight(&dir, &c);
		}
		if(wParam == 0x42)
		{
			D3DXVECTOR3 position(0.0f,0.0f,-1.0f);
			light = d3d::InitPointLight(&position,&c);
		}
		if(wParam == 0x43)
		{
			D3DXVECTOR3 position(0.0f,0.0f,-1.0f);
			D3DXVECTOR3 dir(0.0f,0.0f,1.0f);
			light = d3d::InitSpotLight(&position,&dir,&c);
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