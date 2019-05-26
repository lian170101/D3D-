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
D3DMATERIAL9 mtrl;

ID3DXMesh* Teapot=0;
D3DMATERIAL9 TeapotMtrl;

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
		6 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	Vertex* v;

	VB->Lock(0,0,(void**)&v,0);

	v[0] = Vertex(-10.0f,-10.0f,0.0f,0.0f,0.0f,-1.0f,0.0f,1.0f);
	v[1] = Vertex(-10.0f,10.0f,0.0f,0.0f,0.0f,-1.0f,0.0f,0.0f);
	v[2] = Vertex(10.0f,10.0f,0.0f,0.0f,0.0f,-1.0f,1.0f,0.0f);

	v[3] = Vertex(-10.0f,-10.0f,0.0f,0.0f,0.0f,-1.0f,0.0f,1.0f);
	v[4] = Vertex(10.0f,10.0f,0.0f,0.0f,0.0f,-1.0f,1.0f,0.0f);
	v[5] = Vertex(10.0f,-10.0f,0.0f,0.0f,0.0f,-1.0f,1.0f,1.0f);

	VB->Unlock();

	mtrl = d3d::WHITE_MTRL;


	TeapotMtrl = d3d::RED_MTRL;
	TeapotMtrl.Diffuse.a = 0.5f;

	D3DXCreateTeapot(Device,&Teapot,0);

	D3DLIGHT9 dir;
	::ZeroMemory(&dir, sizeof(dir));
	dir.Type      = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse   = d3d::WHITE;
	dir.Specular  = d3d::WHITE * 0.2f;
	dir.Ambient   = d3d::WHITE * 0.6f;
	dir.Direction = D3DXVECTOR3(0.707f, 0.0f, 0.707f);

	Device->SetLight(0, &dir);
	Device->LightEnable(0, true);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	D3DXCreateTextureFromFile(
		Device,
		"flower.jpg",
		&Tex);

	Device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	Device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	Device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_POINT);
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);//[3]
	Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);//[1]
	//Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	//Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);//[2]
	//Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);

	Device->SetRenderState(D3DRS_LIGHTING,true);

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

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(VB);
	d3d::Release<IDirect3DTexture9*>(Tex);
	d3d::Release<ID3DXMesh*>(Teapot);
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

		D3DXMATRIX W;

		D3DXMatrixTranslation(&W,0.0f,0.0f,0.0f);
		Device->SetTransform(D3DTS_WORLD,&W);
		Device->SetStreamSource(0,VB,0,sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);

		Device->SetMaterial(&mtrl);
		Device->SetTexture(0, Tex);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);


		Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);

		D3DXMatrixTranslation(&W,0.0f,0.0f,-5.0f);
		Device->SetTransform(D3DTS_WORLD,&W);
		Device->SetMaterial(&TeapotMtrl);
		Device->SetTexture(0, 0);
		Teapot->DrawSubset(0);

		Device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);

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
		if( wParam == VK_UP )		{			TeapotMtrl.Diffuse.a += 0.1f;			if(TeapotMtrl.Diffuse.a > 1.0f)				TeapotMtrl.Diffuse.a = 1.0f;		}		if( wParam == VK_DOWN)		{			TeapotMtrl.Diffuse.a -= 0.1f;
			if(TeapotMtrl.Diffuse.a < 0.0f)
				TeapotMtrl.Diffuse.a = 0.0f;
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