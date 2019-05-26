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
#include <vector>

const int Width  = 640;
const int Height = 480;

IDirect3DDevice9* Device = 0; 

ID3DXMesh* mesh = 0;

std::vector<D3DMATERIAL9> Mtrls(0);
std::vector<IDirect3DTexture9*> Tex(0);

bool Setup()
{
	IDirect3DTexture9* texture = 0;
	D3DXCreateTextureFromFile(
		Device,
		"ice.bmp",
		&texture);

	HRESULT hr = 0;

	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls = 0;

	hr = D3DXLoadMeshFromX(
		"bigship1.x",
		D3DXMESH_MANAGED,
		Device,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&mesh);

	if(FAILED(hr))
	{
		::MessageBox(0,"D3DXLoadMeshFromX() - FAILED",0,0);
		return false;
	}

	if(mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; i++)
		{
			D3DMATERIAL9 Mtrl = d3d::BLACK_MTRL;
			//mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			//Mtrls.push_back (mtrls[i].MatD3D);
			Mtrls.push_back (Mtrl);
			if(mtrls[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					Device,
					mtrls[i].pTextureFilename,
					&tex);

				Tex.push_back(tex);
			}
			else
			{
				//Tex.push_back(0);
				Tex.push_back(texture);
			}
		}
	}

	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	d3d::Release<ID3DXBuffer*>(mtrlBuffer);

	DWORD* attributeBuffer = 0;
	mesh->LockAttributeBuffer(0, &attributeBuffer);

	for(int a = 0; a < 4; a++)
		attributeBuffer[a] = 0;

	for(int b = 4; b < 8; b++)
		attributeBuffer[b] = 1;

	for(int c = 8; c < 12; c++)
		attributeBuffer[c] = 2;

	mesh->UnlockAttributeBuffer();


	std::vector<DWORD> adjacencyBuffer(mesh->GetNumFaces() * 3);
	mesh->GenerateAdjacency(0.0f, &adjacencyBuffer[0]);

	hr = mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT  |
		D3DXMESHOPT_VERTEXCACHE,
		&adjacencyBuffer[0],
		0, 0, 0);
	d3d::Release<ID3DXBuffer*>(adjBuffer);


	DWORD numEntries = 0;
	mesh ->GetAttributeTable(0,&numEntries);
	std::vector<D3DXATTRIBUTERANGE> table(numEntries);
	mesh->GetAttributeTable(&table[0],&numEntries);
	&table[0].AttribId;
	/*&table[0].FaceCount;
	&table[0].VertexCount;
	for (int i = 0; i < numEntries; i++)
	{
		table[i].AttribId;
		table[i].FaceCount;
		table[i].VertexCount;
	}*/

	D3DXVECTOR3 dir(1.0f,0.0f,0.0f);
	D3DXCOLOR c = d3d::WHITE;
	D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &c);
	Device->SetLight(0,&light);
	Device->LightEnable(0,true);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS,true);

	D3DXVECTOR3 position(0.0f, 2.0f, -20.0f);
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

	Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);

	Device -> SetRenderState(D3DRS_LIGHTING,true);

	return true;
}

void Cleanup()
{
	d3d::Release<ID3DXMesh*>(mesh);
	for (int i = 0; i < Tex.size(); i++)
		d3d::Release<IDirect3DTexture9*>(Tex[i]);
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

		D3DXMATRIX Ry;
		static float y = 0.0f;
		D3DXMatrixRotationY(&Ry,y);
		y += timeDelta;

		Device->SetTransform(D3DTS_WORLD,&Ry);

		for(int i = 0; i < Mtrls.size(); i++)
		{
			Device->SetMaterial(&Mtrls[i]);
			Device->SetTexture(0,Tex[i]);
			mesh->DrawSubset( i );
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