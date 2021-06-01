#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h> 
#include <stdbool.h>
#include <mmsystem.h>
#include <stdlib.h>
#include "cful.h"

#pragma comment(lib, "winmm.lib") //사운드

clock_t startDropT, endT, startGroundT;
clock_t startSpaceT, endSpaceT;

int x = 8, y = 0;
RECT blockSize;
int blockForm;
int blockRotation = 0;
int key;
bool isSpace = false;
int blockNum[7] = { 0 }, blockCnt = 8;
bool isHold = false;
bool isHoldAlready = false;
int holdBlockForm;
bool isMusic = true;

#define Width 90  // 창 가로 크기
#define Height 30  // 창 세로 크기
#define kbhit _kbhit
#define getch _getch

int block[7][4][4][4] = {  // [7]은 블럭갯수. 랜덤으로 변환, 2차원 [4]는 증감할때마다 회전
	{ // T모양 블럭
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,1,0,0}
		}
	},
	{    // 번개 블럭
		{
			{0,0,0,0},
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,1,0,0}
		}
	},
	{   // 번개 블럭 반대
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{1,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{1,0,0,0}
		}
	},
	{   // 1자형 블럭
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		}
	},
	{   // L자형 블럭
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{1,0,0,0},
			{1,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,0,0,0}
		}
	},
	{   // L자형 블럭 반대
		{
			{0,0,0,0},
			{0,0,1,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{1,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0},
			{1,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		}
	},
	{   // 네모 블럭
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		}
	}
};

int space[1 + 20 + 1][10 + 2] = {  // 세로 (위벽)1+15+1(아래벽)칸, 가로 10+2(양쪽 벽)칸  
	{1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1}
};

int UIspace[22][20] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

void Console_Size(); // 콘솔 사이즈 설정
void CursorView(char show); // 커서 깜빡임 숨기기. 0이면 숨김, 1이면 보임
void gotoxy(int x, int y); //커서 이동 함수

void DesignMainMenu(); // 메인 메뉴 디자인
int MainMenu(); // 메인 메뉴
void MenuTwo(); // 조작법 메뉴
void MenuThree(); // 제작자 메뉴
void MenuOne(); // 게임시작 메뉴
void DrawMap(); // 맵의 형태와 쌓인 블럭 그림
void DrawBlock(); // 현재 블럭 그리기 (4차원배열)
void DropBlock(); // 0.8초마다 블럭을 한칸씩 밑으로 내림
void BlockToGround(); // 1초동안 땅에 닿아있을때 동작이 없으면 땅으로 변함, 랜덤한 블럭을 만들고 위로 올림
void RemoveLine(); // 블럭제거 후 한칸씩 땡김
void InputKey();
void CreateRandomForm(); // 블럭이 내려올때마다 랜덤으로 바뀜. 0~6
bool CheckCrash(int x, int y); // 충돌감지 겹치는게 있으면 true를 반환
void ShowBlockArrivePosition(); // 블럭의 도착 추정 위치 표시
void DrawUI(); // Map 옆부분 UI 그리기
void ShowNextBlock(); // 다음 블럭 표시
void HoldFunction(); // 블럭 홀드 기능
void OptionMenu(); // 옵션 메뉴

int main() {
	srand(time(NULL));
	SetConsoleTitle(TEXT("NEW TETRIS"));
	CursorView(0);  // 커서 깜빡임 숨기기. 0이면 숨김, 1이면 보임
	Console_Size(); // 콘솔 사이즈 설정
	DesignMainMenu(); // 메인메뉴 디자인 출력
	PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // 배경음악 재생

	while (1) // 게임 메뉴 선택
	{
		int return_n = MainMenu();

		switch (return_n)
		{
		case 0:
			MenuOne();
			break;
		case 3:
			MenuTwo();
			break;
		case 6:
			MenuThree();
			break;
		default:
			exit(1);
		}
	}

	return 0;
}

