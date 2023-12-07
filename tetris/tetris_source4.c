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
//Ÿ�Ϻ��� ���� ������ Ÿ���� �׸� �� �ٽ� �Ͼ������ ������ ���� �Լ�
#define putTile(c,x,y,s){color(c);gotoxy(x, y);puts(s);color(15);}
#define BX 5
#define BY 1
#define BW 10
#define BH 20
#define BOFFSET 2

void DrawScreen();
BOOL ProcessKey();
void bigBrickShape(int brick, int rot);
void PrintBrick(BOOL Show);
void PrintNextBrick();
void PrintHold();
void holdBrick();
int GetAround(int x, int y, int b, int r);
BOOL MoveDown();
void TestFull();
void changeBombColor();
void explosion();
void pushBrick();
void loopGame(int nFrame);
void initGame(int nFrame);
void chooseDifficulty();
void plusDelay(int delta);
int createBrick();
void plusBrickLine(int lineAmount);
void gameOver();
int brickValue(int bricks);

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
	{ { 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0 } }
	//,{ { 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 } }
};
//������ ���� ����Ű�� ���ڸ� �ؽ�Ʈ�� ���� �а��ϱ� ���� �߰�
enum {BLACK=0,WHITE=15,COLOR_I=11, COLOR_O=14, COLOR_S=10, COLOR_Z=4, COLOR_J=1, COLOR_L=6, COLOR_T=5, COLOR_G=7,COLOR_BOMB=12, BIG=14};
int TileColor[13] = { BLACK,WHITE,COLOR_I,COLOR_O,COLOR_S,COLOR_Z,COLOR_J,COLOR_L,COLOR_T,COLOR_BOMB,COLOR_G,WHITE,BIG };
enum { EMPTY, WALL, BRICK_I, BRICK_O, BRICK_S, BRICK_Z, BRICK_J, BRICK_L, BRICK_T, BOMB, BRICK_G,BRICK_WHITE,BIG_BRICK, HOLD_E=16};
char arTile[13][4] = { ". ","��","��","��","��","��","��","��","��","��","��","��","��"};
int board[BW + 2][BH + 2];
int infoBoard[6][BH -3];
int holdBoard[6][7];
int nx, ny;
int brick, rot;
int next[3];
BOOL isBig[4]={FALSE, FALSE, FALSE, FALSE };
BOOL isBigHold = FALSE;
struct Point bShape[16] = { 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 };
int hold= HOLD_E;
int gameDelay;
int line=0;
int sevenBag[7] = { 0,1,2,3,4,5,6 };
int sevenBagidx = 0;
BOOL isHolded = FALSE;
int brickCount = 0;
int modeselect = 0;
int difficulty = 0;
clock_t before;
BOOL bombColorTick = FALSE;
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
				putTile(TileColor[board[x][y]],BX + x * 2, BY + y, arTile[board[x][y]]);
			}
			else if(y < BH - 3) {
				putTile(TileColor[infoBoard[x - (BW + 1)][y]], BX + x * 2, BY + y, arTile[infoBoard[x - (BW + 1)][y]]);
			}
			
		}
	}
	for (int x = BW + 6; x < BW + 12; x++) {
		for (int y = 0; y < 7; y++) {
			putTile(TileColor[holdBoard[x - (BW + 6)][y]], BX + x * 2, BY + y, arTile[holdBoard[x - (BW + 6)][y]]);
		}
	}
	char buffer[3];
	_itoa_s(gameDelay, buffer, 3, 10);
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
				if (brickValue(brick) != BOMB) {
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

void bigBrickShape(int brick, int rot) {
	//ū ũ���� ������ ����� ���� �Լ�
	struct Point defaultShape[4] = { {0,0},{0,1},{1,0},{1,1} };
	for (int i = 0;i < 4;i++) {
		for (int j = 0;j < 4;j++) {
			bShape[(i * 4) + j].x = defaultShape[j].x + (Shape[brick][rot][i].x * 2);
			bShape[(i * 4) + j].y = defaultShape[j].y + (Shape[brick][rot][i].y * 2);
		}
	}
}

void PrintBrick(BOOL Show)
{
	if (isBig[0]) {
		bigBrickShape(brick, rot);
		for (int i = 0;i < 16;i++) {
			putTile(Show ? TileColor[brickValue(brick)] : TileColor[EMPTY], BX + (bShape[i].x + nx) * 2, BY + bShape[i].y + ny, arTile[Show ? brickValue(brick) : EMPTY])
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			putTile(Show ? TileColor[brickValue(brick)] : TileColor[EMPTY] , BX + (Shape[brick][rot][i].x + nx) * 2, BY + Shape[brick][rot][i].y + ny, arTile[Show ? brickValue(brick) : EMPTY])
		}
	}
	
}
void PrintNextBrick()
{
	//���� ������ ǥ���ϱ� ���� �Լ�
	//������ ǥ���ϱ� ���� ��ҿ� �ִ� ������ �����
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 4; k++) {
				putTile(TileColor[EMPTY],29 + (i) * 2, 3 + (5 * j) + k, arTile[EMPTY]);
			}
		}
	}
	//���� ������ ǥ���Ѵ�
	for (int i = 0; i < 3; i++) {	
		if (isBig[i + 1]) {
			putTile(TileColor[BIG_BRICK], 35, (3 + (5 * i)), arTile[BIG_BRICK]);
		}
		for (int j = 0; j < 4; j++) {
			putTile(brickValue(next[i])==BOMB ? COLOR_BOMB :TileColor[brickValue(next[i])], 31 + (Shape[next[i]][0][j].x) * 2, (5 + (5 * i)) + Shape[next[i]][0][j].y, arTile[brickValue(next[i])]);
		}
	}
}
void PrintHold() {
	//Ȧ��� ������ ǥ���ϱ� ���� �Լ�
	//������ ǥ���ϱ� ���� ��ҿ� �ִ� ������ �����
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			putTile(TileColor[EMPTY], 39 + (i) * 2, 3 + j, arTile[EMPTY])
		}
	}
	//Ȧ��� ������ ǥ���Ѵ�
	if (hold != HOLD_E) {
		if (isBigHold) {
			putTile(TileColor[BIG_BRICK], 45, 3, arTile[BIG_BRICK]);
		}
		for (int i = 0; i < 4; i++) {
			putTile(TileColor[brickValue(hold)],41 + (Shape[hold][0][i].x) * 2, 5 + Shape[hold][0][i].y, arTile[brickValue(hold)]);
		}
	}
}
void holdBrick()
{
	//������ �����ϰ� �������� �ִ� ������ ��ȯ�ϴ� �Լ�
	if (isHolded) {
		//�̹� Ȧ�带 ���������� ����� ������ ������ Ȧ���� �� ���� �ϱ� ����
		return;
	}
	if (hold == HOLD_E) {
		//������ ������ 7���̹Ƿ� �����Ҽ� ���� ������ �̿��� Ȧ�带 ���� ���� �ʱⰪ���� ǥ��
		PrintBrick(FALSE);
		hold = brick;
		isHolded = TRUE;
		nx = BW / 2;
		ny = 3;
		rot = 0;
		isBigHold = isBig[0];
		pushBrick();
		PrintNextBrick();
		PrintHold();
		PrintBrick(TRUE);
	}
	else {
		//������ ���� ��� Ȧ��� ������ ���� ���� �ٲ� �ٸ� ������ �������� ��
		PrintBrick(FALSE);
		int tmp = hold;
		BOOL bigtmp = isBigHold;
		hold = brick;
		brick = tmp;
		nx = BW / 2;
		ny = 3;
		rot = 0;
		isBigHold = isBig[0];
		isBig[0] = bigtmp;
		isHolded = TRUE;
		PrintHold();
		PrintBrick(TRUE);
	}
}

