#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define randomize() srand((unsigned)time(NULL))
#define random(n) (rand() % (n))
#define delay(n) Sleep(n)
#define clrscr() system("cls")
#define gotoxy(x,y) { COORD Cur = {x, y}; \
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);}
#define showcursor(bShow) { CONSOLE_CURSOR_INFO CurInfo = {20, bShow}; \
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo); }


enum { ESC = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };
#define putsxy(x, y, s) {gotoxy(x, y);puts(s);}
#define BX 5
#define BY 1
#define BW 10
#define BH 20

void DrawScreen();
BOOL ProcessKey();
void PrintBrick(BOOL Show);
void PrintNextBrick();
void holdBrick();
int GetAround(int x, int y, int b, int r);
BOOL MoveDown();
void TestFull();
void pushBrick();
void loopGame(int nFrame);
void initGame();
void plusDelay(int delta);
int createBrick(int beforeBrick);
void gameOver(int nFrame);

struct Point {
	int x, y;
};
struct Point Shape[][4][4] = {
	{ { 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 },{ 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 } },
	{ { 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 } },
	{ { 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 },{ 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 } },
	{ { 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 },{ 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 } },
	{ { 0,0,-1,0,1,0,-1,-1 },{ 0,0,0,-1,0,1,-1,1 },{ 0,0,-1,0,1,0,1,1 },{ 0,0,0,-1,0,1,1,-1 } },
	{ { 0,0,1,0,-1,0,1,-1 },{ 0,0,0,1,0,-1,-1,-1 },{ 0,0,1,0,-1,0,-1,1 },{ 0,0,0,-1,0,1,1,1 } },
	{ { 0,0,-1,0,1,0,0,1 },{ 0,0,0,-1,0,1,1,0 },{ 0,0,-1,0,1,0,0,-1 },{ 0,0,-1,0,0,-1,0,1 } },
};

enum { EMPTY, BRICK, WALL };
char arTile[3][4] = { ". ","��","��" };
int board[BW + 2][BH + 2];
int infoBoard[6][BH + 2];
int holdBoard[6][7];
int nx, ny;
int brick, rot;
int next[4];
int hold=7;
int gameDelay;
int line=0;

int main()
{
	int nFrame = 20;
	showcursor(FALSE);
	// ��ü ���� ����
	loopGame(nFrame);
	showcursor(TRUE);
	return 0;
}

void DrawScreen()
{
	for (int x = 0; x < BW + 7; x++) {
		for (int y = 0; y < BH + 2; y++) {
			if (x < BW + 2) {
				putsxy(BX + x * 2, BY + y, arTile[board[x][y]]);
			}
			else {
				putsxy(BX + x * 2, BY + y, arTile[infoBoard[x- (BW + 1) ][y]]);
			}
			
		}
	}
	for (int x = BW + 6; x < BW + 12; x++) {
		for (int y = 0; y < 7; y++) {
				putsxy(BX + x * 2, BY + y, arTile[holdBoard[x - (BW + 6)][y]]);
		}
	}
	char buffer[3];
	_itoa_s(gameDelay, buffer, 3, 10);
	putsxy(55, 3, "Tetris Ver 1.2");
	putsxy(55, 5, "�¿�:�̵�, ��:ȸ��, �Ʒ�:����");
	putsxy(55, 7, "�����̽� ��:���� ����, a:Ȧ��");
	putsxy(55, 9, "gameDelay : ");
	putsxy(67, 9, buffer);
	putsxy(29, 2, "n e x t");
	putsxy(39, 2, "h o l d");
	PrintNextBrick();
}

BOOL ProcessKey()
{
	if (kbhit()) {
		int ch = getch();
		if (ch == 0xE0 || ch == 0) {
			ch = getch();
			switch (ch) {
			case LEFT:
				if (GetAround(nx - 1, ny, brick, rot) == EMPTY) {
					PrintBrick(FALSE);
					nx--;
					PrintBrick(TRUE);
				}
				break;
			case RIGHT:
				if (GetAround(nx + 1, ny, brick, rot) == EMPTY) {
					PrintBrick(FALSE);
					nx++;
					PrintBrick(TRUE);
				}
				break;
			case UP:
				if (GetAround(nx, ny, brick, (rot + 1) % 4) == EMPTY) {
					PrintBrick(FALSE);
					rot = (rot + 1) % 4;
					PrintBrick(TRUE);
				}
				break;
			case DOWN:
				if (MoveDown()) {
					return TRUE;
				}
				break;
			}
		}
		else {
			switch (ch) {
			case ' ':
				while (MoveDown() == FALSE) { ; }
				return TRUE;
			case 'A':
			case 'a':
				if (hold==7) {
					holdBrick();
				}
				else if(GetAround(nx, ny, hold, 0) == EMPTY) {
					holdBrick();
				}
				break;
			case ESC:
				exit(0);
			}
		}
	}
	return FALSE;
}