void gotoxy(int x, int y) //커서 이동 함수
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Console_Size() // 콘솔 사이즈 설정
{
	char temp[50];
	sprintf(temp, "Mode con cols=%d lines=%d", Width, Height);
	system(temp);
}

void CursorView(char show)  // 커서 깜빡임 숨기기. 0이면 숨김, 1이면 보임
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void DesignMainMenu() // 메인 메뉴 디자인
{
	printf("\n\n");
	printf("         ■■■■■   ■■■   ■■■■■   ■■■■      ■■■■■     ■■■■  \n");
	printf("             ■       ■           ■       ■      ■        ■        ■         \n");
	printf("             ■       ■■■       ■       ■■■■          ■         ■■■■  \n");
	printf("             ■       ■           ■       ■     ■         ■                ■  \n");
	printf("             ■       ■■■       ■       ■      ■    ■■■■■     ■■■■  \n");
	printf("\n\n\n");
}

int MainMenu()
{
	system("cls");
	DesignMainMenu();

	gotoxy(Width / 2 - 4, Height / 2);
	printf("게임 시작");
	gotoxy(Width / 2 - 4, Height / 2 + 3);
	printf("조작법");
	gotoxy(Width / 2 - 4, Height / 2 + 6);
	printf("제작자");
	gotoxy(Width / 2 - 4, Height / 2 + 9);
	printf("게임 종료");
	gotoxy(Width / 2 + 8, Height / 2);
	printf("◀");

	int return_n = 0;
	while (1) // 키보드 움직임
	{
		int key;
		if (kbhit()) // 키보드 입력이 들어왔을 경우
		{
			key = getch(); // 그 키의 아스키코드값을 받아 key에 저장
			if (key == 224 || key == 0) // 그 키가 방향키인 경우 작동
			{
				key = getch();
				switch (key)
				{
				case 72: // 위쪽 방향키
					gotoxy(Width / 2 + 8, Height / 2 + return_n); //원래 자리로 이동
					printf("  "); //삭제
					return_n -= 3; //화살표의 좌표를 위로 3변경시키고
					if (return_n < 0) return_n = 0; //범위밖으로 나가지 못하게
					gotoxy(Width / 2 + 8, Height / 2 + return_n);
					printf("◀"); //바뀐 좌표에 방향키 출력
					break;
				case 80: //아래쪽 방향키를 누른 경우
					gotoxy(Width / 2 + 8, Height / 2 + return_n); //원래 자리로 이동
					printf("  "); //삭제
					return_n += 3; //화살표의 좌표를 아래로 3변경시키고
					if (return_n > 6) return_n = 9; //범위밖으로 나가지 못하게
					gotoxy(Width / 2 + 8, Height / 2 + return_n);
					printf("◀"); //바뀐 좌표에 방향키 출력
					break;
				default:
					break;
				}
			}
			else
				if (key == 13) //엔터키를 눌렀을 때
					return return_n; //화살표의 좌표값을 반환
		}
	}
}

void MenuTwo() // 조작법 메뉴
{
	system("cls");
	DesignMainMenu();
	gotoxy(Width / 2 - 3, Height / 2 - 6);
	printf("조작키");
	gotoxy(Width / 2 - 10, Height / 2 - 4);
	printf("←, → : Move Left, Right");
	gotoxy(Width / 2 - 6, Height / 2 - 2);
	printf("↓ : Soft Drop");
	gotoxy(Width / 2 - 9, Height / 2);
	printf("Space : Hard Drop");
	gotoxy(Width / 2 - 6, Height / 2 + 2);
	printf("↑ : Rotate");
	gotoxy(Width / 2 - 9, Height / 2 + 4);
	printf("Shift : Hold");

	gotoxy(Width / 2 - 8, Height / 2 + 7);
	printf("종료하시겠습니까?");
	gotoxy(Width / 2 - 6, Height / 2 + 9);
	printf("예");
	gotoxy(Width / 2 + 1, Height / 2 + 9);
	printf("아니오");
	gotoxy(Width / 2 - 3, Height / 2 + 9);
	printf("◀");

	int return_n = 0;
	while (1) //키보드 움직임
	{
		int key;
		if (kbhit())
		{
			key = getch();
			if (key == 224 || key == 0)
			{
				key = getch();
				switch (key)
				{
				case 75: //좌 기본
					gotoxy(Width / 2 - 3 + return_n, Height / 2 + 9);
					printf("  ");
					return_n = 0;
					gotoxy(Width / 2 - 3, Height / 2 + 9);
					printf("◀");
					break;
				case 77: //우 11칸앞
					return_n = 11;
					gotoxy(Width / 2 + 8 - return_n, Height / 2 + 9);
					printf("  ");
					gotoxy(Width / 2 + 8, Height / 2 + 9);
					printf("◀");
					break;
				default:
					break;
				}
			}
			else
				if (key == 13)//엔터
				{
					if (return_n == 0)
						exit(1);
					else
						break;
				}
		}
	}
}