int GetAround(int x, int y, int b, int r)
{
	int k = EMPTY;
	// ������ ������ Ÿ�� ��� �� ���� ū ���� ã�´�.
	if (isBig[0]) {
		bigBrickShape(b, r);
		for (int i = 0; i < 16; i++) {
			k = max(k, board[x + bShape[i].x][y + bShape[i].y]);
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
		}
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
	if (isBig[0]) {
		bigBrickShape(brick, rot);
		
		for (int i = 0; i < 16; i++) {
			board[nx + bShape[i].x][ny + bShape[i].y] = brickValue(brick);
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = brickValue(brick);
		}
	}
	if (brickValue(brick) == BOMB) {
		explosion();
		return;
	}	
	else {
		brickCount++;
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
			brickCount = 0;
			line++;
			if (line >= 2) {
				plusDelay(difficulty);
				line = 0;
			}
			DrawScreen();
			delay(100);
		}
	}
	//������ �������� �̻� �׿������� ���� ������ ������ �� ���� �߰�
	if (brickCount >= 9 - difficulty) {
		plusBrickLine(difficulty);
		DrawScreen();
		brickCount = 0;
	}
}

void changeBombColor() {
	//��ź�� ���� �����ϱ� ���� �Լ�
	if (bombColorTick) {
		TileColor[BOMB] = COLOR_BOMB;
		bombColorTick = !bombColorTick;
	}
	else {
		TileColor[BOMB] = WHITE;
		bombColorTick = !bombColorTick;
	}
}

void explosion() {
	//��ź�� ������ �������� �� �ֺ� ������ ���ֱ� ���� �Լ�
	for (int y = BH; y >= 1; y--) {
		BOOL isExploded = FALSE;
		for (int x = 1; x < BW + 1; x++) {
			if (board[x][y] == BOMB) {
				isExploded = TRUE;
				for (int tx = 1; tx < BW + 1; tx++) {
					board[tx][y] = BRICK_WHITE;
				}
				break;
			}		
		}
		if (isExploded) {
			DrawScreen();
			delay(50);
			for (int ty = y; ty > 1; ty--) {
				for (int x = 1; x < BW + 1; x++) {
					board[x][ty] = board[x][ty-1];
				}
			}
			break;
		}
	}
	brickCount = 0;
	clrscr();
	delay(100);
	DrawScreen();
}

