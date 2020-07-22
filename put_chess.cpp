#include "d3dframe.h"
#include "var.h"

const int self[]={1,30,160,2500,125000,100000000};
const int other[]={0,15,64,500,16000,1000000};//���� 

void play_init()
{
	play_round = status = 0;
	srand(timeGetTime());
	
	ZeroMemory(&p,    sizeof(p));
	ZeroMemory(state, sizeof(state));
	ZeroMemory(score, sizeof(score));
}
//������� 
float ran()
{
	return 1.0f + (rand() - 32765) / 500000.0f;
}

bool incoord(int i, int j, int k)
{
	return (0<=i && i<=2*N   
	   &&   0<=j && j<=2*N   
	   &&   0<=k && k<=2*N);
}

void put(int x, int y, int z)
{
	if(state[x][y][z] <= 0)
		state[x][y][z] = ++play_round;
}

void pass()
{
	++play_round;
}

void highlight(int i0, int j0, int k0)
{
	int i, j, k;
	forijk 
		if(state[i][j][k] < 0) 
			state[i][j][k] = 0;
	if(state[i0][j0][k0] == 0)
		state[i0][j0][k0] = -1;
}

bool cancel()
{
	int i,j,k;
	if(play_round <= 1)
	{
		MessageBox(0, "���ó�����", 0, 0); 
		return false;
	}
	play_round -= 2;
	forijk 
		if(state[i][j][k] > play_round) 
			state[i][j][k] = 0;
	
	status = 0;
	return true;
}
//����һ�����ӵĵ÷� 
void calcsingleline(int dx, int dy, int dz, const int x0, const int y0, const int z0)
{
	int x1 = x0, y1 = y0, z1 = z0, p = play_round % 2;
	if((!incoord(x0, y0, z0)) 
	|| (!incoord(x0 + (T-1) * dx,y0 + (T-1) * dy,z0 + (T-1) * dz)) 
	|| (dx==0 && dy==0 && dz==0))
		return;//��ô�����ܣ����� 
	int n1=0, n2=0, n0=0;
	//n1���ҷ����Ӹ�����n2���Է����Ӹ�����n0����λ���� 
	for(int i = 1; i <= T; i++)
	{
		if(state[x1][y1][z1] > 0)
		{
			if(state[x1][y1][z1]%2 == p) 
			     n1++; 
			else n2++;
		}
		else     n0++;
		x1 += dx;
		y1 += dy;
		z1 += dz;
	}
	
	x1 = x0;
	y1 = y0;
	z1 = z0;
	//���ǿ�λ���ӷ� 
	if(n1 == 0)
		for(int i = 1; i <= T; i++)
		{
			if(state[x1][y1][z1] <= 0)
				score[x1][y1][z1] += self[n2];
			x1 += dx;
			y1 += dy;
			z1 += dz;
		}
	else if(n2 == 0)
		for(int i = 1; i <= T; i++)
		{
			if(state[x1][y1][z1] <= 0)
				score[x1][y1][z1] += other[n1];
			x1 += dx;
			y1 += dy;
			z1 += dz;
		}
	//n1��n2����Ϊ0���򲻻��� 
}
int calcscore(bool putchess)//�������л���֮�Ͳ����� 
{
	ZeroMemory(score, sizeof(score));
	int i, j, k, di, dj, dk, max = -1;
	
	for(di = -1; di <= 1; di++)
	for(dj = -1; dj <= 1; dj++)
	for(dk = -1; dk <= 1; dk++)
		if(abs(di)+abs(dj)+abs(dk) == 1  ||  abs(di)+abs(dj)+abs(dk) == 2) 
			forijk  
				calcsingleline(di, dj, dk, i, j, k);
	
	forijk
	{
		if(state[i][j][k] <= 0)
		{	
			score[i][j][k] += 2;
			score[i][j][k] *= ran();
		}
	 	if(score[i][j][k] > max)
		{
			max = score[i][j][k];
			di = i;
			dj = j;
			dk = k;
		}
	}
	
	if(putchess)
		put(di,dj,dk);
	else 
		highlight(di,dj,dk);
	
	return 0;
}
bool same(int i, int j, int k, int di, int dj, int dk)
{
	int c;
	for(c = 1; c <= T-1; c++)
		if((state[i][j][k] % 2) != (state[i+c*di][j+c*dj][k+c*dk] % 2) 
		 || state[i][j][k] <= 0 ||  state[i+c*di][j+c*dj][k+c*dk] <=0)
		 	return false;
	return true;
}
int judge()//�ж�ʤ�� 
{
	int i, j, k, di, dj, dk, c = 3;
	forijk 
		if(state[i][j][k]<=0)
			c=0;//����ȷ�� 
	for(di = -1; di <= 1; di++)
	for(dj = -1; dj <= 1; dj++)
	for(dk = -1; dk <= 1; dk++)
		if(abs(di) + abs(dj) + abs(dk) == 1 
		|| abs(di) + abs(dj) + abs(dk) == 2) 
			forijk 
				if(incoord(             i,              j,              k)
				&& incoord(i + (T-1) * di, j + (T-1) * dj, k + (T-1) * dk)//�⼸���㶼�������� 
				&& same(i, j, k, di, dj, dk))//����� 
				{
					p.x1 = i;
					p.y1 = j;
					p.z1 = k;
					p.x2 = i + (T-1) * di;
					p.y2 = j + (T-1) * dj;
					p.z2 = k + (T-1) * dk;
					return ((state[i][j][k]%2==1)?1:2);//1:�췽ʤ 2:����ʤ
				}
	return c;		//3:ƽ�� 
}
int inform(HWND hwnd)//��֪�û� 
{
	if(status == 0)
	switch(status = judge())
	{
		case 1:
			if(MessageBox (hwnd, TEXT ("�췽ʤ��������"), 0, 
			 MB_ICONINFORMATION|MB_YESNO) == IDYES) cancel();
			 break;
		case 2:
			if(MessageBox (hwnd, TEXT ("����ʤ��������"), 0, 
			 MB_ICONINFORMATION|MB_YESNO) == IDYES) cancel();
			 break;
		case 3:
			if(MessageBox (hwnd, TEXT ("ƽ���ˣ�������"), 0, 
			 MB_ICONINFORMATION|MB_YESNO)==IDYES) play_init();
			 break;
	}
	return status;
}
void confirm()
{
	int i, j, k;
	if(inform(0) == 0) 
		forijk 
			if(state[i][j][k] == -1)
			{
				put(i, j, k);
				return;
			}
}
