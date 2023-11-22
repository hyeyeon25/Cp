#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define randomize() srand((unsigned)time(NULL))
#define random(n) (rand() % (n))
#define delay(n) Sleep(n)
#define clrscr() system("cls")
//블럭의 색을 표현해 주기 위한 함수를 사용하기 쉽게 정의
#define color(colorNum) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum)
#define gotoxy(x,y) { COORD Cur = {x, y}; \
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);}
#define showcursor(bShow) { CONSOLE_CURSOR_INFO CurInfo = {20, bShow}; \
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo); }

enum { ESC = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };
#define putsxy(x, y, s) {gotoxy(x, y);puts(s);}
#define putTile(c,x,y,s){color(c);gotoxy(x, y);puts(s);color(15);}
#define BX 5
#define BY 1
#define BW 10
#define BH 20
#define BOFFSET 2

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
};
enum {BLACK=0,WHITE=15,COLOR_I=11, COLOR_O=14, COLOR_S=10, COLOR_Z=4, COLOR_J=1, COLOR_L=6, COLOR_T=5, COLOR_G=7};
int TileColor[10] = { BLACK,WHITE,COLOR_I,COLOR_O,COLOR_S,COLOR_Z,COLOR_J,COLOR_L,COLOR_T,COLOR_G };
enum { EMPTY, WALL, BRICK_I, BRICK_O, BRICK_S, BRICK_Z, BRICK_J, BRICK_L, BRICK_T, BRICK_G };
char arTile[10][4] = { ". ","□","■","■","■","■","■","■","■","■" };
int board[BW + 2][BH + 2];
int infoBoard[6][BH -3];
int holdBoard[6][7];
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
	// 전체 게임 루프
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
	putsxy(55, 5, "좌우:이동, 위:회전, 아래:내림");
	putsxy(55, 7, "스페이스 바:전부 내림, c:홀드");
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
		putTile(Show ? TileColor[brickValue(brick)] : TileColor[EMPTY] , BX + (Shape[brick][rot][i].x + nx) * 2, BY + Shape[brick][rot][i].y + ny, arTile[Show ? brickValue(brick) : EMPTY])
	}
}
void PrintNextBrick()
{
	//다음 벽돌을 표시하기 위한 함수
	//벽돌을 표시하기 위한 장소에 있는 벽돌을 지운다
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 4; k++) {
				putTile(TileColor[EMPTY],29 + (i) * 2, 3 + (5 * j) + k, arTile[EMPTY]);
			}
		}
	}
	//다음 벽돌을 표시한다
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			putTile(TileColor[brickValue(next[i])], 31 + (Shape[next[i]][0][j].x) * 2, (5 + (5 * i)) + Shape[next[i]][0][j].y, arTile[brickValue(next[i])]);
		}
	}
}
void PrintHold() {
	//홀드된 벽돌을 표시하기 위한 함수
	//벽돌을 표시하기 위한 장소에 있는 벽돌을 지운다
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			putTile(TileColor[EMPTY], 39 + (i) * 2, 3 + j, arTile[EMPTY])
		}
	}
	//홀드된 벽돌을 표시한다
	if (hold != 7) {
		for (int i = 0; i < 4; i++) {
			putTile(TileColor[brickValue(hold)],41 + (Shape[hold][0][i].x) * 2, 5 + Shape[hold][0][i].y, arTile[brickValue(hold)]);
		}
	}
}
void holdBrick()
{
	//벽돌을 저장하고 떨어지고 있는 벽돌과 교환하는 함수
	if (isHolded) {
		//이미 홀드를 사용했을경우 블록이 떨어질 때까지 홀드할 수 없게 하기 위함
		return;
	}
	if (hold == 7) {
		//벽돌의 종류가 7개이므로 존재할수 없는 값임을 이용해 홀드를 하지 않은 초기값임을 표시
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
		//나머지 값일 경우 홀드와 벽돌의 값을 서로 바꿔 다른 벽돌을 떨어지게 함
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

	// 벽돌이 차지한 타일 모양 중 가장 큰 값을 찾는다.
	for (int i = 0; i < 4; i++) {
		k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	}
	return k;
}

BOOL MoveDown()
{
	// 바닥에 닿았으면 가득찼는지 점검하고 TRUE를 리턴한다.
	if (GetAround(nx, ny + 1, brick, rot) != EMPTY) {
		TestFull();
		return TRUE;
	}
	// 아직 공중에 떠 있으면 한칸 아래로 내린다.
	PrintBrick(FALSE);
	ny++;
	PrintBrick(TRUE);
	return FALSE;
}

void TestFull()
{
	// 바닥에 내려앉은 벽돌 기록
	for (int i = 0; i < 4; i++) {
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = brickValue(brick);
	}
	brickCount++;
	// 수평으로 가득찬 벽돌 제거
	for (int y = 1; y < BH + 1; y++) {
		BOOL bFull = TRUE;
		for (int x = 1; x < BW + 1; x++) {
			if (board[x][y] == EMPTY) {
				bFull = FALSE;
				break;
			}
		}
		// 한줄이 가득 찼으면 이 줄을 제거한다.
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
			delay(200);
		}
	}
	//벽돌이 일정개수 이상 쌓였을때도 줄을 지우지 못했을 때 벽돌 추가
	if (brickCount >= 9 - difficulty) {
		plusBrickLine(difficulty);
		DrawScreen();
		brickCount = 0;
	}
}

