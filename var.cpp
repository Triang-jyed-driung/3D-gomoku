#include <d3dx9.h>
 
int Width = 1080, Height = 720;//���ڵĿ�͸� 
short play_round, status = 0;//�����������״̬ 
float size = 2.0f;
char N = 4, T = 5, player1 = 0, player2 = 1;//���̴�С���������Ӹ������Ⱥ���
IDirect3DDevice9* Device = 0; 
short state[25][25][25];
int   score[25][25][25];

struct __{int x1, y1, z1, x2, y2, z2;} p;//���ߵ������յ� 

float angle = (1.5f * D3DX_PI), theta = 0, dist = 7.0f;//����Ƕ� 