void MenuThree() // 제작자 메뉴
{
	system("cls");
	DesignMainMenu();
	gotoxy(Width / 2 - 2, Height / 2 - 6);
	printf("제작");
	gotoxy(Width / 2 - 15, Height / 2 - 3);
	printf("중앙대학교 소프트웨어학부 21학번");
	gotoxy(Width / 2 - 7, Height / 2);
	printf("김재오, 김여진");
	gotoxy(Width / 2 - 7, Height / 2 + 2);
	printf("이우진, 전수빈");

	gotoxy(Width / 2 - 8, Height / 2 + 7);
	printf("종료하시겠습니까?");
	gotoxy(Width / 2 - 6, Height / 2 + 9);
	printf("예");
	gotoxy(Width / 2 + 1, Height / 2 + 9);
	printf("아니오");
	gotoxy(Width / 2 - 3, Height / 2 + 9);
	printf("◀");

	int return_n = 0;
	while (1) //키보드 움직임
	{
		int key;
		if (kbhit())
		{
			key = getch();
			if (key == 224 || key == 0)
			{
				key = getch();
				switch (key)
				{
				case 75: //좌 기본
					gotoxy(Width / 2 - 3 + return_n, Height / 2 + 9);
					printf("  ");
					return_n = 0;
					gotoxy(Width / 2 - 3, Height / 2 + 9);
					printf("◀");
					break;
				case 77: //우 11칸앞
					return_n = 11;
					gotoxy(Width / 2 + 8 - return_n, Height / 2 + 9);
					printf("  ");
					gotoxy(Width / 2 + 8, Height / 2 + 9);
					printf("◀");
					break;
				default:
					break;
				}
			}
			else
				if (key == 13)//엔터
				{
					if (return_n == 0)
						exit(1);
					else
						break;
				}
		}
	}
}

void OptionMenu() // 옵션 메뉴
{
	system("cls");
	gotoxy(Width / 2 - 5, Height / 2 - 12);
	printf("옵   션");
	gotoxy(Width / 2 - 7, Height / 2 - 4);
	printf("배 경 음 악");
	gotoxy(Width / 2 - 9, Height / 2 + 1);
	printf("ON");
	gotoxy(Width / 2 + 3, Height / 2 + 1);
	printf("OFF");
	gotoxy(Width / 2 + 8, Height / 2 + 1);
	printf("◀");

	int return_n = 13;
	while (1) { // 키보드 움직임
		int key;
		if (kbhit()) {
			key = getch();
			if (key == 224 || key == 0) {
				key = getch();
				switch (key) {
				case 75: // 왼쪽
					gotoxy(Width / 2 - 5 + return_n, Height / 2 + 1);
					printf("   ");
					return_n = 0;
					gotoxy(Width / 2 - 5, Height / 2 + 1);
					printf("◀");
					break;
				case 77: // 오른쪽
					return_n = 13;
					gotoxy(Width / 2 + 8 - return_n, Height / 2 + 1);
					printf("   ");
					gotoxy(Width / 2 + 8, Height / 2 + 1);
					printf("◀");
					break;
				default:
					break;
				}
			}
			else {
				if (key == 13) // 엔터
				{
					if (return_n == 0) // ON
					{
						if (isMusic == false) PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // 배경음악 재생
						isMusic = true;
						break;
					}
					else // OFF
					{
						PlaySound(NULL, 0, 0);
						isMusic = false;
						break;
					}
				}
			}
		}
	}
}