void PrintBrick(BOOL Show)
{
	for (int i = 0; i < 4; i++) {
		gotoxy(BX + (Shape[brick][rot][i].x + nx) * 2, BY + Shape[brick][rot][i].y + ny);
		puts(arTile[Show ? BRICK : EMPTY]);
	}
}
void PrintNextBrick()
{
	//���� ������ ǥ���ϱ� ���� �Լ�
	//������ ǥ���ϱ� ���� ��ҿ� �ִ� ������ �����
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				putsxy(29 + (i) * 2, 3 + (5 * j) + k, arTile[EMPTY]);
			}
		}
	}
	//���� ������ ǥ���Ѵ�
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			gotoxy(31 + (Shape[next[i]][0][j].x) * 2, (5+(5*i)) + Shape[next[i]][0][j].y );
			puts(arTile[BRICK]);
		}
	}
}

void holdBrick()
{
	
	//������ �����ϰ� �������� �ִ� ������ ��ȯ�ϴ� �Լ�
	if (hold == 7) {
		//������ ������ 7���̹Ƿ� �����Ҽ� ���� ������ �̿��� Ȧ�带 ���� ���� �ʱⰪ���� ǥ��
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				putsxy(39 + (i) * 2, 3 + j, arTile[EMPTY]);
			}
		}
		hold = brick;
		rot = 0;
		for (int i = 0; i < 4; i++) {
			gotoxy(41 + (Shape[hold][0][i].x) * 2, 5  + Shape[hold][0][i].y);
			puts(arTile[BRICK]);
		}
		PrintBrick(FALSE);
		pushBrick();
		PrintNextBrick();
		PrintBrick(TRUE);
	}
	else {
		//������ ���� ��� Ȧ��� ������ ���� ���� �ٲ� �ٸ� ������ �������� ��
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				putsxy(39 + (i) * 2, 3 + j, arTile[EMPTY]);
			}
		}
		PrintBrick(FALSE);
		int tmp = hold;
		hold = brick;
		brick = tmp;
		rot = 0;
		PrintBrick(TRUE);
		for (int i = 0; i < 4; i++) {
			gotoxy(41 + (Shape[hold][0][i].x) * 2, 5 + Shape[hold][0][i].y);
			puts(arTile[BRICK]);
		}
	}
}

int GetAround(int x, int y, int b, int r)
{
	int k = EMPTY;

	// ������ ������ Ÿ�� ��� �� ���� ū ���� ã�´�.
	for (int i = 0; i < 4; i++) {
		k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	}
	return k;
}

BOOL MoveDown()
{
	// �ٴڿ� ������� ����á���� �����ϰ� TRUE�� �����Ѵ�.
	if (GetAround(nx, ny + 1, brick, rot) != EMPTY) {
		TestFull();
		return TRUE;
	}
	// ���� ���߿� �� ������ ��ĭ �Ʒ��� ������.
	PrintBrick(FALSE);
	ny++;
	PrintBrick(TRUE);
	return FALSE;
}

void TestFull()
{
	// �ٴڿ� �������� ���� ���
	for (int i = 0; i < 4; i++) {
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = BRICK;
	}

	// �������� ������ ���� ����
	for (int y = 1; y < BH + 1; y++) {
		BOOL bFull = TRUE;
		for (int x = 1; x < BW + 1; x++) {
			if (board[x][y] == EMPTY) {
				bFull = FALSE;
				break;
			}
		}
		// ������ ���� á���� �� ���� �����Ѵ�.
		if (bFull) {
			for (int ty = y; ty > 1; ty--) {
				for (int x = 1; x < BW + 1; x++) {
					board[x][ty] = board[x][ty - 1];
				}
			}
			line++;
			if (line >= 1) {
				plusDelay(2);
				line = 0;
			}
			DrawScreen();
			delay(200);
		}
	}
}

