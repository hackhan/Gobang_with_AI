#include <graphics.h>
#include <stdlib.h>
#include <windows.h>
#include "gobang.h"

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/*
 * 用一个二维数组表示棋盘上每个落子点的状态，
 * 0 代表无子，1 代表黑子，2 代表白子
 */
int position[CK_SIZE][CK_SIZE];

/*
 * 记录当前该下哪种棋子，值为 true 时该黑子下，
 * 否则该白子下
 */
bool setblack = true;

int main() {
	initgraph(WINDOW_SIZE, WINDOW_SIZE);
	setcaption("五子棋");
	setbkcolor(EGERGB(218, 165, 105));

	bool flag = select_order();
	draw_board();

	// 对弈前，先清空鼠标消息
	mouse_msg temp;
	while (mousemsg()) temp = getmouse();

	int x = -1, y = -1;
	do {
		if (flag) {
			ai(x, y);
		} else {
			mouse_msg msg = { 0 };
			for ( ; is_run(); delay_fps(60)) {
				while (mousemsg())
					msg = getmouse();
				if (msg.is_up()) {
					x = msg.x / UNIT_SIZE;
					y = msg.y / UNIT_SIZE;
					break;
				}
			}
		}
		if (set_chess(x, y))
			flag = !flag;
	} while (!is_gameover(x, y));

	Sleep(100);
	MessageBox(NULL, setblack ? "白子胜！" : "黑子胜！",
			   "Game Over", MB_OK | MB_ICONINFORMATION);
	closegraph();
	return 0;
}

/*
 * 选择对战次序(电脑先下 / 玩家先下)
 * 如果选择电脑先下则返回 true，否则返回 false
 */
bool select_order() {
	setcolor(EGERGB(0, 0, 0));
	setfont(-19, 0, "宋体");
	outtextxy(6 * UNIT_SIZE - 4, 6 * UNIT_SIZE, "● 电脑先下");
	outtextxy(6 * UNIT_SIZE - 4, 7 * UNIT_SIZE, "○ 玩家先下");
	setfont(-16, 0, "宋体");
	outtextxy(9 * UNIT_SIZE, 14 * UNIT_SIZE, "↑ ↓ 选择  Enter 确定");
	setfont(-19, 0, "宋体");

	bool flag = true;
	for (key_msg k = { 0 }; k = getkey(), k.key != key_esc; ) {
		switch (k.key) {
			case 38: outtextxy(6 * UNIT_SIZE - 4, 6 * UNIT_SIZE, "●");
					 outtextxy(6 * UNIT_SIZE - 4, 7 * UNIT_SIZE, "○");
					 flag = true;
					 break;
			case 40: outtextxy(6 * UNIT_SIZE - 4, 6 * UNIT_SIZE, "○");
					 outtextxy(6 * UNIT_SIZE - 4, 7 * UNIT_SIZE, "●");
					 flag = false;
					 break;
			case 13: cleardevice();
					 return flag;
		}
	}
	exit(0);
}


/*
 * 绘制棋盘
 */
void draw_board() {
	setcolor(EGERGB(0, 0, 0));
	setfillcolor(EGERGB(0, 0, 0));
	
	// 绘制网格线
	for (int i = 0; i < CK_SIZE; i++) {
		bar(CELL_SIZE / 2 - 1, 
			CELL_SIZE / 2 - 1 + UNIT_SIZE * i,
			CELL_SIZE / 2 + 1 + WINDOW_SIZE - CELL_SIZE,
			CELL_SIZE / 2 + 1 + UNIT_SIZE * i);
	}
	for (int i = 0; i < CK_SIZE; i++) {
		bar(CELL_SIZE / 2 - 1 + UNIT_SIZE * i,
			CELL_SIZE / 2 - 1,
			CELL_SIZE / 2 + 1 + UNIT_SIZE * i,
			CELL_SIZE / 2 + 1 + WINDOW_SIZE - CELL_SIZE);
	}

	
	// 绘制天元及周围四个星位的黑点
	PIMAGE img = newimage();
	getimage(img, "res/。.ico");
	putimage(CK_SIZE / 2 * UNIT_SIZE, CK_SIZE / 2 * UNIT_SIZE, img);
	putimage(3 * UNIT_SIZE, 3 * UNIT_SIZE, img);
	putimage((CK_SIZE - 4) * UNIT_SIZE, 3 * UNIT_SIZE, img);
	putimage(3 * UNIT_SIZE, (CK_SIZE - 4) * UNIT_SIZE, img);
	putimage((CK_SIZE - 4) * UNIT_SIZE, (CK_SIZE - 4) * UNIT_SIZE, img);

	delimage(img);
}

bool set_chess(int x, int y) {
	if (position[y][x] != 0) return false;

	PIMAGE img = newimage();
	getimage(img, setblack ? "res/black.ico" : "res/white.ico");
	putimage(x * UNIT_SIZE, y * UNIT_SIZE, img);
	setblack ? (position[y][x] = 1) : (position[y][x] = 2);
	setblack = !setblack;
	return true;
}

bool is_gameover(int x, int y) {
	vector<int> five(5, setblack ? 2 : 1);
	vector<int> ivec;

#ifdef DEBUG
	for (auto it : five) 
		cout << it << " ";
	cout << "\n";
#endif	
	/*
	 * 将 position 数组的第 y 行拷贝到 ivec 中，然后搜索该行是否出现
	 * 了连续相同的五个元素
	 */
	copy(position[y], position[y] + CK_SIZE, back_inserter(ivec));
	auto it = search(ivec.begin(), ivec.end(), five.begin(), five.end());
	if (it != ivec.end()) return true;
	ivec.clear();

	/*
	 * 将 position 数组第 x 列拷贝到 ivec 中，然后搜索该列是否出现
	 * 了连续相同的五个元素
	 */
	for (int i = 0; i < CK_SIZE; i++) {
		ivec.push_back(position[i][x]);
	}
	it = search(ivec.begin(), ivec.end(), five.begin(), five.end());
	if (it != ivec.end()) return true;
	ivec.clear();

	/*
	 * 将 position[y][x] 所在的 "\" 方向斜线拷贝到 ivec 中，搜索
	 * 是否出现连续相同的五个元素
	 */
	int _x, _y;
	x < y ? (_x = x - x, _y = y - x) : (_x = x - y, _y = y - y);
	for ( ; _x < CK_SIZE && _y < CK_SIZE; _x++, _y++) {
		ivec.push_back(position[_y][_x]);
	}
	it = search(ivec.begin(), ivec.end(), five.begin(), five.end());
	if (it != ivec.end()) return true;
	ivec.clear();

	/*
	 * 将 position[y][x] 所在的 "/" 方向斜线拷贝到 ivec 中，搜索
	 * 是否出现连续相同的五个元素
	 */
	_x = x;
	_y = y;
	while (_x != 0 && _y != CK_SIZE - 1) {
		_x--;
		_y++;
	}
	for (; _x < CK_SIZE && _y >= 0; _x++, _y--)	{
		ivec.push_back(position[_y][_x]);
	}
#ifdef DEBUG
	cout << "ivec: ";
	for (auto it : ivec) 
		cout << it << " ";
	cout << "\n";
#endif
	it = search(ivec.begin(), ivec.end(), five.begin(), five.end());
	if (it != ivec.end()) return true;

	return false;
}

