#ifndef _D3DFRAME_H
#define _D3DFRAME_H

#include <d3dx9.h>
#include <d3dx9.h>
#include <string.h>
#include <float.h> 
#include <stdio.h>
#include <stdlib.h>


namespace d3d
{
	bool InitD3D(HWND hwnd, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device); 
	int EnterMsgLoop(bool(*ptr_display)(float));
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	
	template<class T> void Release(T t)
	{
		if(t){
			t->Release();
			t = 0;
		}
	}
	template<class T> void Delete(T t)
	{
		if(t){
			delete t;
			t = 0;
		}
	}
	const D3DXCOLOR 
	WHITE   (D3DCOLOR_XRGB(255, 255, 255)),
	BLACK   (D3DCOLOR_XRGB(  0,   0,   0)),
	RED     (D3DCOLOR_XRGB(255,   0,   0)),
	GREEN   (D3DCOLOR_XRGB(  0, 255,   0)),
	BLUE    (D3DCOLOR_XRGB(  0,   0, 255)),
	YELLOW  (D3DCOLOR_XRGB(255, 255,   0)),
	CYAN    (D3DCOLOR_XRGB(  0, 255, 255)),
	MAGENTA (D3DCOLOR_XRGB(255,   0, 255));
	
	D3DLIGHT9 InitPointLight(D3DXVECTOR3*, D3DXCOLOR*);
	D3DMATERIAL9 InitMtrl(D3DXCOLOR, D3DXCOLOR, D3DXCOLOR, D3DXCOLOR, float);
	
	const D3DMATERIAL9
	RED_MTRL    = InitMtrl(RED, RED, 
	                       D3DCOLOR_XRGB(255, 192, 192), D3DCOLOR_XRGB(0, 0, 0), 3.0f),
	REDDER_MTRL = InitMtrl(D3DCOLOR_XRGB(255, 63, 63), D3DCOLOR_XRGB(255, 63, 63),
	                       D3DCOLOR_XRGB(255, 192, 192), D3DCOLOR_XRGB(128, 0, 0), 7.0f),
	GREEN_MTRL  = InitMtrl(GREEN, GREEN,
	                       D3DCOLOR_XRGB(192, 255, 192), D3DCOLOR_XRGB(0, 63, 0), 5.0f),
	BLUE_MTRL   = InitMtrl(BLUE, BLUE,
	                       D3DCOLOR_XRGB(192, 192, 255), D3DCOLOR_XRGB(0, 0, 0), 3.0f),
	BLUER_MTRL  = InitMtrl(D3DCOLOR_XRGB(63, 63, 255), D3DCOLOR_XRGB(63, 63, 255),
	                       D3DCOLOR_XRGB(192, 192, 255), D3DCOLOR_XRGB(0, 0, 128), 7.0f),
	GREY_MTRL   = InitMtrl(D3DCOLOR_XRGB(63, 127, 63), D3DCOLOR_XRGB(127, 127, 127),
	                       D3DCOLOR_XRGB(127, 127, 127), BLACK, 1.0f);
	
	struct Ray
	{
		D3DXVECTOR3 _origin, _direction;
	};
	const float INFINITY_ = FLT_MAX, EPSILON = 0.001f;
}

bool Setup();
void Cleanup();
bool Display(float);
d3d::Ray CalcPickingRay(int, int);
void TransformRay(d3d::Ray*, D3DXMATRIX*);
float calcdist2(D3DXVECTOR3*, d3d::Ray* );


//为了节省代码量 
#define defijk          int i,j,k
#define forijk          for(i=0;i<=2*N;i++)for(j=0;j<=2*N;j++)for(k=0;k<=2*N;k++)
#define abs(x_)         ((x_)>0?(x_):-(x_))
#define REPORTERROR(P)  {MessageBox(0, P "错误！",NULL,MB_ICONERROR|MB_OK);return 0;}
#define F(i)            -size+i*(size/N)

#endif // _D3DFRAME_H
