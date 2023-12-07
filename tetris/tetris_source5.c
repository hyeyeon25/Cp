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
//타일별로 색을 변경후 타일을 그린 후 다시 하얀색으로 돌리기 위한 함수
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
void initGame(int initFrame);
void chooseDifficulty();
void plusDelay(int delta);
int createBrick();
void plusBrickLine(int lineAmount);
void gameOver();
int brickValue(int bricks);

void puyoDrop();
void PrintPuyo(BOOL Show);
void PrintNextPuyo();
int GetAroundPuyo(int x, int y, int r);
BOOL MoveDownPuyo();
BOOL CheckLinkPuyo();
void DrawPuyoScreen();
BOOL ProcessKeyPuyo();
void pushPuyo();
void loopPuyoGame(int initFrame);
void initPuyoGame(int nFrame);
void createPuyo(int puyo[]);
void plusPuyoLine(int lineAmount);

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
//벽돌의 색을 가리키는 숫자를 텍스트만 보고 분간하기 위해 추가
enum { BLACK = 0, WHITE = 15, COLOR_I = 11, COLOR_O = 14, COLOR_S = 10, COLOR_Z = 4, COLOR_J = 1, COLOR_L = 6, COLOR_T = 5, COLOR_G = 7, COLOR_BOMB = 12, BIG = 14 };
int TileColor[13] = { BLACK,WHITE,COLOR_I,COLOR_O,COLOR_S,COLOR_Z,COLOR_J,COLOR_L,COLOR_T,COLOR_BOMB,COLOR_G,WHITE,BIG };
enum { EMPTY, WALL, BRICK_I, BRICK_O, BRICK_S, BRICK_Z, BRICK_J, BRICK_L, BRICK_T, BOMB, BRICK_G, BRICK_WHITE, BIG_BRICK, HOLD_E = 16 };
char arTile[13][4] = { ". ","□","■","■","■","■","■","■","■","◈","■","■","★" };
int board[BW + 2][BH + 2];
int infoBoard[6][BH - 3];
int holdBoard[6][7];
int nx, ny;
int brick, rot;
int next[3];
BOOL isBig[4] = { FALSE, FALSE, FALSE, FALSE };
BOOL isBigHold = FALSE;
struct Point bShape[16] = { 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 };
int hold = HOLD_E;
int gameDelay;
int line = 0;
int sevenBag[7] = { 0,1,2,3,4,5,6 };
int sevenBagidx = 0;
BOOL isHolded = FALSE;
int brickCount = 0;
int modeselect = 0;
int difficulty = 0;
clock_t before;
BOOL bombColorTick = FALSE;
BOOL isGameOver = FALSE;

struct Point PuyoShape[4][2] = { {0,0,0,-1}, {0,0,1,0}, {0,0,0,1}, {0,0,-1,0} };
enum { PUYO_CLR_R = 4, PUYO_CLR_G = 10, PUYO_CLR_Y = 6, PUYO_CLR_P = 13, PUYO_CLR_B = 9, PUYO_CLR_W = 7 };
enum { PUYO_R = 2, PUYO_G = 3, PUYO_Y = 4, PUYO_P = 5, PUYO_B = 6, PUYO_W = 7 };
int PuyoColor[8] = { BLACK,WHITE,PUYO_CLR_R ,PUYO_CLR_G,PUYO_CLR_Y,PUYO_CLR_P,PUYO_CLR_B,PUYO_CLR_W };
char PuyoTile[8][4] = { ". ","□","●","●","●","●","●","●" };
int puyo[2];
int puyocnt = 0;
int nextPuyo[3][2];
int checked[BW + 2][BH + 2] = { 0, };

int main()
{
	int nFrame = 20;
	showcursor(FALSE);
	// 전체 게임 루프
	for (; 1;) {
		chooseDifficulty();
		if (modeselect != 2) {
			initGame(nFrame);
			loopGame(nFrame);
		}
		else {
			initPuyoGame(nFrame);
			loopPuyoGame(nFrame);
		}
		gameOver();
	}
	showcursor(TRUE);
}