void MenuOne() // 게임시작 메뉴
{
	system("cls");
	startDropT = clock();
	endSpaceT = clock();
	CreateRandomForm();

	while (1)
	{
		DrawMap();
		ShowNextBlock();
		DrawUI();
		ShowBlockArrivePosition();
		DrawBlock();
		DropBlock();
		BlockToGround();
		RemoveLine();
		InputKey();
	}
}

void CreateRandomForm() { // 랜덤 수 생성 (7bag 시스템 구현)
	isHoldAlready = false;
	if (blockCnt < 7) {
		blockForm = blockNum[blockCnt++];
		return;
	}
	blockCnt = 0; // 여기가 실행되면 blockCnt가 7일때 이므로 초기화

	for (int i = 0; i < 7; i++)
		blockNum[i] = i;
	
	int i, j;
	int n = 25; // 변경 횟수
	while (n--) {
		int tmp;
		i = rand() % 7;
		j = rand() % 7;

		tmp = blockNum[i];
		blockNum[i] = blockNum[j];
		blockNum[j] = tmp;
	}
	CreateRandomForm();
}

void ShowNextBlock() {	// 다음 블럭 표시
	for (int i = 4; i < 8; i++) {
		for (int j = 1; j < 6; j++) {
			UIspace[i][j] = 0;
		}
	}

	if (blockCnt != 7) {
		switch (blockNum[blockCnt]) {
		case 0: // T자
			UIspace[5][3] = 2;
			UIspace[6][2] = 2;
			UIspace[6][3] = 2;
			UIspace[6][4] = 2;
			break;
		case 1: // 오른쪽번개블럭
			UIspace[5][3] = 3;
			UIspace[5][4] = 3;
			UIspace[6][2] = 3;
			UIspace[6][3] = 3;
			break;
		case 2: // 왼쪽번개블럭
			UIspace[5][2] = 4;
			UIspace[5][3] = 4;
			UIspace[6][3] = 4;
			UIspace[6][4] = 4;
			break;
		case 3: // I자 블럭
			UIspace[6][2] = 5;
			UIspace[6][3] = 5;
			UIspace[6][4] = 5;
			UIspace[6][5] = 5;
			break;
		case 4: // L자 반대블럭
			UIspace[4][3] = 6;
			UIspace[5][3] = 6;
			UIspace[6][3] = 6;
			UIspace[6][4] = 6;
			break;
		case 5: // L자블럭
			UIspace[4][3] = 7;
			UIspace[5][3] = 7;
			UIspace[6][2] = 7;
			UIspace[6][3] = 7;
			break;
		case 6: // 네모블럭
			UIspace[5][2] = 8;
			UIspace[5][3] = 8;
			UIspace[6][2] = 8;
			UIspace[6][3] = 8;
			break;
		}
	}
	else {
		UIspace[5][2] = 9;
		UIspace[5][3] = 9;
		UIspace[5][4] = 9;
		UIspace[6][2] = 9;
		UIspace[6][3] = 9;
		UIspace[6][4] = 9;
	}
}

