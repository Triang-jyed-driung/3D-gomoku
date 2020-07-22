#include "d3dframe.h"
#include "defs.h" 
#include "var.h"
#include "put_chess.h"

//界面模块 

HWND InitMainWindow(HINSTANCE hInstance)
{ 
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = d3d::WndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, ICONNAME);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.lpszMenuName  = MENUNAME;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = APPNAME;
	if(!RegisterClass(&wc))
		REPORTERROR("RegisterClass()");
		
	HWND hwnd = CreateWindow(APPNAME, APPNAME, WS_OVERLAPPEDWINDOW, 
	                         CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	                         0, 0, hInstance, 0);
	
	if(!hwnd)
		REPORTERROR("CreateWindow()");
	
	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);
	
	RECT rect;
	GetClientRect(hwnd, &rect);
	Height = rect.bottom;
	Width  = rect.right;
	
	return hwnd;
}

#define READ(P)  fread (&P, sizeof(P), 1, fp)
#define WRITE(P) fwrite(&P, sizeof(P), 1, fp)
#define CHECK(P) CheckMenuItem(hmenu, P, MF_CHECKED)

void CorrectMenu(HMENU hmenu)
{
	int i; 
	for(i = MENUBEGIN; i <= MENUEND; i++)
		CheckMenuItem(hmenu, i, MF_UNCHECKED);

	CHECK(ID_HUMAN_1 + player1);
	CHECK(ID_HUMAN_2 + player2);
	CHECK(MENUSIZE + N + N + 1);
	CHECK(MENUWINNUM + T);
}

void SetOpenFile(OPENFILENAME* p, HWND hwnd, char* szFile)
{
	*szFile = 0;
	ZeroMemory(p, sizeof(OPENFILENAME));
	p->lStructSize  = sizeof(OPENFILENAME);
	p->hwndOwner    = hwnd;
	p->lpstrFile    = szFile;
	p->nMaxFile     = MAX_PATH;
	p->nFilterIndex = 1;
}

char* InitName(char* p)//去除双引号 
{
	char* q;
	if(*p == '"')
	{
		q = strdup(p + 1);
		if(*q)
			q[strlen(q) - 1] = 0;
	}
	else
		q = strdup(p);
	return q; 
}

bool OpenByName(char* file)//写文件 
{
	defijk;
	FILE* fp = fopen(file, "rb");
	if(!fp)
		REPORTERROR("文件");
	
	READ(N);
	READ(T);
	READ(player1);
	READ(player2);
	READ(play_round);
	READ(angle);
	READ(theta);
	READ(dist);
	forijk 
		READ(state[i][j][k]);
	
	fclose(fp);
	return 1;
}


LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)//窗口过程 
{
	int                 i, j, k;
	static int          i0, j0, k0;
	static HMENU        hmenu; 
	static int          lwparam, timecount = 0; 
	static char         szFile[MAX_PATH + 2] = "";
    static OPENFILENAME ofn;
    RECT                rect;
    
	switch(msg)
	{
	case WM_CREATE:
		timecount = 0;
		hmenu = GetMenu(hwnd);
		play_init();
		CorrectMenu(hmenu);
		break;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
			DestroyWindow(hwnd);
		if(wParam == VK_SPACE)
		{
			confirm();
			timecount = 0;
		}
		if(wParam == VK_DELETE)
		{
			cancel();
			timecount = -5;
		}
		break;
		
	case WM_TIMER:
		timecount++;
		if(timecount <= 6)
			break;
		timecount = 0;
		
		if(inform(hwnd) > 0)//判断胜负并告知 
			break;
		if ((player2 && play_round % 2 == 1)
		  ||(player1 && play_round % 2 == 0))
			calcscore(true);//算分并下棋 
		break;
		
	case WM_COMMAND:
		switch (lwparam = LOWORD(wParam))
		{//菜单处理 
			case ID_OPEN:
				SetOpenFile(&ofn, hwnd, szFile);
				ofn.lpstrFilter = "棋谱文件(*.gomoku)\0*.gomoku\0";
				ofn.lpstrDefExt = "gomoku";
				ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
				if(GetOpenFileName(&ofn))
				{
					OpenByName(szFile);
					Cleanup();
					Setup();
					CorrectMenu(hmenu);
				}
				else REPORTERROR("文件");
				break;
			case ID_SAVE:
				SetOpenFile(&ofn, hwnd, szFile);
				ofn.lpstrFilter = "棋谱文件(*.gomoku)\0*.gomoku\0";
				ofn.lpstrDefExt = "gomoku";
				if(GetSaveFileName(&ofn))
				{
					FILE* fp;
					if(! (fp = fopen(szFile,"wb")) ) REPORTERROR("文件");
					WRITE(N);
					WRITE(T);
					WRITE(player1);
					WRITE(player2);
					WRITE(play_round);
					WRITE(angle);
					WRITE(theta);
					WRITE(dist);
					forijk 
						WRITE(state[i][j][k]);
					fclose(fp);
				}
				else REPORTERROR("文件");
				break;
			case ID_SAVEIMAGE:
				SetOpenFile(&ofn, hwnd, szFile);
				ofn.lpstrFilter = "图片文件(*.jpg)\0*.jpg\0";
				ofn.lpstrDefExt = "jpg";
				if(GetSaveFileName(&ofn))
				{
					IDirect3DSurface9 *pBackBuffer;
					Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
					D3DXSaveSurfaceToFile(szFile, D3DXIFF_JPG, pBackBuffer, NULL, NULL);
					pBackBuffer->Release();
				}
				else REPORTERROR("文件");
				break;
			case ID_BEGIN: 
				SetTimer(hwnd, 1, 180, NULL); 
				break;
			case ID_STOP: 
				KillTimer(hwnd,1);
				break;
			case ID_DELETE: 
				cancel();
				break;
			case ID_PASS: 
				pass();
				break;
			case ID_REBEGIN: 
				play_init();
				break;
			case ID_HINT: 
				calcscore(false);
				break;
			case ID_INFO: 
				MessageBox(0,"3D五子棋0.5版\n\
				             规则：在nxnxn网格中，某个与坐标轴垂直的平面上，\
							 5个同色棋子连成一条横（竖、斜）线者胜。\n\
				             用鼠标点击格点，看到格点亮起时，再点击一次或按空格以放置棋子。\n\
				             使用上下左右箭头以调整方向。使用+/-以放大缩小。按delete可以悔棋。" ,
				             "规则", MB_ICONINFORMATION|MB_OK);
				break;
			case ID_EXIT:
				if(MessageBox(0, "确定退出吗？", 0, MB_ICONINFORMATION|MB_OKCANCEL) == IDOK)
					PostQuitMessage(0);
				break;
			case ID_HUMAN_1:
				player1 = 0;
				CorrectMenu(hmenu);
				break;
			case ID_MACHINE_1: 
				player1 = 1;
				CorrectMenu(hmenu);
				break;
			case ID_HUMAN_2:
				player2 = 0;
				CorrectMenu(hmenu);
				break;
			case ID_MACHINE_2:
				player2 = 1;
				CorrectMenu(hmenu);
				break;
			case ID_WINNUM_4:
				T=4;
				CorrectMenu(hmenu);
				break;
			case ID_WINNUM_5:
				T=5;
				CorrectMenu(hmenu);
				break;
			case ID_WINNUM_6:
				T=6;
				CorrectMenu(hmenu);
				break;
			case ID_WINNUM_7:
				T=7;
				CorrectMenu(hmenu);
				break;
			default:
				if(lwparam > MENUSIZE && lwparam <= ID_SIZE_MAX
				 && MessageBox(0, "确定清空棋盘吗？", 0, MB_ICONINFORMATION|MB_OKCANCEL) == IDOK)
				 {
					N = (lwparam - MENUSIZE) / 2;
					Setup();
					CorrectMenu(hmenu);
					break;
				}
		}
		break;
	case WM_SIZE:
		GetClientRect(hwnd, &rect);
		Width  = rect.right;
		Height = rect.bottom;
		break;
	case WM_LBUTTONDOWN: 
		d3d::Ray ray = CalcPickingRay(LOWORD(lParam), HIWORD(lParam));//计算投影射线 
		D3DXMATRIX view;
		Device->GetTransform(D3DTS_VIEW, &view);
		D3DXMATRIX viewInverse;
		D3DXMatrixInverse(&viewInverse,	0, &view);
		TransformRay(&ray, &viewInverse);//矩阵变换 
		float min = d3d::INFINITY_, tmp;
		forijk
		{//取夹角最小值 
			D3DXVECTOR3 p(F(i), F(j), F(k));
			if(state[i][j][k] <= 0 && (tmp = calcdist2(&p, &ray)) < min)
			{
				min = tmp;
				i0 = i;
				j0 = j;
				k0 = k;
			}
		}
		if(state[i0][j0][k0] == 0)
			highlight(i0, j0, k0);
		else
		{
			confirm();
			timecount = 0; 
		}
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//主函数 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   PSTR szCmd, int iCmdShow)
{
	HWND hwnd;
	if( !( hwnd = InitMainWindow(hInstance) ) ) 
		REPORTERROR("窗口");
	
	if(!d3d::InitD3D(hwnd, true, D3DDEVTYPE_HAL, &Device))
		REPORTERROR("InitD3D()");
	
	if(*szCmd)
	{
		char* p = InitName(szCmd);
		if(!OpenByName(p)) 
			REPORTERROR("文件");
		
		HMENU hmenu = GetMenu(hwnd);
		CorrectMenu(hmenu);
	}
	
	Setup();
	d3d::EnterMsgLoop( Display );
	
	Cleanup();
	Device->Release();
	
	return 0;
}
