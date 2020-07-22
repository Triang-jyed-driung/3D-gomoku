#include "D3Dframe.h" 
#include "var.h"
//3D基本函数的文件 
ID3DXMesh* small_ball;
ID3DXMesh* big_ball;
ID3DXMesh* light_ball;
ID3DXMesh* line_cylinder;
D3DXMATRIX Worlds[25][25][25], temp;
D3DMATERIAL9 grey, green, red, redder, blue, bluer;

bool d3d::InitD3D(HWND hwnd, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device)
{//初始化Direct3D 
	HRESULT hr = 0;
	IDirect3D9* d3d9 = 0;
    d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
    if(!d3d9)
		REPORTERROR("Direct3DCreate9()");
    
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);
	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		 vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = Width;
	d3dpp.BackBufferHeight           = Height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = windowed;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
	
	hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, device);
	if( FAILED(hr) )
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, device);
		if( FAILED(hr) ){
			d3d9->Release();
			REPORTERROR("CreateDevice()");
		}
	}
	d3d9->Release();
	return true;
}
int d3d::EnterMsgLoop(bool(*ptr_display)(float))
{
	MSG msg;
	int currTime;
	ZeroMemory(&msg, sizeof(MSG));
	static int lastTime = timeGetTime(); 
	while(msg.message != WM_QUIT)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			currTime = timeGetTime();
			ptr_display((currTime - lastTime) * 0.001f);
			lastTime = currTime;
		}
    }
    return msg.wParam;
}

D3DLIGHT9 d3d::InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* col)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type      = D3DLIGHT_POINT;
	light.Ambient   = *col * 0.85f;
	light.Diffuse   = *col * 0.45f;
	light.Specular  = *col * 1.2f;
	light.Position  = *position;
	light.Range        = 500.0f;
	light.Falloff      = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.04f;
	light.Attenuation2 = 0;
	return light;
}

D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = a;
	mtrl.Diffuse  = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power    = p;
	return mtrl;
}

bool Setup()
{
	int i, j, k;
	timeBeginPeriod(1);
	small_ball = big_ball = light_ball = line_cylinder = 0;
	D3DXCreateSphere  (Device, size / (20.0f * N), 10, 10, &small_ball,    0);
	D3DXCreateSphere  (Device, size / (3.0f  * N), 40, 40, &big_ball,      0);
	D3DXCreateSphere  (Device, size / (2.8f  * N), 40, 40, &light_ball,    0);
	D3DXCreateCylinder(Device, size / (12.0f * N), size/(12.0f*N), 2.0f,
	                                               20, 1,  &line_cylinder, 0);
	forijk 
		D3DXMatrixTranslation(&Worlds[i][j][k], F(i), F(j), F(k));
		
	grey  = d3d::GREY_MTRL;
	red   = d3d::RED_MTRL;
	blue  = d3d::BLUE_MTRL;
	green = d3d::GREEN_MTRL;
	redder= d3d::REDDER_MTRL;
	bluer = d3d::BLUER_MTRL;
	
	D3DXVECTOR3 pos(-10.0f, 6.0f, -6.0f);
	D3DXCOLOR c = d3d::WHITE;
	D3DLIGHT9 point = d3d::InitPointLight(&pos, &c);
	Device->SetLight(0, &point);
	Device->LightEnable(0, true);
	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.25f, 
	                          (float) Width / (float) Height, 1.0f, 400.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);
	return true;
}

void Cleanup()
{
	d3d::Release <ID3DXMesh*> (small_ball);
	d3d::Release <ID3DXMesh*> (big_ball);
	d3d::Release <ID3DXMesh*> (light_ball);
	d3d::Release <ID3DXMesh*> (line_cylinder);
	timeEndPeriod(1);
}