void DrawMap()
{
	gotoxy(11, 1);
	printf(FG_COLOR(255, 0, 0) "요 " RESET);
	printf(FG_COLOR(255, 127, 0) "리 " RESET);
	printf(FG_COLOR(255, 255, 0) "왕 " RESET);
	printf(FG_COLOR(0, 255, 0) "비 " RESET);
	printf(FG_COLOR(0, 255, 255) "룡" RESET);
	gotoxy(12, 4);
	printf(FG_COLOR(255, 0, 0) "T " RESET);
	printf(FG_COLOR(255, 127, 0) "E " RESET);
	printf(FG_COLOR(255, 255, 0) "T " RESET);
	printf(FG_COLOR(0, 255, 0) "R " RESET);
	printf(FG_COLOR(0, 255, 255) "I " RESET);
	printf(FG_COLOR(148, 0, 211) "S" RESET);

	gotoxy(0, 0);
	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 12; j++) {
			switch (space[i][j]) {
			case 1:
				gotoxy(j * 2 + 6, i + 6);
				printf("▣");
				break;
			case 2: // blockForm = 0
				gotoxy(j * 2 + 6, i + 6);
				printf(FG_COLOR(255, 0, 255) "■" RESET); // 보라색, T자블럭
				break;
			case 3: // blockForm = 1
				gotoxy(j * 2 + 6, i + 6);
				printf(FG_COLOR(0, 255, 51) "■" RESET); // 초록색, 오른쪽번개블럭
				break;
			case 4: // blockForm = 2
				gotoxy(j * 2 + 6, i + 6);
				printf(FG_COLOR(255, 0, 0) "■" RESET); // 빨간색, 왼쪽번개블럭
				break;
			case 5: // blockForm = 3
				gotoxy(j * 2 + 6, i + 6);
				printf(FG_COLOR(0, 255, 255) "■" RESET); // 하늘색, I자 블럭
				break;
			case 6: // blockForm = 4
				gotoxy(j * 2 + 6, i + 6);
				printf(FG_COLOR(0, 102, 255) "■" RESET); // 파랑색, L자반대블럭
				break;
			case 7: // blockForm = 5
				gotoxy(j * 2 + 6, i + 6);
				printf(FG_COLOR(255, 127, 0) "■" RESET); // 주황색, L자블럭
				break;
			case 8: // blockForm = 6
				gotoxy(j * 2 + 6, i + 6);
				printf(FG_COLOR(255, 255, 0) "■" RESET); // 노랑색, ㅁ자블럭
				break;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
}

void DrawUI() {
	gotoxy(34, 7);
	printf("NEXT BLOCK");
	gotoxy(35, 18);
	printf("H O L D");

	gotoxy(33, 1);
	printf("ESC : OPTION,");
	gotoxy(38, 3);
	printf("일시정지");

	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 20; j++) {
			switch (UIspace[i][j]) {
			case 1:
				gotoxy(j * 2 + 32, i + 6);
				printf("▣");
				break;
			case 2: // blockForm = 0
				gotoxy(j * 2 + 32, i + 6);
				printf(FG_COLOR(255, 0, 255) "■" RESET); // 보라색, T자블럭
				break;
			case 3: // blockForm = 1
				gotoxy(j * 2 + 32, i + 6);
				printf(FG_COLOR(0, 255, 51) "■" RESET); // 초록색, 오른쪽번개블럭
				break;
			case 4: // blockForm = 2
				gotoxy(j * 2 + 32, i + 6);
				printf(FG_COLOR(255, 0, 0) "■" RESET); // 빨간색, 왼쪽번개블럭
				break;
			case 5: // blockForm = 3
				gotoxy(j * 2 + 32, i + 6);
				printf(FG_COLOR(0, 255, 255) "■" RESET); // 하늘색, I자 블럭
				break;
			case 6: // blockForm = 4
				gotoxy(j * 2 + 32, i + 6);
				printf(FG_COLOR(0, 102, 255) "■" RESET); // 파랑색, L자반대블럭
				break;
			case 7: // blockForm = 5
				gotoxy(j * 2 + 32, i + 6);
				printf(FG_COLOR(255, 127, 0) "■" RESET); // 주황색, L자블럭
				break;
			case 8: // blockForm = 6
				gotoxy(j * 2 + 32, i + 6);
				printf(FG_COLOR(255, 255, 0) "■" RESET); // 노랑색, ㅁ자블럭
				break;
			case 9:
				gotoxy(j * 2 + 32, i + 6);
				printf("?");
				break;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
}

void DrawBlock()
{
	switch (blockForm) {
	case 0:
		SET_FG_COLOR(255, 0, 255); // 보라색, T자블럭
		break;
	case 1:
		SET_FG_COLOR(0, 255, 51); // 초록색, 오른쪽번개블럭
		break;
	case 2:
		SET_FG_COLOR(255, 0, 0); // 빨간색, 왼쪽번개블럭
		break;
	case 3:
		SET_FG_COLOR(0, 255, 255); // 하늘색, I자 블럭
		break;
	case 4:
		SET_FG_COLOR(0, 102, 255); // 파랑색, L자반대블럭
		break;
	case 5:
		SET_FG_COLOR(255, 127, 0); // 주황색, L자블럭
		break;
	case 6:
		SET_FG_COLOR(255, 255, 0); // 노랑색, ㅁ자블럭
		break;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
				gotoxy(x + j * 2 + 6, y + i + 6);
				printf("■");
			}
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void DropBlock() // 0.8초마다 블럭을 한칸씩 밑으로 내림
{
	endT = clock();
	if ((float)(endT - startDropT) >= 800) {
		if (CheckCrash(x, y + 1) == true) return;
		y++;
		startDropT = clock();
		startGroundT = clock();
		system("cls");
	}
}

void BlockToGround() { // 1초동안 땅에 닿아있을때 동작이 없으면 땅으로 변함, 랜덤한 블럭을 만들고 위로 올림
	if (CheckCrash(x, y + 1) == true) {
		if ((float)(endT - startGroundT) > 1000 || isSpace == true) {
			// 현재 블록 저장
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (block[blockForm][blockRotation][i][j] == 1) {
						switch (blockForm) {
						case 0:
							space[i + y][j + x / 2] = 2;
							break;
						case 1:
							space[i + y][j + x / 2] = 3;
							break;
						case 2:
							space[i + y][j + x / 2] = 4;
							break;
						case 3:
							space[i + y][j + x / 2] = 5;
							break;
						case 4:
							space[i + y][j + x / 2] = 6;
							break;
						case 5:
							space[i + y][j + x / 2] = 7;
							break;
						case 6:
							space[i + y][j + x / 2] = 8;
							break;
						}
					}
				}
			}
			isSpace = false;
			x = 8;
			y = 1;
			CreateRandomForm();
		}
	}
}

void RemoveLine() {
	for (int i = 20; i > 0; i--) {
		int cnt = 0;
		for (int j = 1; j < 11; j++) {
			if (space[i][j] == 2 || space[i][j] == 3 || space[i][j] == 4 || space[i][j] == 5 || space[i][j] == 6 || space[i][j] == 7 || space[i][j] == 8) {
				cnt++;
			}
		}

		if (cnt >= 10) { // 한줄이 다 찼으면
			for (int j = i; j > 1; j--) {
				for (int k = 1; k < 11; k++) {
					space[j][k] = space[j - 1][k];
				}
			}
		}
	}
}

void InputKey() {
	if (_kbhit()) {
		key = _getch();
		switch (key) {
		case 72: // up
			blockRotation++;
			if (blockRotation >= 4) blockRotation = 0;
			startGroundT = clock();
			break;
		case 75: // left
			if (CheckCrash(x - 2, y) == false) {
				x -= 2;
				startGroundT = clock();
			}
			break;
		case 77: // right
			if (CheckCrash(x + 2, y) == false) {
				x += 2;
				startGroundT = clock();
			}
			break;
		case 80: // down
			if (CheckCrash(x, y + 1) == false)
				y++;
			break;
		case 32: // space
			endSpaceT = clock();
			if ((float)(endSpaceT - startSpaceT >= 500)) { // 연속입력 방지
				while (1) {
					if (CheckCrash(x, y + 1) == false) y++;
					else break;
				}
				isSpace = true;
			}
			startSpaceT = clock();
			break;
		case 67: // C
		case 99: // c
			HoldFunction();
			break;
		case 27: // ESC
			OptionMenu();
			break;
		}
		system("cls");
	}
}

bool CheckCrash(int x, int y) { // 충돌감지 겹치는게 있으면 true를 반환
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
				int t = space[i + y][j + x / 2];
				if (t == 1 || t == 2 || t == 3 || t == 4 || t == 5 || t == 6 || t == 7 || t == 8) { // 벽일 때, 블럭일 때
					return true;
				}
			}
		}
	}
	return false;
}