void pushBrick() {
	//벽돌의 값을 1번씩 밀어넣고 그 다음에 올 새 벽돌을 만드는 함수
	brick = next[0];
	next[0] = next[1];
	next[1] = next[2];
	next[2] = createBrick();
}

void loopGame(int nFrame) {
	//게임이 반복되는 구간을 함수화
	int nStay;
	for (; !isGameOver;) {
		pushBrick(); 
		nx = BW / 2;
		ny = 3;
		rot = 0;
		PrintBrick(TRUE);
		PrintNextBrick();
		if (GetAround(nx, ny, brick, rot) != EMPTY) break;

		// 벽돌 하나가 바닥에 닿을 때까지의 루프
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
	//게임의 상태를 초기화하는 함수
	chooseDifficulty();
	gameDelay = 1000 / initFrame;
	int x, y;
	randomize();
	clrscr();
	// 가장자리는 벽, 나머지는 빈 공간으로 초기화한다.
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
	//다음 벽돌이 표시될 공간을 초기화
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
	//홀드가 표시될 공간을 초기화
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
	//다음 벽돌을 초기화

	for (int i = 0;i < 3;i++) {
		next[i]= createBrick();
	}
	isGameOver = FALSE;
}
void chooseDifficulty() {
	//난이도를 선택하기 위한 함수
	clrscr();
	putsxy(17, 8, "난이도를 선택해 주세요.");
	putsxy(11, 10, "↑↓:난이도 변경, ENTER: 난이도 선택");
	putsxy(22, 12, "▶ easy");
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
					putsxy(22, 12 + arrowPosition, "▶");
					break;
				case DOWN:
					putsxy(22, 12 + arrowPosition, "  ");
					arrowPosition += 2;
					if (arrowPosition > 4) {
						arrowPosition = 4;
					}
					difficulty = arrowPosition / 2;
					putsxy(22, 12 + arrowPosition, "▶");
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
	//게임의 스피드를 바꾸기 위한 함수
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
	//벽돌을 만들기 위한 함수
	if (sevenBagidx == 0) {
		//7-bag을 구현하기 위해 블록의 순서를 저장한 배열을 섞음
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
	//방해용 벽돌을 추가하는 함수
	int empty = random(10)+1;
	if (lineAmount == 0) {
		return;
	}
	//맨 윗줄에서 방해용 벽돌의 라인수만큼의 줄에 벽돌이 있을경우 게임오버
	for (int i = 0; i < lineAmount-1; i++) {
		for (int x = 1; x < BW + 1; x++) {
			if (lineAmount > 0 && board[x][i+1] != EMPTY) {
				isGameOver = TRUE;
			}
		}
	}
	//벽돌을 방해용 벽돌의 라인수만큼 올린다
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
	//게임오버가 되었을 경우의 화면을 표시하기 위한 함수
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
	return (bricks + BOFFSET);
}