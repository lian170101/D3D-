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
IDirect3DTexture9* tex = 0;
D3DMATERIAL9 mtrl;

ID3DXMesh* Teapot = 0;
D3DMATERIAL9 TeapotMtrl;

D3DXVECTOR3 TeapotPos(0.0f, 0.0f, -5.0f);

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

	TeapotMtrl = d3d::YELLOW_MTRL;

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
	Device->SetRenderState(D3DRS_SPECULARENABLE, false);

	D3DXCreateTextureFromFile(
		Device,
		"ice.bmp",
		&tex);

	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

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

	Device->SetRenderState(D3DRS_LIGHTING,true);

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(VB);
	d3d::Release<IDirect3DTexture9*>(tex);
	d3d::Release<ID3DXMesh*>(Teapot);

}

bool Display(float timeDelta)
{
	if( Device ) // Only use Device methods if we have a valid device.
	{
		// Instruct the device to set each pixel on the back buffer black -
		// D3DCLEAR_TARGET: 0x00000000 (black) - and to set each pixel on
		// the depth buffer to a value of 1.0 - D3DCLEAR_ZBUFFER: 1.0f.
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0xffffffff, 1.0f, 0);
		
		Device->BeginScene();

		Device->SetRenderState(D3DRS_STENCILENABLE,true);
		Device->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_ALWAYS);
		Device->SetRenderState(D3DRS_STENCILREF,0x1);
		Device->SetRenderState(D3DRS_STENCILZFAIL,D3DSTENCILOP_KEEP);
		Device->SetRenderState(D3DRS_STENCILFAIL,D3DSTENCILOP_KEEP);
		Device->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_REPLACE);

		D3DXMATRIX W;

		D3DXMatrixTranslation(&W,0.0f,0.0f,0.0f);
		Device->SetTransform(D3DTS_WORLD,&W);
		Device->SetStreamSource(0,VB,0,sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);
		Device->SetMaterial(&mtrl);
		Device->SetTexture(0, tex);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

		Device->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_EQUAL);
		Device->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);

		D3DXMatrixTranslation(&W,TeapotPos.x,TeapotPos.y,TeapotPos.z);
		
		D3DXMATRIX R,p;
		D3DXPLANE plane(0.0f,0.0f,1.0f,0.0f);
		D3DXMatrixReflect(&R,&plane);
		p = W * R;

		Device->SetTransform(D3DTS_WORLD,&p);
		Device->SetMaterial(&TeapotMtrl);
		Device->SetTexture(0, 0);

		Device->Clear(0,0,D3DCLEAR_ZBUFFER,0,1.0f,0);
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
		Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
		Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
		Teapot->DrawSubset(0);
		
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
		Device->SetRenderState(D3DRS_STENCILENABLE,false);
		Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);


		D3DXMatrixTranslation(&W,TeapotPos.x,TeapotPos.y,TeapotPos.z);
		Device->SetTransform(D3DTS_WORLD,&W);
		Device->SetMaterial(&TeapotMtrl);
		Device->SetTexture(0, 0);
		Teapot->DrawSubset(0);

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
		if ( wParam == VK_LEFT )
			TeapotPos.x -= 0.1f;
		if ( wParam == VK_RIGHT )
			TeapotPos.x += 0.1f;
		if( wParam == VK_UP )
			TeapotPos.y += 0.1f;
		if( wParam == VK_DOWN )
			TeapotPos.y -= 0.1f;
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