void ShowBlockArrivePosition() { // 블럭의 도착 추정 위치 표시
	int k = y;
	while (1) {
		if (CheckCrash(x, k + 1) == false) k++;
		else break;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1 && k != y) {
				gotoxy(x + j * 2 + 6, k + i + 6);
				switch (blockForm) {
				case 0:
					printf(FG_COLOR(255, 0, 255) "□" RESET); // 보라색, T자블럭
					break;
				case 1:
					printf(FG_COLOR(0, 255, 51) "□" RESET); // 초록색, 오른쪽번개블럭
					break;
				case 2:
					printf(FG_COLOR(255, 0, 0) "□" RESET); // 빨간색, 왼쪽번개블럭
					break;
				case 3:
					printf(FG_COLOR(0, 255, 255) "□" RESET); // 하늘색, I자 블럭
					break;
				case 4:
					printf(FG_COLOR(0, 102, 255) "□" RESET); // 파랑색, L자반대블럭
					break;
				case 5:
					printf(FG_COLOR(255, 127, 0) "□" RESET); // 주황색, L자블럭
					break;
				case 6:
					printf(FG_COLOR(255, 255, 0) "□" RESET); // 노랑색, ㅁ자블럭
					break;
				}
			}
		}
	}
}

void HoldFunction() { // 블럭 홀드 기능
	if (isHoldAlready == false) {
		for (int i = 15; i < 19; i++) {
			for (int j = 1; j < 6; j++) {
				UIspace[i][j] = 0;
			}
		}

		switch (blockForm) {
		case 0: // T자
			UIspace[16][3] = 2;
			UIspace[17][2] = 2;
			UIspace[17][3] = 2;
			UIspace[17][4] = 2;
			break;
		case 1: // 오른쪽번개블럭
			UIspace[16][3] = 3;
			UIspace[16][4] = 3;
			UIspace[17][2] = 3;
			UIspace[17][3] = 3;
			break;
		case 2: // 왼쪽번개블럭
			UIspace[16][2] = 4;
			UIspace[16][3] = 4;
			UIspace[17][3] = 4;
			UIspace[17][4] = 4;
			break;
		case 3: // I자 블럭
			UIspace[17][2] = 5;
			UIspace[17][3] = 5;
			UIspace[17][4] = 5;
			UIspace[17][5] = 5;
			break;
		case 4: // L자 반대블럭
			UIspace[15][3] = 6;
			UIspace[16][3] = 6;
			UIspace[17][3] = 6;
			UIspace[17][4] = 6;
			break;
		case 5: // L자블럭
			UIspace[15][3] = 7;
			UIspace[16][3] = 7;
			UIspace[17][2] = 7;
			UIspace[17][3] = 7;
			break;
		case 6: // 네모블럭
			UIspace[16][2] = 8;
			UIspace[16][3] = 8;
			UIspace[17][2] = 8;
			UIspace[17][3] = 8;
			break;
		}

		if (isHold == true) {
			int tmp = blockForm;
			blockForm = holdBlockForm;
			holdBlockForm = tmp;
			isHoldAlready = true;
		}
		else {
			isHold = true;
			holdBlockForm = blockForm;
			CreateRandomForm();
		}
		system("cls");
		x = 8;
		y = 1;
	}
}