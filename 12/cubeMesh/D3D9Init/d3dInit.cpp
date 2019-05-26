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

ID3DXMesh* cube = 0;
const DWORD NumSubsets = 3;
D3DMATERIAL9 cubeMtrl[3] = {d3d::BLUE_MTRL,d3d::RED_MTRL,d3d::GREEN_MTRL};

ID3DXFont* Font = 0;
RECT rect = {0, 0, Width, Height};

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z, float nx, float ny, float nz)
	{
		_x = x; _y = y; _z = z; _nx = nx; _ny = ny; _nz = nz;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;


bool Setup()
{
	D3DXFONT_DESC df;
	ZeroMemory(&df,sizeof(D3DXFONT_DESC));
	df.Height = 25;
	df.Width = 12;
	df.Weight = 500;
	df.MipLevels = D3DX_DEFAULT;
	df.Italic = false;
	df.CharSet = DEFAULT_CHARSET;
	df.OutputPrecision = 0;
	df.Quality = 0;
	df.PitchAndFamily = 0;
	strcpy(df.FaceName,"Times New Roman");
	D3DXCreateFontIndirect(Device,&df,&Font);

	HRESULT hr = 0;
	hr = D3DXCreateMeshFVF(
		12,
		24,
		D3DXMESH_MANAGED,
		Vertex::FVF,
		Device,
		&cube);

	Vertex* v = 0;
	cube->LockVertexBuffer(0, (void**)&v);
	v[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f);//前面蓝,cubeMtrl[0]
	v[1] = Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f);
	v[2] = Vertex( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f);
	v[3] = Vertex( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f);

	v[4] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f);//后面
	v[5] = Vertex(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[6] = Vertex( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[7] = Vertex( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

	v[8] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f);//顶面红,cubeMtrl[1]
	v[9] = Vertex(-1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[10] = Vertex( 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex( 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	v[12] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f);//底面,cubeMtrl[1]
	v[13] = Vertex(-1.0f,  -1.0f, 1.0f, 0.0f, -1.0f, 0.0f);
	v[14] = Vertex( 1.0f,  -1.0f, 1.0f, 0.0f, -1.0f, 0.0f);
	v[15] = Vertex( 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f);

	v[16] = Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f);//左面绿,cubeMtrl[2]
	v[17] = Vertex(-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f);
	v[18] = Vertex( -1.0f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f);
	v[19] = Vertex( -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f);

	v[20] = Vertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f);//右面
	v[21] = Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f);
	v[22] = Vertex( 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	v[23] = Vertex( 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f);

	cube->UnlockVertexBuffer();

	WORD* i = 0;
	cube->LockIndexBuffer(0, (void**)&i);
	i[0] = 0; i[1] = 1; i[2] = 2;//前
	i[3] = 0; i[4] = 2; i[5] = 3;

	i[6] = 4; i[7] = 6; i[8] = 5;//后
	i[9] = 4; i[10] = 7; i[11] = 6;

	i[12] = 8; i[13] = 9; i[14] = 10;//上
	i[15] = 8; i[16] = 10; i[17] = 11;

	i[18] = 12; i[19] = 14; i[20] = 13;//下
	i[21] = 12; i[22] = 15; i[23] = 14;

	i[24] = 16; i[25] = 18; i[26] = 17;//左
	i[27] = 16; i[28] = 19; i[29] = 18;

	i[30] = 20; i[31] = 21; i[32] = 22;//右
	i[33] = 20; i[34] = 22; i[35] = 23;
	cube->UnlockIndexBuffer();


	DWORD* attributeBuffer = 0;
	cube->LockAttributeBuffer(0, &attributeBuffer);

	for(int a = 0; a < 4; a++)
		attributeBuffer[a] = 0;

	for(int b = 4; b < 8; b++)
		attributeBuffer[b] = 1;

	for(int c = 8; c < 12; c++)
		attributeBuffer[c] = 2;

	cube->UnlockAttributeBuffer();


	/*DWORD adjacencyInfo[cube->GetNumFaces() * 3];
	cube->GenerateAdjacency(0.0f, adjacencyInfo);
	DWORD optimizedAdjacencyInfo[cube->GetNumFaces() * 3];

	hr = cube->OptimizeInplace(
	D3DXMESHOPT_ATTRSORT |
	D3DXMESHOPT_COMPACT  |
	D3DXMESHOPT_VERTEXCACHE,
	optimizedAdjacencyInfo,
	0, 0, 0);*/

	D3DXVECTOR3 dir(1.0f,0.0f,0.0f);
		D3DXCOLOR c = d3d::WHITE;
		D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &c);
		Device->SetLight(0,&light);
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

	Device -> SetRenderState(D3DRS_LIGHTING,true);

	return true;
}

void Cleanup()
{
	d3d::Release<ID3DXMesh*>(cube);
	d3d::Release<ID3DXFont*>(Font);
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

		Font->DrawText(NULL,"1、按'X'，绕X轴旋转\n2、按'Y'，绕Y轴旋转\n3、按'Z'，绕Z轴旋转",-1,&rect,DT_TOP|DT_LEFT,0xff000000);
		
		D3DXMATRIX Rx,Ry,Rz;
		static float x = 0.0f;
		static float y = 0.0f;
		static float z = 0.0f;

		D3DXMatrixRotationX(&Rx,x);
		D3DXMatrixRotationY(&Ry,y);
		D3DXMatrixRotationZ(&Rz,z);
		y += timeDelta;
		
		if( ::GetAsyncKeyState('X') )
		{
			x += timeDelta;
		}
		else if( ::GetAsyncKeyState('Y') )
		{
			y += timeDelta;
		}
		else if( ::GetAsyncKeyState('Z') )
		{
			z += timeDelta;
		}

		D3DXMATRIX p = Rx * Ry * Rz;
		Device->SetTransform(D3DTS_WORLD,&p);

		for(int i = 0; i < NumSubsets; i++)
		{
			Device->SetMaterial(&cubeMtrl[i]);
			cube->DrawSubset( i );
		}

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