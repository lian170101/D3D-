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

ID3DXMesh* mesh[6] = {0,0,0,0,0,0};
IDirect3DTexture9* tex[2] = {0,0};
D3DMATERIAL9 mtrl[2];
D3DXMATRIX W[6];
D3DXVECTOR3 TeapotPos(0.0f,3.0f,0.0f);

float x = 0.0f;
float y = 0.0f;
float z = -5.0f;

ID3DXFont* Font = 0;
RECT rect = {0, 0, Width, Height};


bool Setup()
{
	D3DXFONT_DESC df;
	ZeroMemory(&df,sizeof(D3DXFONT_DESC));

	df.Height = 15;
	df.Width = 10;
	df.Weight = 200;
	df.MipLevels = D3DX_DEFAULT;
	df.Italic = false;
	df.CharSet = DEFAULT_CHARSET;
	df.OutputPrecision = 0;
	df.Quality = 0;
	df.PitchAndFamily = 0;
	strcpy(df.FaceName,"宋体");

	D3DXCreateFontIndirect(Device,&df,&Font);

	D3DXCreateCylinder(Device, 3.0f, 3.0f, 0.1f, 40, 40, &mesh[0], 0);
	D3DXCreateCylinder(Device, 0.2f, 0.2f, 3.0f, 20, 20, &mesh[1], 0);
	D3DXCreateCylinder(Device, 0.2f, 0.2f, 3.0f, 20, 20, &mesh[2], 0);
	D3DXCreateCylinder(Device, 0.2f, 0.2f, 3.0f, 20, 20, &mesh[3], 0);
	D3DXCreateCylinder(Device, 0.2f, 0.2f, 3.0f, 20, 20, &mesh[4], 0);
	D3DXCreateTeapot(Device, &mesh[5], 0);

	mtrl[0] = d3d::WHITE_MTRL;
	mtrl[1] = d3d::YELLOW_MTRL;

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

	D3DXCreateTextureFromFile(Device,"black.jpg",&tex[0]);
	D3DXCreateTextureFromFile(Device,"brown.jpg",&tex[1]);

	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

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
	for(int i = 0; i < 6; i++)
		d3d::Release<ID3DXMesh*>(mesh[i]);
	for(int j = 0; j < 2; j++)
		d3d::Release<IDirect3DTexture9*>(tex[j]);
	d3d::Release<ID3DXFont*>(Font);
}

