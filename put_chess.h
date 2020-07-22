#ifndef _PUT_CHESS_H
#define _PUT_CHESS_H

void play_init();
void put(int x, int y, int z);
void pass();
void highlight(int i0, int j0, int k0);
bool cancel();
int calcscore(bool putchess);
int inform(HWND hwnd);
void confirm();

#endif 