#define PRESS(P) if(GetAsyncKeyState(VK_##P) & 0x8000f)
bool Display(float deltaT)
{
	int i, j, k;
	if(Device)
	{
		PRESS(LEFT)     angle -= 0.5f * deltaT; 
		PRESS(RIGHT)    angle += 0.5f * deltaT;
		PRESS(UP)       theta += 0.5f * deltaT;
		PRESS(DOWN)     theta -= 0.5f * deltaT;
		PRESS(OEM_PLUS) dist  -=        deltaT;
		PRESS(OEM_MINUS)dist  +=        deltaT;

		if(theta >  D3DX_PI / 2.0f) theta =  D3DX_PI / 2.0f - d3d::EPSILON;
		if(theta < -D3DX_PI / 2.0f) theta = -D3DX_PI / 2.0f + d3d::EPSILON;
		
		if(dist < 5.0f)
		{
			float l = 1.02f + sqrtf(3.0f) * size + size / (2.8f * N); 
			if(dist < l)dist = l;
		}
		if(dist > 20.0f)
			dist = 20.0f;
		D3DXVECTOR3 position(cosf(angle) * cosf(theta) * dist, 
		            sinf(theta) * dist, 
					sinf(angle) * cosf(theta) * dist),
		   target(0, 0, 0), 
		   up    (0, 1, 0);
		
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);
		
		Device->SetTransform(D3DTS_VIEW, &V);
		
		Device->Clear(0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x000000, 1, 0);
		
		Device->BeginScene();
		
		forijk
		{
			if     (state[i][j][k] ==  0)Device->SetMaterial(&grey);
			else if(state[i][j][k] == -1)Device->SetMaterial(&green);
			else if(state[i][j][k] % 2 == 1)
			{
				if(state[i][j][k] == play_round)
				     Device->SetMaterial(&redder);
				else Device->SetMaterial(&red);
			}
			else if(state[i][j][k] % 2 == 0)
			{
				if(state[i][j][k] == play_round)
				     Device->SetMaterial(&bluer);
				else Device->SetMaterial(&blue);
			}
			
			Device->SetTransform(D3DTS_WORLD, &Worlds[i][j][k]);
			if(state[i][j][k] <= 0)//画球 
				small_ball->DrawSubset(0);
			else if(state[i][j][k] != play_round)
				big_ball  ->DrawSubset(0);
			else 
				light_ball->DrawSubset(0);
		}
		//画圆柱 
		if((status==1 || status==2) && ((p.x1-p.x2) || (p.y1-p.y2) || (p.z1-p.z2)))
		{
			D3DXVECTOR3 
				a(F(p.x1), F(p.y1), F(p.z1)),
				b(F(p.x2), F(p.y2), F(p.z2)),
				c, d, d0;
			
			c = 0.5f * (a + b),
			d = 0.5f * (a - b);
			D3DXVec3Normalize (&d0, &d);
			D3DXVECTOR3 e0(d.y - d.z, d.z - d.x, d.x - d.y), f0;
			D3DXVec3Normalize (&e0, &e0);
			D3DXVec3Cross(&f0, &d0, &e0);
			D3DXMATRIX m(
			 e0.x, e0.y, e0.z, 0,
			 f0.x, f0.y, f0.z, 0,
 			  d.x,  d.y,  d.z, 0,
			  c.x,  c.y,  c.z, 1
			  );
			Device->SetMaterial(&green);
			Device->SetTransform(D3DTS_WORLD, &m);
			line_cylinder->DrawSubset(0);
		}
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true; 
}

//这里是计算点击投影的函数
d3d::Ray CalcPickingRay(int x, int y)
{
	D3DXMATRIX proj;
	Device->GetTransform(D3DTS_PROJECTION, &proj);
	d3d::Ray ray;
	ray._origin    = D3DXVECTOR3(0, 0, 0);
	ray._direction = D3DXVECTOR3( ((( 2.0f * x) /  Width) - 1) / proj(0, 0),
	                              (((-2.0f * y) / Height) + 1) / proj(1, 1), 1);
	return ray;
}

void TransformRay(d3d::Ray* ray, D3DXMATRIX* T)
{
	D3DXVec3TransformCoord (&ray->_origin,    &ray->_origin,    T);
	D3DXVec3TransformNormal(&ray->_direction, &ray->_direction, T);
	D3DXVec3Normalize      (&ray->_direction, &ray->_direction   );
}
//计算距离 
float calcdist2(D3DXVECTOR3* p, d3d::Ray* r)
{
	D3DXVECTOR3 *s = &(r->_origin), *t = &(r->_direction);
	D3DXVECTOR3 d = *s - *p;
	float a = D3DXVec3Dot(&d, t);
	return - (a * a) / ( D3DXVec3Dot(t, t) * D3DXVec3Dot(&d, &d) );
}