bool Display(float timeDelta)
{
	if( Device ) // Only use Device methods if we have a valid device.
	{
		// Instruct the device to set each pixel on the back buffer black -
		// D3DCLEAR_TARGET: 0x00000000 (black) - and to set each pixel on
		// the depth buffer to a value of 1.0 - D3DCLEAR_ZBUFFER: 1.0f.
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0xffffffff, 1.0f, 0);

		D3DXVECTOR3 position(x, y, z);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);
		Device -> SetTransform(D3DTS_VIEW, &V);

		Device->BeginScene();

		/*Device->SetRenderState(D3DRS_STENCILENABLE,true);
		Device->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_ALWAYS);
		Device->SetRenderState(D3DRS_STENCILREF,0x1);
		Device->SetRenderState(D3DRS_STENCILZFAIL,D3DSTENCILOP_KEEP);
		Device->SetRenderState(D3DRS_STENCILFAIL,D3DSTENCILOP_KEEP);
		Device->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_REPLACE);*/

		/*提示信息*/
		Font->DrawText(NULL,
			"1、'VK_LEFT'茶壶左移，'VK_RIGHT'茶壶右移、\n   'VK_UP'茶壶上移，  'VK_DOWN'茶壶下移\n\n2、'W'视点y+、'S'视点y-、\n   'A'视点x-、'D'视点x+、\n   'Z'视点z+、'X'视点z-",
			-1,
			&rect,
			DT_TOP|DT_LEFT,
			0xff000000);


		/*绘制圆盘*/
		D3DXMATRIX Ry1;
		D3DXMatrixRotationX(&Ry1,3.14f/4.0f);
		D3DXMatrixTranslation(&W[0],  0.0f,  1.0f, -1.0f);
		D3DXMATRIX p1 = W[0] * Ry1;
		Device->SetMaterial(&mtrl[0]);
		Device->SetTransform(D3DTS_WORLD, &p1);
		Device->SetTexture(0, tex[0]);
		mesh[0]->DrawSubset(0);

		/*绘制四条腿*/
		D3DXMATRIX Ry2;
		D3DXMatrixRotationX(&Ry2,3.14f/4.0f);
		D3DXMatrixTranslation(&W[1],  -1.2f, -0.8f,0.5f);
		D3DXMATRIX p2 = W[1] * Ry2;
		Device->SetMaterial(&mtrl[0]);
		Device->SetTransform(D3DTS_WORLD, &p2);
		Device->SetTexture(0, tex[1]);
		mesh[1]->DrawSubset(0);//右下

		D3DXMATRIX Ry3;
		D3DXMatrixRotationX(&Ry3,3.14f/4.0f);
		D3DXMatrixTranslation(&W[2],   -1.2f,  2.2f, 0.5f);
		D3DXMATRIX p3 = W[2] * Ry3;
		Device->SetMaterial(&mtrl[0]);
		Device->SetTransform(D3DTS_WORLD, &p3);
		Device->SetTexture(0, tex[1]);
		mesh[2]->DrawSubset(0);//左上

		D3DXMATRIX Ry4;
		D3DXMatrixRotationX(&Ry4,3.14f/4.0f);
		D3DXMatrixTranslation(&W[3], 1.0f,  -0.8f, 0.5f);
		D3DXMATRIX p4 = W[3] * Ry4;
		Device->SetMaterial(&mtrl[0]);
		Device->SetTransform(D3DTS_WORLD, &p4);
		Device->SetTexture(0, tex[1]);
		mesh[3]->DrawSubset(0);//右下

		D3DXMATRIX Ry5;
		D3DXMatrixRotationX(&Ry5,3.14f/4.0f);
		D3DXMatrixTranslation(&W[4], 1.0f,  2.2f, 0.5f);
		D3DXMATRIX p5 = W[4] * Ry5;
		Device->SetMaterial(&mtrl[0]);
		Device->SetTransform(D3DTS_WORLD, &p5);
		Device->SetTexture(0, tex[1]);
		mesh[4]->DrawSubset(0);//右上


		/*Device->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_EQUAL);
		Device->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);*/


		/*绘制茶壶镜像，不绘制也可，茶壶镜像与本题无关*/
		/*D3DXMATRIX Ry6,R,zoom;
		D3DXMatrixRotationX(&Ry6,3.14f/4.0f);
		D3DXMatrixTranslation(&W[5],TeapotPos.x,TeapotPos.y,TeapotPos.z);
		D3DXMatrixScaling(&zoom,0.5,0.5,0.5);

		D3DXPLANE plane(0.0f,-1.0f,0.0f,0.0f);
		D3DXMatrixReflect(&R,&plane);

		D3DXMATRIX p6 = W[5] * Ry6 * R * zoom;
		Device->SetTransform(D3DTS_WORLD,&p6);
		Device->SetMaterial(&mtrl[1]);
		Device->SetTexture(0, 0);
		mesh[5]->DrawSubset(0);

		Device->Clear(0,0,D3DCLEAR_ZBUFFER,0,1.0f,0);
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
		Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
		Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
		mesh[5]->DrawSubset(0);

		Device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
		Device->SetRenderState(D3DRS_STENCILENABLE,false);
		Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);*/

		D3DXMATRIX Ry7,zoom1;
		D3DXMatrixRotationX(&Ry7,-3.14f/4.0f);
		D3DXMatrixTranslation(&W[5],TeapotPos.x,TeapotPos.y,TeapotPos.z+1.0f);
		D3DXMatrixScaling(&zoom1,0.5,0.5,0.5);
		D3DXMATRIX p7 = W[5] * Ry7 * zoom1;
		Device->SetTransform(D3DTS_WORLD,&p7);
		Device->SetMaterial(&mtrl[1]);
		Device->SetTexture(0, 0);
		mesh[5]->DrawSubset(0);

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
		{
			TeapotPos.x -= 0.1f;
			if(TeapotPos.x < -3.5f)
				TeapotPos.x = -3.5f;
		}	
		if ( wParam == VK_RIGHT )
		{
			TeapotPos.x += 0.1f;
			if(TeapotPos.x > 3.5f)
				TeapotPos.x = 3.5f;
		}	
		if( wParam == VK_UP )
		{
			TeapotPos.z += 0.1f;
			if(TeapotPos.z > 5.0f)
				TeapotPos.z = 5.0f;
		}
		if( wParam == VK_DOWN )
		{
			TeapotPos.z -= 0.1f;
			if(TeapotPos.z < -3.5f)
				TeapotPos.z = -3.5f;
		}
		if( ::GetAsyncKeyState('W'))
			y += 2.0f;
		if( ::GetAsyncKeyState('S'))
			y -= 2.0f;
		if( ::GetAsyncKeyState('A'))
			x -= 2.0f;
		if( ::GetAsyncKeyState('D'))
			x += 2.0f;
		if( ::GetAsyncKeyState('Z'))
			z += 2.0f;
		if( ::GetAsyncKeyState('X'))
			z -= 2.0f;

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