void pushBrick() {
	//������ ���� 1���� �о�ְ� �� ������ �� �� ������ ����� �Լ�
	brick = next[0];
	next[0] = next[1];
	next[1] = next[2];
	next[2] = next[3];
	next[3] = createBrick(next[2]);
}

void loopGame(int nFrame) {
	//������ �ݺ��Ǵ� ������ �Լ�ȭ
	int nStay;
	initGame(nFrame);
	for (; 1;) {
		pushBrick();
		nx = BW / 2;
		ny = 3;
		rot = 0;
		PrintBrick(TRUE);
		PrintNextBrick();
		if (GetAround(nx, ny, brick, rot) != EMPTY) break;

		// ���� �ϳ��� �ٴڿ� ���� �������� ����
		nStay = nFrame;
		for (; 2;) {
			if (--nStay == 0) {
				nStay = nFrame;
				if (MoveDown()) break;
			}
			if (ProcessKey()) break;
			delay(gameDelay);
		}
	}
	clrscr();
	gameOver(nFrame);
}

void initGame(int initFrame) {
	//������ ���¸� �ʱ�ȭ�ϴ� �Լ�
	gameDelay = 1000 / initFrame;
	int x, y;
	randomize();
	clrscr();
	// �����ڸ��� ��, �������� �� �������� �ʱ�ȭ�Ѵ�.
	for (x = 0; x < BW + 2; x++) {
		for (y = 0; y < BH + 2; y++) {
			if (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) {
				board[x][y] = WALL;
			}
			else {
				board[x][y] = EMPTY;
			}
		}
	}
	//���� ������ ǥ�õ� ������ �ʱ�ȭ
	for (x = 0; x < 6; x++) {
		for (y = 0; y < BH + 2; y++) {
			if (y == 0 || (y > 1 && ((y % 5) == 1)) || y == BH + 1 || x == 0 || x == 5) {
				infoBoard[x][y] = WALL;
			}
			else {
				infoBoard[x][y] = EMPTY;
			}
		}
	}
	//Ȧ�尡 ǥ�õ� ������ �ʱ�ȭ
	for (x = 0; x < 6; x++) {
		for (y = 0; y < 7; y++) {
			if (y == 0 || y == 6 || x == 0 || x == 5) {
				holdBoard[x][y] = WALL;
			}
			else {
				holdBoard[x][y] = EMPTY;
			}
		}
	}
	DrawScreen();
	//���� ������ �ʱ�ȭ

	for (int i = 0;i < 3;i++) {
		if (i == 0) {
			next[i]= random(sizeof(Shape) / sizeof(Shape[0]));
		}
		else {
			next[i] = createBrick(next[i - 1]);
		}
	}
}

void plusDelay(int delta) {
	//������ ���ǵ带 �ٲٱ� ���� �Լ�
	if (gameDelay - delta < 10) {
		return;
	}
	else if (gameDelay - delta >= 50) {
		return;
	}
	else {
		gameDelay -= delta;
	}
}

int createBrick(int beforeBrick) {
	//�ٷ��� ������ �ٸ� ����� ������ ����� ���� �Լ�
	int brick = random(sizeof(Shape) / sizeof(Shape[0]));
	if (beforeBrick==brick) {
		int newBrick = random(sizeof(Shape) / sizeof(Shape[0])-1);
		if (brick <= newBrick) {
			brick = brick + 1;
		}
		else {
			brick = newBrick;
		}
	}
	return brick;
}

void gameOver(int nFrame) {
	//���ӿ����� �Ǿ��� ����� ȭ���� ǥ���ϱ� ���� �Լ�
	BOOL isKeyDown = FALSE;
	putsxy(15, 10, "G A M E   O V E R");
	putsxy(18, 12, "R E T R Y ?");
	putsxy(21, 14, "Y / N");
	do {
		if (kbhit()) {
			int ch = getch();
			switch (ch) {
			case 'Y':
			case 'y':
				loopGame(nFrame);
				break;
			case 'N':
			case 'n':
				isKeyDown = TRUE;
				break;
			case ESC:
				exit(0);
			}
		}
	} while (!isKeyDown);
}