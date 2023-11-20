#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define randomize() srand((unsigned)time(NULL))
#define random(n) (rand() % (n))
#define delay(n) Sleep(n)
#define clrscr() system("cls")
//���� ���� ǥ���� �ֱ� ���� �Լ��� ����ϱ� ���� ����
#define color(colorNum) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum)
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
#define WHITE 15

void DrawScreen();
BOOL ProcessKey();
void PrintBrick(BOOL Show);
void PrintNextBrick();
void PrintHold();
void holdBrick();
int GetAround(int x, int y, int b, int r);
BOOL MoveDown();
void TestFull();
void pushBrick();
void loopGame(int nFrame);
void initGame();
void chooseDifficulty();
void plusDelay(int delta);
int createBrick();
void plusBrickLine(int lineAmount);
void gameOver();

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
int TileColor[8] = { 11,14,10,4,1,6,5,7 };
enum { EMPTY, BRICK, WALL };
char arTile[3][4] = { ". ","��","��" };
int board[BW + 2][BH + 2][2];
int infoBoard[6][BH -3][2];
int holdBoard[6][7][2];
int nx, ny;
int brick, rot;
int next[3];
int hold=7;
int gameDelay;
int line=0;
int sevenBag[7] = { 0,1,2,3,4,5,6 };
int sevenBagidx = 0;
BOOL isHolded = FALSE;
int brickCount = 0;
int difficulty = 0;
BOOL isGameOver = FALSE;

int main()
{
	int nFrame = 20;
	showcursor(FALSE);
	// ��ü ���� ����
	for (; 1;) {
		initGame(nFrame);
		loopGame(nFrame);
		gameOver();
	}
	showcursor(TRUE);
}

void DrawScreen()
{
	for (int x = 0; x < BW + 7; x++) {
		for (int y = 0; y < BH + 2; y++) {
			if (x < BW + 2) {
				color(board[x][y][1]);
				putsxy(BX + x * 2, BY + y, arTile[board[x][y][0]]);
			}
			else if(y < BH - 3) {
				color(infoBoard[x - (BW + 1)][y][1]);
				putsxy(BX + x * 2, BY + y, arTile[infoBoard[x- (BW + 1) ][y][0]]);
			}
			
		}
	}
	for (int x = BW + 6; x < BW + 12; x++) {
		for (int y = 0; y < 7; y++) {
				color(holdBoard[x - (BW + 6)][y][1]);
				putsxy(BX + x * 2, BY + y, arTile[holdBoard[x - (BW + 6)][y][0]]);
		}
	}
	char buffer[3];
	_itoa_s(gameDelay, buffer, 3, 10);
	color(WHITE);
	putsxy(55, 3, "Tetris Ver 1.2");
	putsxy(55, 5, "�¿�:�̵�, ��:ȸ��, �Ʒ�:����");
	putsxy(55, 7, "�����̽� ��:���� ����, c:Ȧ��");
	putsxy(55, 9, "gameDelay : ");
	putsxy(67, 9, buffer);
	putsxy(29, 2, "n e x t");
	putsxy(39, 2, "h o l d");
	PrintNextBrick();
	PrintHold();
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
			case 'C':
			case 'c':
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
		color(Show ? TileColor[brick] : EMPTY);
		gotoxy(BX + (Shape[brick][rot][i].x + nx) * 2, BY + Shape[brick][rot][i].y + ny);
		puts(arTile[Show ? BRICK : EMPTY]);
	}
	color(WHITE);
}
void PrintNextBrick()
{
	//���� ������ ǥ���ϱ� ���� �Լ�
	//������ ǥ���ϱ� ���� ��ҿ� �ִ� ������ �����
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 4; k++) {
				color(EMPTY);
				putsxy(29 + (i) * 2, 3 + (5 * j) + k, arTile[EMPTY]);
			}
		}
	}
	//���� ������ ǥ���Ѵ�
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			color(TileColor[next[i]]);
			gotoxy(31 + (Shape[next[i]][0][j].x) * 2, (5+(5*i)) + Shape[next[i]][0][j].y );
			puts(arTile[BRICK]);
		}
	}
	color(WHITE);
}
void PrintHold() {
	//Ȧ��� ������ ǥ���ϱ� ���� �Լ�
	//������ ǥ���ϱ� ���� ��ҿ� �ִ� ������ �����
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			color(EMPTY);
			putsxy(39 + (i) * 2, 3 + j, arTile[EMPTY]);
		}
	}
	//���� ������ ǥ���Ѵ�
	if (hold != 7) {
		for (int i = 0; i < 4; i++) {
			color(TileColor[hold]);
			gotoxy(41 + (Shape[hold][0][i].x) * 2, 5 + Shape[hold][0][i].y);
			puts(arTile[BRICK]);
		}
	}
	color(WHITE);
}
void holdBrick()
{
	//������ �����ϰ� �������� �ִ� ������ ��ȯ�ϴ� �Լ�
	if (isHolded) {
		//�̹� Ȧ�带 ���������� ����� ������ ������ Ȧ���� �� ���� �ϱ� ����
		return;
	}
	if (hold == 7) {
		//������ ������ 7���̹Ƿ� �����Ҽ� ���� ������ �̿��� Ȧ�带 ���� ���� �ʱⰪ���� ǥ��
		PrintBrick(FALSE);
		hold = brick;
		isHolded = TRUE;
		nx = BW / 2;
		ny = 3;
		rot = 0;
		pushBrick();
		PrintNextBrick();
		PrintHold();
		PrintBrick(TRUE);
	}
	else {
		//������ ���� ��� Ȧ��� ������ ���� ���� �ٲ� �ٸ� ������ �������� ��
		PrintBrick(FALSE);
		int tmp = hold;
		hold = brick;
		brick = tmp;
		nx = BW / 2;
		ny = 3;
		rot = 0;
		isHolded = TRUE;
		PrintHold();
		PrintBrick(TRUE);
	}
}

