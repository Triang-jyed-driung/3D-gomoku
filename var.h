#ifndef _VAR_H
#define _VAR_H 

extern int Width, Height;//���ڵĿ�͸� 
extern short play_round, status;//�����������״̬ 
extern const float size;
extern IDirect3DDevice9* Device; 
extern char N, T, player1, player2;//���̴�С���������Ӹ������Ⱥ���

extern short state[25][25][25];
extern int   score[25][25][25];

extern struct __{int x1, y1, z1, x2, y2, z2;} p;//���ߵ������յ� 

extern float angle, theta, dist;//����Ƕ� 

#endif