void pushBrick() {
	//������ ���� 1���� �о�ְ� �� ������ �� �� ������ ����� �Լ�
	brick = next[0];
	next[0] = next[1];
	next[1] = next[2];
	for (int i = 0;i < 3;i++) {
		isBig[i] = isBig[i + 1];
	}
	if (modeselect && sevenBag[sevenBagidx] == random(7)) {
		isBig[3] = TRUE;
	}
	else {
		isBig[3] = FALSE;
	}
	next[2] = createBrick();
	if (modeselect && brickValue(next[2]) == BOMB) {
		isBig[3] = FALSE;
	}
}

void loopGame(int nFrame) {
	//������ �ݺ��Ǵ� ������ �Լ�ȭ
	int nStay;
	for (; !isGameOver;) {
		pushBrick(); 
		nx = BW / 2;
		if (isBig[0]) {
			ny = 4;
		}
		else {
			ny = 3;
		}
		rot = 0;
		PrintBrick(TRUE);
		PrintNextBrick();
		if (GetAround(nx, ny, brick, rot) != EMPTY) break;

		// ���� �ϳ��� �ٴڿ� ���� �������� ����
		nStay = nFrame;
		for (; 2;) {
			if ((double)(clock() - before)>=50) {
				changeBombColor();
				before = clock();
			}
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
	before = clock();
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
		if (modeselect && sevenBag[sevenBagidx] == random(7)) {
			isBig[i+1] = TRUE;
		}
		else {
			isBig[i+1] = FALSE;
		}
		next[i]= createBrick();
	}
	isGameOver = FALSE;
}
void chooseDifficulty() {
	//���̵��� �����ϱ� ���� �Լ�
	clrscr();
	putsxy(15, 6, "���� ���̵��� ������ �ּ���.");
	putsxy(5, 8, "���:��� ����, ���:���̵� ����, ENTER: ���̵� ����");
	putsxy(21, 11, "��  simple  ��");
	putsxy(22, 14, "�� easy");
	putsxy(22, 16, "   normal");
	putsxy(22, 18, "   hard");
	int arrowPosition=0;
	for (; 1;) {
		if (kbhit()) {
			int ch = getch();
			if (ch == 0xE0 || ch == 0) {
				ch = getch();
				putsxy(22, 14 + arrowPosition, "  ");
				switch (ch) {
				case UP:
					arrowPosition -= 2;
					if (arrowPosition > 4) {
						arrowPosition = 4;
					}
					difficulty= arrowPosition/2;
					break;
				case DOWN:
					arrowPosition += 2;
					if (arrowPosition > 4) {
						arrowPosition = 4;
					}
					difficulty = arrowPosition / 2;
					break;
				case LEFT:
					modeselect = !modeselect;
					break;
				case RIGHT:
					modeselect = !modeselect;
					break;
				}
				if (modeselect) {
					putsxy(21, 11, "��  bigger  ��");
				}
				else {
					putsxy(21, 11, "��  simple  ��");
				}		
				putsxy(22, 14 + arrowPosition, "��");
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
	int brick;
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
	if (random(4) == 3) {
		brick = 7;
	}
	else {

		brick = sevenBag[sevenBagidx];
		sevenBagidx = (sevenBagidx + 1) % 7;
	}
	return brick;
}
void plusBrickLine(int lineAmount) {
	//���ؿ� ������ �߰��ϴ� �Լ�
	int empty = random(10)+1;
	if (lineAmount == 0) {
		return;
	}
	//�� ���ٿ��� ���ؿ� ������ ���μ���ŭ�� �ٿ� ������ ������� ���ӿ���
	for (int i = 0; i < lineAmount-1; i++) {
		for (int x = 1; x < BW + 1; x++) {
			if (lineAmount > 0 && board[x][i+1] != EMPTY) {
				isGameOver = TRUE;
			}
		}
	}
	//������ ���ؿ� ������ ���μ���ŭ �ø���
	for (int ty = 1; ty <= BH - lineAmount; ty++) {
		for (int x = 1; x < BW + 1; x++) {
			board[x][ty] = board[x][ty + lineAmount];
			board[x][ty + lineAmount] = EMPTY;
		}
	}	
	for (int ty = BH; ty > BH-lineAmount; ty--) {
		for (int x = 1; x < BW + 1; x++) {
			if (x == empty) {
				board[x][ty] = EMPTY;
			}
			else {
				board[x][ty] = BRICK_G;
			}
		}
	}
}
void gameOver() {
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

int brickValue(int bricks) { 
	//brick,hold,next������ board�� ������ �� arTile�迭�� �°� ��ȯ�ϱ� ���� �Լ�
	return (bricks + BOFFSET);
}