int GetAround(int x, int y, int b, int r)
{
	int k = EMPTY;

	// ������ ������ Ÿ�� ��� �� ���� ū ���� ã�´�.
	for (int i = 0; i < 4; i++) {
		k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y][0]);
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
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y][0] = BRICK;
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y][1] = TileColor[brick];
	}
	brickCount++;
	// �������� ������ ���� ����
	for (int y = 1; y < BH + 1; y++) {
		BOOL bFull = TRUE;
		for (int x = 1; x < BW + 1; x++) {
			if (board[x][y][0] == EMPTY) {
				bFull = FALSE;
				break;
			}
		}
		// ������ ���� á���� �� ���� �����Ѵ�.
		if (bFull) {
			for (int ty = y; ty > 1; ty--) {
				for (int x = 1; x < BW + 1; x++) {
					board[x][ty][0] = board[x][ty - 1][0];
					board[x][ty][1] = board[x][ty - 1][1];
				}
			}
			brickCount = 0;
			line++;
			if (line >= 2) {
				plusDelay(difficulty);
				line = 0;
			}
			DrawScreen();
			delay(200);
		}
	}
	//������ �������� �̻� �׿������� ���� ������ ������ �� ���� �߰�
	if (brickCount >= 9 - difficulty) {
		plusBrickLine(difficulty);
		DrawScreen();
		brickCount = 0;
	}
}

void pushBrick() {
	//������ ���� 1���� �о�ְ� �� ������ �� �� ������ ����� �Լ�
	brick = next[0];
	next[0] = next[1];
	next[1] = next[2];
	next[2] = createBrick();
}

void loopGame(int nFrame) {
	//������ �ݺ��Ǵ� ������ �Լ�ȭ
	int nStay;
	for (; !isGameOver;) {
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
		isHolded = FALSE;
	}
	clrscr();
}