void DrawScreen()
{
	for (int x = 0; x < BW + 7; x++) {
		for (int y = 0; y < BH + 2; y++) {
			if (x < BW + 2) {
				putTile(TileColor[board[x][y]], BX + x * 2, BY + y, arTile[board[x][y]]);
			}
			else if (y < BH - 3) {
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
	//큰 크기의 벽돌을 만들어 내는 함수
	struct Point defaultShape[4] = { {0,0},{0,1},{1,0},{1,1} };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			bShape[(i * 4) + j].x = defaultShape[j].x + (Shape[brick][rot][i].x * 2);
			bShape[(i * 4) + j].y = defaultShape[j].y + (Shape[brick][rot][i].y * 2);
		}
	}
}

void PrintBrick(BOOL Show)
{
	if (isBig[0]) {
		bigBrickShape(brick, rot);
		for (int i = 0; i < 16; i++) {
			putTile(TileColor[Show ? brickValue(brick) : EMPTY], BX + (bShape[i].x + nx) * 2, BY + bShape[i].y + ny, arTile[Show ? brickValue(brick) : EMPTY])
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			putTile(TileColor[Show ? brickValue(brick) : EMPTY], BX + (Shape[brick][rot][i].x + nx) * 2, BY + Shape[brick][rot][i].y + ny, arTile[Show ? brickValue(brick) : EMPTY])
		}
	}

}
void PrintNextBrick()
{
	//다음 벽돌을 표시하기 위한 함수
	//벽돌을 표시하기 위한 장소에 있는 벽돌을 지운다
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 4; k++) {
				putTile(TileColor[EMPTY], 29 + (i) * 2, 3 + (5 * j) + k, arTile[EMPTY]);
			}
		}
	}
	//다음 벽돌을 표시한다
	for (int i = 0; i < 3; i++) {
		if (isBig[i + 1]) {
			putTile(TileColor[BIG_BRICK], 35, (3 + (5 * i)), arTile[BIG_BRICK]);
		}
		for (int j = 0; j < 4; j++) {
			putTile(brickValue(next[i]) == BOMB ? COLOR_BOMB : TileColor[brickValue(next[i])], 31 + (Shape[next[i]][0][j].x) * 2, (5 + (5 * i)) + Shape[next[i]][0][j].y, arTile[brickValue(next[i])]);
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
	if (hold != HOLD_E) {
		if (isBigHold) {
			putTile(TileColor[BIG_BRICK], 45, 3, arTile[BIG_BRICK]);
		}
		for (int i = 0; i < 4; i++) {
			putTile(TileColor[brickValue(hold)], 41 + (Shape[hold][0][i].x) * 2, 5 + Shape[hold][0][i].y, arTile[brickValue(hold)]);
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
	if (hold == HOLD_E) {
		//벽돌의 종류가 7개이므로 존재할수 없는 값임을 이용해 홀드를 하지 않은 초기값임을 표시
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
		//나머지 값일 경우 홀드와 벽돌의 값을 서로 바꿔 다른 벽돌을 떨어지게 함
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
	// 벽돌이 차지한 타일 모양 중 가장 큰 값을 찾는다.
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
			delay(100);
		}
	}
	//벽돌이 일정개수 이상 쌓였을때도 줄을 지우지 못했을 때 벽돌 추가
	if (brickCount >= 9 - difficulty) {
		plusBrickLine(difficulty);
		DrawScreen();
		brickCount = 0;
	}
}

void changeBombColor() {
	//폭탄의 색을 변경하기 위한 함수
	if ((double)(clock() - before) >= 30) {
		if (bombColorTick) {
			TileColor[BOMB] = COLOR_BOMB;
			bombColorTick = (bombColorTick + 1) % 2;
		}
		else {
			TileColor[BOMB] = WHITE;
			bombColorTick = (bombColorTick + 1) % 2;
		}
		before = clock();
	}
}

void explosion() {
	//폭탄이 완전히 떨어졌을 때 주변 벽돌을 없애기 위한 함수
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
					board[x][ty] = board[x][ty - 1];
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
	//벽돌의 값을 1번씩 밀어넣고 그 다음에 올 새 벽돌을 만드는 함수
	brick = next[0];
	next[0] = next[1];
	next[1] = next[2];
	for (int i = 0; i < 3; i++) {
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
	//게임이 반복되는 구간을 함수화
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

		// 벽돌 하나가 바닥에 닿을 때까지의 루프
		nStay = nFrame;
		for (; 2;) {
			changeBombColor();
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
	gameDelay = 1000 / initFrame;
	int x, y;
	randomize();
	clrscr();
	before = clock();
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

	for (int i = 0; i < 3; i++) {
		if (modeselect && sevenBag[sevenBagidx] == random(7)) {
			isBig[i + 1] = TRUE;
		}
		else {
			isBig[i + 1] = FALSE;
		}
		next[i] = createBrick();
	}
	isGameOver = FALSE;
}
void chooseDifficulty() {
	//난이도를 선택하기 위한 함수
	clrscr();
	putsxy(15, 6, "모드와 난이도를 선택해 주세요.");
	putsxy(5, 8, "←→:모드 변경, ↑↓:난이도 변경, ENTER: 난이도 선택");
	putsxy(22, 11, "◀  simple  ▶");
	putsxy(22, 14, "▶  easy");
	putsxy(22, 16, "    normal");
	putsxy(22, 18, "    hard");
	int arrowPosition = 0;
	for (; 1;) {
		if (kbhit()) {
			int ch = getch();
			if (ch == 0xE0 || ch == 0) {
				ch = getch();
				putsxy(22, 14 + arrowPosition, "  ");
				switch (ch) {
				case UP:
					arrowPosition -= 2;
					if (arrowPosition < 0) {
						arrowPosition = 0;
					}
					difficulty = arrowPosition / 2;
					break;
				case DOWN:
					arrowPosition += 2;
					if (arrowPosition > 4) {
						arrowPosition = 4;
					}
					difficulty = arrowPosition / 2;
					break;
				case LEFT:
					if (modeselect == 0) {
						modeselect = 2;
					}
					else {
						modeselect = (modeselect - 1) % 3;
					}
					break;
				case RIGHT:
					modeselect = (modeselect + 1) % 3;
					break;
				}
				if (modeselect == 1) {
					putsxy(22, 11, "◀  bigger  ▶");
				}
				else if (modeselect == 2) {
					putsxy(22, 11, "◀ puyopuyo ▶");
				}
				else {
					putsxy(22, 11, "◀  simple  ▶");
				}
				putsxy(22, 14 + arrowPosition, "▶");
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
	int brick;
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
	//방해용 벽돌을 추가하는 함수
	int empty = random(10) + 1;
	if (lineAmount == 0) {
		return;
	}
	//맨 윗줄에서 방해용 벽돌의 라인수만큼의 줄에 벽돌이 있을경우 게임오버
	for (int i = 0; i < lineAmount - 1; i++) {
		for (int x = 1; x < BW + 1; x++) {
			if (board[x][i + 1] != EMPTY) {
				isGameOver = TRUE;
				return;
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
	for (int ty = BH; ty > BH - lineAmount; ty--) {
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
	//brick,hold,next변수를 board에 저장할 때 arTile배열에 맞게 변환하기 위한 함수
	return (bricks + BOFFSET);
}

void puyoDrop() {
	//공중에 떠있는 뿌요뿌요 블럭을 완전히 바닥이나 뿌요뿌요 블럭위로 떨어지게 하는 함수
	BOOL checkEmpty = FALSE;
	BOOL wholeEmpty = FALSE;
	int y = BH;
	int x = 1;
	int j;
	int line;
	for (int y= BH; y >=2; y--) {
		x = 1;
		line = y;
		do {
			if (board[x][y] == EMPTY) {
				for (int k = y; k >=2; k--) {
					board[x][k] = board[x][k - 1];
					board[x][k - 1]=EMPTY;
				}
			}
			wholeEmpty = TRUE;
			for (j = y; j > 1; j--) {
				if (board[x][j] != EMPTY && board[x][j - 1] == EMPTY) {
					line = j-1;
				}
				else if (board[x][j] == EMPTY && board[x][j - 1] != EMPTY) {
					wholeEmpty = FALSE;
					break;
				}
			}
			if (!wholeEmpty) {
				for (int k = line; k >= 2; k--) {
					board[x][k] = board[x][k - 1];
					board[x][k - 1] = EMPTY;
				}
				continue;
			}	
			if (x == BW) {
				checkEmpty = TRUE;
			}
			else if((x < BW)){
				x++;
			}
		} while (!checkEmpty);
	}
	DrawPuyoScreen();
}
void PrintPuyo(BOOL Show) {
	//지금 떨어지고 있는 뿌요뿌요 블럭을 표시하기 위한 함수
	for (int i = 0; i < 2; i++) {
		putTile(PuyoColor[Show ? brickValue(puyo[i]) : EMPTY], BX + (PuyoShape[rot][i].x + nx) * 2, BY + PuyoShape[rot][i].y + ny, PuyoTile[Show ? brickValue(puyo[i]) : EMPTY]);
	}
}
void PrintNextPuyo() {
	//다음에 나올 뿌요뿌요 블럭들을 표시하는 함수
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 4; k++) {
				putTile(PuyoColor[EMPTY], 29 + (i) * 2, 3 + (5 * j) + k, PuyoTile[EMPTY]);
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			putTile(PuyoColor[brickValue(nextPuyo[i][j])], 31 + (PuyoShape[0][j].x) * 2, (5 + (5 * i)) + PuyoShape[0][j].y, PuyoTile[brickValue(nextPuyo[i][j])]);
		}
	}
}
int GetAroundPuyo(int x, int y, int r) {
	//뿌요뿌요 블럭이 차지한 타일 모양 중 가장 큰 값을 찾는다.
	int k = EMPTY;
	for (int i = 0; i < 2; i++) {
		k = max(k, board[x + PuyoShape[r][i].x][y + PuyoShape[r][i].y]);
	}
	return k;
}

BOOL MoveDownPuyo()
{
	//뿌요뿌요 블럭이 떨어지는 중이거나 완전히 떨어졌을 때 호출되는 함수
	BOOL isChain = FALSE;
	// 바닥에 닿았으면 가득찼는지 점검하고 TRUE를 리턴한다.
	if (GetAroundPuyo(nx, ny + 1, rot) != EMPTY) {
		for (int i = 0; i < 2; i++) {
			board[nx + PuyoShape[rot][i].x][ny + PuyoShape[rot][i].y] = brickValue(puyo[i]);
		}
		puyocnt++;
		do {
			puyoDrop();
			isChain = CheckLinkPuyo();
			if (line >= 5) {
				plusDelay(difficulty);
				line = 0;
			}
		} while (isChain);
		if (puyocnt>= (9 - difficulty)*2) {
			plusPuyoLine(difficulty);
			puyocnt = 0;
		}
		DrawPuyoScreen();
		return TRUE;
	}
	// 아직 공중에 떠 있으면 한칸 아래로 내린다.
	PrintPuyo(FALSE);
	ny++;
	PrintPuyo(TRUE);
	return FALSE;
}

BOOL CheckLinkPuyo() {
	//뿌요뿌요 블럭이 연결되어 있는지 체크하고 4개이상 연결된 뿌요뿌요 블럭을 지우는 함수
	memset(checked, 0, sizeof(int) * ((BW+2) * (BH+2)));
	int count = 0;
	int puyocount = 1;
	BOOL isLink = FALSE;
	for (int y = 1; y < BH + 1; y++) {
		for (int x = 1; x < BW + 1; x++) {
			if (checked[x][y] == 0 && board[x][y]!=EMPTY && board[x][y] != PUYO_W) {
				if (checked[x + 1][y] != 0 && board[x][y] == board[x + 1][y]) {
					checked[x][y] = checked[x + 1][y];
				}
				else if (checked[x - 1][y] != 0 && board[x][y] == board[x - 1][y]) {
					checked[x][y] = checked[x - 1][y];
				}
				else if (checked[x][y + 1] != 0 && board[x][y] == board[x][y + 1]) {
					checked[x][y] = checked[x][y + 1];
				}
				else if (checked[x][y - 1] != 0 && board[x][y] == board[x][y - 1]) {
					checked[x][y] = checked[x][y - 1];
				}
			}
		}
		for (int x = BW; x >= 1; x--) {
			if (checked[x][y] == 0 && board[x][y] != EMPTY && board[x][y] != PUYO_W) {
				if (checked[x + 1][y] != 0 && board[x][y] == board[x + 1][y]) {
					checked[x][y] = checked[x + 1][y];
				}
				else if (checked[x - 1][y] != 0 && board[x][y] == board[x - 1][y]) {
					checked[x][y] = checked[x - 1][y];
				}
				else if (checked[x][y + 1] != 0 && board[x][y] == board[x][y + 1]) {
					checked[x][y] = checked[x][y + 1];
				}
				else if (checked[x][y - 1] != 0 && board[x][y] == board[x][y - 1]) {
					checked[x][y] = checked[x][y - 1];
				}
				else {
					checked[x][y] = puyocount;
					puyocount++;
				}
			}
		}
	}
	isLink = FALSE;
	for (int i = 1; i <= puyocount; i++) {
		count = 0;
		for (int y = BH; y >= 1; y--) {
			for (int x = BW; x >= 1; x--) {
				if (checked[x][y] == i) {
					count++;
				}
			}
		}
		if (count > 3) {
			line++;
			for (int y = BH; y >= 1; y--) {
				for (int x = BW; x >= 1; x--) {
					if (checked[x][y] == i) {
						isLink = TRUE;
						if (board[x + 1][y] == PUYO_W) {
							board[x + 1][y] = EMPTY;
						}
						if (board[x - 1][y] == PUYO_W) {
							board[x - 1][y] = EMPTY;
						}
						if (board[x][y + 1] == PUYO_W) {
							board[x][y + 1] = EMPTY;
						}
						if (board[x][y - 1] == PUYO_W) {
							board[x][y - 1] = EMPTY;
						}
						board[x][y] = EMPTY;
					}
				}
			}
		}
	}
	if (isLink) {
		puyocnt = 0;
	}
	return isLink;
}
void DrawPuyoScreen()
{
	//뿌요뿌요 게임의 화면을 표시하는 함수
	for (int x = 0; x < BW + 7; x++) {
		for (int y = 0; y < BH + 2; y++) {
			if (x < BW + 2) {
				putTile(PuyoColor[board[x][y]], BX + x * 2, BY + y, PuyoTile[board[x][y]]);
			}
			else if (y < BH - 3) {
				putTile(PuyoColor[infoBoard[x - (BW + 1)][y]], BX + x * 2, BY + y, PuyoTile[infoBoard[x - (BW + 1)][y]]);
			}

		}
	}
	char buffer[3];
	_itoa_s(gameDelay, buffer, 3, 10);
	putsxy(55, 3, "PuyoPuyo ver 1.0");
	putsxy(55, 5, "좌우:이동, 위:회전, 아래:내림");
	putsxy(55, 7, "스페이스 바:전부 내림");
	putsxy(55, 9, "gameDelay : ");
	putsxy(67, 9, buffer);
	putsxy(29, 2, "n e x t");
	PrintNextPuyo();
}

BOOL ProcessKeyPuyo()
{
	//뿌요뿌요 게임중 키가 눌렸을 때 실행될 함수
	if (kbhit()) {
		int ch = getch();
		if (ch == 0xE0 || ch == 0) {
			ch = getch();
			switch (ch) {
			case LEFT:
				if (GetAroundPuyo(nx - 1, ny, rot) == EMPTY) {
					PrintPuyo(FALSE);
					nx--;
					PrintPuyo(TRUE);
				}
				break;
			case RIGHT:
				if (GetAroundPuyo(nx + 1, ny, rot) == EMPTY) {
					PrintPuyo(FALSE);
					nx++;
					PrintPuyo(TRUE);
				}
				break;
			case UP:
				if (GetAroundPuyo(nx, ny, (rot + 1) % 4) == EMPTY) {
					PrintPuyo(FALSE);
					rot = (rot + 1) % 4;
					PrintPuyo(TRUE);
				}
				break;
			case DOWN:
				if (MoveDownPuyo()) {
					return TRUE;
				}
				break;
			}
		}
		else {
			switch (ch) {
			case ' ':
				while (MoveDownPuyo() == FALSE) { ; }
				return TRUE;
			case ESC:
				exit(0);
			}
		}
	}
	return FALSE;
}

void pushPuyo() {
	//뿌요뿌요 블럭을 하나씩 밀고 새 뿌요뿌요 블럭을 만드는 함수
	for (int i = 0; i < 2; i++) {
		puyo[i] = nextPuyo[0][i];
		nextPuyo[0][i] = nextPuyo[1][i];
		nextPuyo[1][i] = nextPuyo[2][i];
	}
	createPuyo(nextPuyo[2]);
}
void loopPuyoGame(int nFrame) {
	//뿌요뿌요 게임을 루프시키는 함수
	int nStay;
	for (; !isGameOver;) {
		pushPuyo();
		nx = BW / 2;
		if (isBig[0]) {
			ny = 4;
		}
		else {
			ny = 3;
		}
		rot = 0;
		PrintPuyo(TRUE);
		PrintNextPuyo();
		if (GetAroundPuyo(nx, ny, rot) != EMPTY) break;

		//뿌요뿌요 블럭 하나가 바닥에 닿을 때까지의 루프
		nStay = nFrame;
		for (; 2;) {
			if (--nStay == 0) {
				nStay = nFrame;
				if (MoveDownPuyo()) break;
			}
			if (ProcessKeyPuyo()) break;
			delay(gameDelay);
		}
	}
	clrscr();
}
void initPuyoGame(int initFrame) {
	//뿌요뿌요 게임을초기화 하는 함수
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
	//다음 뿌요뿌요 블럭이 표시될 공간을 초기화
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
	DrawPuyoScreen();

	//다음 뿌요뿌요 벽돌을 초기화
	for (int i = 0; i < 3; i++) {
		createPuyo(nextPuyo[i]);
	}
	isGameOver = FALSE;
}
void createPuyo(int puyo[]) {
	//뿌요뿌요 블럭을 생성하는 함수
	puyo[0] = random(5);
	puyo[1] = random(5);
}
void plusPuyoLine(int lineAmount) {
	//방해용 벽돌을 추가하는 함수
	if (lineAmount == 0) {
		return;
	}
	//맨 윗줄에서 방해용 뿌요뿌요 블럭의 라인수만큼 떨어뜨리고 그 줄에 뿌요뿌요 블럭이 있을경우 게임오버
	for (int i = 0; i < lineAmount; i++) {
		for (int x = 1; x < BW + 1; x++) {
			if (board[x][1] != EMPTY) {
				isGameOver = TRUE;
				return;
			}
			else {
				board[x][1] = PUYO_W;
			}
		}
		puyoDrop();
	}
	
}