void initGame(int initFrame) {
	//������ ���¸� �ʱ�ȭ�ϴ� �Լ�
	chooseDifficulty();
	gameDelay = 1000 / initFrame;
	int x, y;
	randomize();
	clrscr();
	// �����ڸ��� ��, �������� �� �������� �ʱ�ȭ�Ѵ�.
	for (x = 0; x < BW + 2; x++) {
		for (y = 0; y < BH + 2; y++) {
			if (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) {
				board[x][y][0] = WALL;
				board[x][y][1] = WHITE;
			}
			else {
				board[x][y][0] = EMPTY;
				board[x][y][1] = EMPTY;
			}
		}
	}
	//���� ������ ǥ�õ� ������ �ʱ�ȭ
	for (x = 0; x < 6; x++) {
		for (y = 0; y < BH + 2; y++) {
			if (y == 0 || (y > 1 && ((y % 5) == 1)) || y == BH + 1 || x == 0 || x == 5) {
				infoBoard[x][y][0] = WALL;
				infoBoard[x][y][1] = WHITE;
			}
			else {
				infoBoard[x][y][0] = EMPTY;
				infoBoard[x][y][1] = EMPTY;
			}
		}
	}
	//Ȧ�尡 ǥ�õ� ������ �ʱ�ȭ
	for (x = 0; x < 6; x++) {
		for (y = 0; y < 7; y++) {
			if (y == 0 || y == 6 || x == 0 || x == 5) {
				holdBoard[x][y][0] = WALL;
				holdBoard[x][y][1] = WHITE;
			}
			else {
				holdBoard[x][y][0] = EMPTY;
				holdBoard[x][y][1] = EMPTY;
			}
		}
	}
	DrawScreen();
	//���� ������ �ʱ�ȭ

	for (int i = 0;i < 3;i++) {
		next[i]= createBrick();
	}
	isGameOver = FALSE;
}
void chooseDifficulty() {
	//���̵��� �����ϱ� ���� �Լ�
	clrscr();
	color(WHITE);
	putsxy(17, 8, "���̵��� ������ �ּ���.");
	putsxy(11, 10, "���:���̵� ����, ENTER: ���̵� ����");
	putsxy(22, 12, "�� easy");
	putsxy(22, 14, "   normal");
	putsxy(22, 16, "   hard");
	int arrowPosition=0;
	for (; 1;) {
		if (kbhit()) {
			int ch = getch();
			if (ch == 0xE0 || ch == 0) {
				ch = getch();
				switch (ch) {
				case UP:
					putsxy(22, 12 + arrowPosition, "  ");
					arrowPosition -= 2;
					if (arrowPosition > 4) {
						arrowPosition = 4;
					}
					difficulty= arrowPosition/2;
					putsxy(22, 12 + arrowPosition, "��");
					break;
				case DOWN:
					putsxy(22, 12 + arrowPosition, "  ");
					arrowPosition += 2;
					if (arrowPosition > 4) {
						arrowPosition = 4;
					}
					difficulty = arrowPosition / 2;
					putsxy(22, 12 + arrowPosition, "��");
					break;
				}
			}
			else {
				switch (ch) {
				case 13:
					return;
			}
		}
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
int createBrick() {
	//������ ����� ���� �Լ�
	if (sevenBagidx == 0) {
		//7-bag�� �����ϱ� ���� ����� ������ ������ �迭�� ����
		int tmp = 0;
		int ridx = 0;
		for (int i = 0; i < 6; i++) {
			ridx = random(7 - i) + i;
			tmp = sevenBag[ridx];
			sevenBag[ridx] = sevenBag[i];
			sevenBag[i] = tmp;
		}
	}
	int brick = sevenBag[sevenBagidx];
	sevenBagidx = (sevenBagidx + 1) % 7;
	return brick;
}
void plusBrickLine(int lineAmount) {
	//���ؿ� ������ �߰��ϴ� �Լ�
	int empty = random(10)+1;
	//�� ���ٿ��� ���ؿ� ������ ���μ���ŭ�� �ٿ� ������ ������� ���ӿ���
	for (int i = 0; i < lineAmount-1; i++) {
		for (int x = 1; x < BW + 1; x++) {
			if (lineAmount > 0 && board[x][i+1][0] == BRICK) {
				isGameOver = TRUE;
			}
		}
	}
	//������ ���ؿ� ������ ���μ���ŭ �ø���
	for (int ty = 1; ty <= BH - lineAmount; ty++) {
		for (int x = 1; x < BW + 1; x++) {
			board[x][ty][0] = board[x][ty + lineAmount][0];
			board[x][ty][1] = board[x][ty + lineAmount][1];
			board[x][ty + lineAmount][0] = EMPTY;
			board[x][ty + lineAmount][1] = EMPTY;
		}
	}	
	for (int ty = BH; ty > BH-lineAmount; ty--) {
		for (int x = 1; x < BW + 1; x++) {
			if (x == empty) {
				board[x][ty][0] = EMPTY;
				board[x][ty][1] = EMPTY;
			}
			else {
				board[x][ty][0] = BRICK;
				board[x][ty][1] = 7;
			}
		}
	}
}
void gameOver() {
	//���ӿ����� �Ǿ��� ����� ȭ���� ǥ���ϱ� ���� �Լ�
	BOOL isKeyDown = FALSE;
	color(WHITE);
	putsxy(15, 10, "G A M E   O V E R");
	putsxy(18, 12, "R E T R Y ?");
	putsxy(21, 14, "Y / N");
	do {
		if (kbhit()) {
			int ch = getch();
			switch (ch) {
			case 'Y':
			case 'y':
				isKeyDown = TRUE;
				break;
			case 'N':
			case 'n':
			case ESC:
				exit(0);
			}
		}
	} while (!isKeyDown);
}
