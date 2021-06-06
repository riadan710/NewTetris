﻿#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h> 
#include <stdbool.h>
#include <mmsystem.h>
#include <stdlib.h>
#include "cful.h"
#include "food.h"
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "winmm.lib") //사운드

#define kbhit _kbhit
#define getch _getch
#define boardWidth 30
#define boardHeight 27

clock_t startDropT, endT, startGroundT;
clock_t startSpaceT, endSpaceT;
clock_t startItemT, endItemT;

RECT blockSize;

int Width = 90, Height = 30; // 창 가로, 세로 길이
int x = 8, y = 0;
int blockForm;
int blockRotation = 0;
int key;
int blockNum[7] = { 0 }, blockCnt = 8;
int holdBlockForm;
int colorGauge[7] = { 0 };
int themenum = 0;
int stagenum = 1;
int downspeed = 0;
int Number_Line = 1, Number_Color = 1, Number_Speed = 1, Number_Block = 1;

bool isSpace = false;
bool isHold = false;
bool isHoldAlready = false;
bool isMusic = true;
bool isFirst = true;
bool isThemeClear[3] = { false };
bool isEnter = false;
bool isSlowItem = false;
bool isColor[7][2] = { false };
bool isStageClear = false;
bool isBlock = false;

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

int Themespace[10][10] = {
	{1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,2,2,2,0,0,0,1},
	{1,0,2,0,0,0,2,0,0,1},
	{1,0,0,0,0,2,0,0,0,1},
	{1,0,0,0,2,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,2,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1}
};

void Console_Size(); // 콘솔 사이즈 설정
void CursorView(char show); // 커서 깜빡임 숨기기. 0이면 숨김, 1이면 보임
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
void CheckEnding(); // 게임 종료 체크
void Gauge(int line); // 경험치 함수
void DeleteArea(int x1, int y1, int x2, int y2); // 범위 지정 삭제
void RemoveSelectedLine(); // 원하는 줄 지우기 아이템
void RemoveSelectedColor(); // 원하는 색 지우기 아이템
void SelectTheme(); // 테마 선택 메뉴
void Theme1();
void Theme2();
void Theme3();
void SlowFallSpeed(); // 하강 속도 일시적 감소 아이템
void DrawGauge(); // 게이즈 출력
void CheckClear(); // 스테이지 클리어 체크
void SelectBlock(); // 원하는 블럭 선택 아이템

int main() {
	srand(time(NULL));
	SetConsoleTitle(TEXT("NEW TETRIS"));
	CursorView(0);  // 커서 깜빡임 숨기기. 0이면 숨김, 1이면 보임
	Console_Size(); // 콘솔 사이즈 설정
	DesignMainMenu(); // 메인메뉴 디자인 출력
	//PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // 배경음악 재생

	while (1) // 게임 메뉴 선택
	{
		int return_n = MainMenu();

		switch (return_n)
		{
		case 0:
			SelectTheme();
			//MenuOne();
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
	gotoxy(Width / 2 - 30, Height / 2 - 4);
	printf("←, → : Move Left, Right");
	gotoxy(Width / 2 - 26, Height / 2 - 2);
	printf("↓ : Soft Drop");
	gotoxy(Width / 2 - 29, Height / 2);
	printf("Space : Hard Drop");
	gotoxy(Width / 2 - 26, Height / 2 + 2);
	printf("↑ : Rotate");
	gotoxy(Width / 2 - 25, Height / 2 + 4);
	printf("C : Hold");
	gotoxy(Width / 2 + 5, Height / 2 - 4);
	printf("D : Select Line to remove");
	gotoxy(Width / 2 + 5, Height / 2 - 2);
	printf("E : Select Color to remove");
	gotoxy(Width / 2 + 5, Height / 2);
	printf("S : Decrease The Fall Speed");
	gotoxy(Width / 2 + 5, Height / 2 + 2);
	printf("W : Select Next Block");


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

void SelectTheme() {
	system("cls");
	gotoxy(Width / 2 - 9, Height / 2 - 12);
	printf("테마를 선택하세요");
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			switch (Themespace[i][j]) {
			case 1:
				gotoxy(j * 2 + 6, i + 10);
				if (isThemeClear[0] == false) printf("▣");
				else printf(FG_COLOR(255, 0, 0) "▣" RESET);

				gotoxy(j * 2 + 35, i + 10);
				if (isThemeClear[1] == false) printf("▣");
				else printf(FG_COLOR(255, 255, 0) "▣" RESET);

				gotoxy(j * 2 + 64, i + 10);
				if (isThemeClear[2] == false) printf("▣");
				else printf(FG_COLOR(0, 102, 255) "▣" RESET);
				break;
			case 2:
				gotoxy(j * 2 + 6, i + 10);
				if (isThemeClear[0] == false) printf("■");
				else printf(FG_COLOR(255, 0, 0) "■" RESET);

				gotoxy(j * 2 + 35, i + 10);
				if (isThemeClear[1] == false) printf("■");
				else printf(FG_COLOR(255, 255, 0) "■" RESET);

				gotoxy(j * 2 + 64, i + 10);
				if (isThemeClear[2] == false) printf("■");
				else printf(FG_COLOR(0, 102, 255) "■" RESET);
				break;
			}
		}
	}
	gotoxy(13, 8);
	printf("튀김류");
	gotoxy(43, 8);
	printf("밥류");
	gotoxy(69, 8);
	printf("디저트류");

	int return_n = 0;
	gotoxy(15, 22);
	printf("▲");
	gotoxy(14, 23);
	printf("▲▲");

	while (1) { // 키보드 움직임
		int key;
		if (kbhit()) {
			key = getch();
			if (key == 224 || key == 0) {
				key = getch();
				switch (key) {
				case 75: // 왼쪽
					gotoxy(15 + return_n, 22);
					printf("  ");
					gotoxy(14 + return_n, 23);
					printf("    ");
					return_n -= 29;
					if (return_n < 0) return_n = 0;
					gotoxy(15 + return_n, 22);
					printf("▲");
					gotoxy(14 + return_n, 23);
					printf("▲▲");
					break;
				case 77: // 오른쪽
					gotoxy(15 + return_n, 22);
					printf("  ");
					gotoxy(14 + return_n, 23);
					printf("    ");
					return_n += 29;
					if (return_n > 58) return_n = 58;
					gotoxy(15 + return_n, 22);
					printf("▲");
					gotoxy(14 + return_n, 23);
					printf("▲▲");
					break;
				default:
					break;
				}
			}
			else {
				if (key == 13) // 엔터
				{
					if (return_n == 0) // 튀김류
						Theme1();
					else if (return_n == 29)
						Theme2();
					else
						Theme3();
				}
			}
		}
	}
}

void Theme1() {
	themenum = 1;
	system("cls");
	MenuOne();
}

void Theme2() {
	themenum = 2;
	system("cls");
	MenuOne();
}

void Theme3() {
	themenum = 3;
	system("cls");
	MenuOne();
}

void DeleteArea(int x1, int y1, int x2, int y2)
{
	for (int i = x1; i <= x2; i += 2) {
		for (int j = y1; j <= y2; j++) {
			gotoxy(i, j);
			printf("  ");
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
		DrawGauge();
		ShowBlockArrivePosition();
		DrawBlock();
		DropBlock();
		BlockToGround();
		RemoveLine();
		InputKey();

		endItemT = clock();
		if ((float)(endItemT - startItemT >= 10000)) {
			isSlowItem = false;
		}
		
		CheckClear();
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

	if (blockCnt <= 6) {
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
			UIspace[6][2] = 6;
			UIspace[6][3] = 6;
			break;
		case 5: // L자블럭
			UIspace[4][3] = 7;
			UIspace[5][3] = 7;
			UIspace[6][3] = 7;
			UIspace[6][4] = 7;
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
	gotoxy(10, 1);
	printf(FG_COLOR(255, 0, 0) "요 " RESET);
	printf(FG_COLOR(255, 127, 0) "리 " RESET);
	printf(FG_COLOR(255, 255, 0) "왕 " RESET);
	printf(FG_COLOR(0, 255, 0) "비 " RESET);
	printf(FG_COLOR(0, 255, 255) "룡 " RESET);
	printf(FG_COLOR(148, 0, 211) "★" RESET);
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
				printf(FG_COLOR(255, 255, 255) "■" RESET); // 하얀색, I자 블럭
				break;
			case 6: // blockForm = 4
				gotoxy(j * 2 + 6, i + 6);
				printf(FG_COLOR(150, 75, 0) "■" RESET); // 갈색, L자반대블럭
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
	gotoxy(34, 9);
	printf("NEXT BLOCK");
	gotoxy(35, 20);
	printf("H O L D");

	gotoxy(33, 1);
	printf("ESC : Option");

	if (isSlowItem == false) {
		if (stagenum == 1) downspeed = 800;
		else if (stagenum == 2) downspeed = 650;
		else if (stagenum == 3) downspeed = 500;
		else downspeed = 350;
	}
	else {
		gotoxy(81, 22);
		printf(FG_COLOR(255, 0, 0) "사용중" RESET);
	}

	SET_FG_COLOR(0, 255, 255);
	if (themenum == 1) {
		gotoxy(35, 4);
		printf("튀김류");
	}
	else if (themenum == 2) {
		gotoxy(36, 4);
		printf("밥류");
	}
	else {
		gotoxy(34, 4);
		printf("디저트류");
	}

	SET_FG_COLOR(255, 51, 51);
	gotoxy(35, 6);
	printf("STAGE %d" RESET, stagenum);

	if (Number_Line == 1) {
		gotoxy(50, 18);
		printf(FG_COLOR(255, 255, 0) "D " RESET);
		printf("라인 파괴 아이템: %d 개", Number_Line);
	}
	else {
		gotoxy(50, 18);
		printf(FG_COLOR(82, 69, 16) "D " RESET);
		SET_FG_COLOR(98, 98, 98);
		printf("라인 파괴 아이템: %d 개", Number_Line);
	}

	if (Number_Color == 1) {
		gotoxy(50, 20);
		printf(FG_COLOR(255, 255, 0) "E " RESET);
		printf("색 파괴 아이템: %d 개", Number_Color);
	}
	else {
		gotoxy(50, 20);
		printf(FG_COLOR(82, 69, 16) "E " RESET);
		SET_FG_COLOR(98, 98, 98);
		printf("색 파괴 아이템: %d 개", Number_Color);
	}

	if (Number_Speed == 1) {
		gotoxy(50, 22);
		printf(FG_COLOR(255, 255, 0) "S " RESET);
		printf("블록 속도 감소 아이템: %d 개", Number_Speed);
	}
	else {
		gotoxy(50, 22);
		printf(FG_COLOR(82, 69, 16) "S " RESET);
		SET_FG_COLOR(98, 98, 98);
		printf("블록 속도 감소 아이템: %d 개", Number_Speed);
	}

	if (Number_Block == 1) {
		gotoxy(50, 24);
		printf(FG_COLOR(255, 255, 0) "W " RESET);
		printf("블록 선택 아이템: %d 개", Number_Block);
	}
	else {
		gotoxy(50, 24);
		printf(FG_COLOR(82, 69, 16) "W " RESET);
		SET_FG_COLOR(98, 98, 98);
		printf("블록 선택 아이템: %d 개", Number_Block);
	}


	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 20; j++) {
			switch (UIspace[i][j]) {
			case 1:
				gotoxy(j * 2 + 32, i + 8);
				printf("▣");
				break;
			case 2: // blockForm = 0
				gotoxy(j * 2 + 32, i + 8);
				printf(FG_COLOR(255, 0, 255) "■" RESET); // 보라색, T자블럭
				break;
			case 3: // blockForm = 1
				gotoxy(j * 2 + 32, i + 8);
				printf(FG_COLOR(0, 255, 51) "■" RESET); // 초록색, 오른쪽번개블럭
				break;
			case 4: // blockForm = 2
				gotoxy(j * 2 + 32, i + 8);
				printf(FG_COLOR(255, 0, 0) "■" RESET); // 빨간색, 왼쪽번개블럭
				break;
			case 5: // blockForm = 3
				gotoxy(j * 2 + 32, i + 8);
				printf(FG_COLOR(255, 255, 255) "■" RESET); // 하얀색, I자 블럭
				break;
			case 6: // blockForm = 4
				gotoxy(j * 2 + 32, i + 8);
				printf(FG_COLOR(150, 75, 0) "■" RESET); // 갈색, L자반대블럭
				break;
			case 7: // blockForm = 5
				gotoxy(j * 2 + 32, i + 8);
				printf(FG_COLOR(255, 127, 0) "■" RESET); // 주황색, L자블럭
				break;
			case 8: // blockForm = 6
				gotoxy(j * 2 + 32, i + 8);
				printf(FG_COLOR(255, 255, 0) "■" RESET); // 노랑색, ㅁ자블럭
				break;
			case 9:
				gotoxy(j * 2 + 32, i + 8);
				printf("?");
				break;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
	}
}

void DrawGauge() {
	if (themenum == 1) {
		if (stagenum == 1) {
			isColor[3][0] = true;
			isColor[6][0] = true;
			isColor[5][0] = true;
			if (isColor[3][1] == true && isColor[6][1] == true && isColor[5][1] == true) {
				isStageClear = true;
			}
		}
		else if (stagenum == 2) {
			isColor[5][0] = true;
			isColor[3][0] = true;
			isColor[6][0] = true;
			isColor[2][0] = true;
		}
		else if (stagenum == 3) {
			isColor[5][0] = true;
			isColor[0][0] = true;
			isColor[1][0] = true;
			isColor[6][0] = true;
			isColor[2][0] = true;
		}
		else {
			isColor[5][0] = true;
			isColor[1][0] = true;
			isColor[0][0] = true;
			isColor[6][0] = true;
			isColor[2][0] = true;
			isColor[3][0] = true;
		}
	}
	else if (themenum == 2) {
		if (stagenum == 1) {
			isColor[3][0] = true;
			isColor[1][0] = true;
			isColor[5][0] = true;
		}
		else if (stagenum == 2) {
			isColor[5][0] = true;
			isColor[3][0] = true;
			isColor[6][0] = true;
			isColor[2][0] = true;
		}
		else if (stagenum == 3) {
			isColor[3][0] = true;
			isColor[0][0] = true;
			isColor[5][0] = true;
			isColor[2][0] = true;
			isColor[4][0] = true;
		}
		else {
			isColor[5][0] = true;
			isColor[1][0] = true;
			isColor[0][0] = true;
			isColor[6][0] = true;
			isColor[4][0] = true;
			isColor[2][0] = true;
		}
	}
	else {
		if (stagenum == 1) {
			isColor[3][0] = true;
			isColor[4][0] = true;
			isColor[2][0] = true;
		}
		else if (stagenum == 2) {
			isColor[5][0] = true;
			isColor[3][0] = true;
			isColor[6][0] = true;
			isColor[4][0] = true;
		}
		else if (stagenum == 3) {
			isColor[5][0] = true;
			isColor[6][0] = true;
			isColor[2][0] = true;
			isColor[1][0] = true;
			isColor[3][0] = true;
		}
		else {
			isColor[6][0] = true;
			isColor[3][0] = true;
			isColor[2][0] = true;
			isColor[5][0] = true;
			isColor[1][0] = true;
			isColor[4][0] = true;
		}
	}

	for (int j = 0; j < 7; j++) {
		if (j == 0 && isColor[j][0] == true) {
			gotoxy(50, (j + 1) * 2);
			printf("보라색");
			for (int i = 0; i < colorGauge[j] / 3; i++)
				printf(FG_COLOR(255, 0, 255) "■" RESET);
		}
		else if (j == 1 && isColor[j][0] == true) {
			gotoxy(50, (j + 1) * 2);
			printf("초록색");
			for (int i = 0; i < colorGauge[j] / 3; i++)
				printf(FG_COLOR(0, 255, 51) "■" RESET);
		}
		else if (j == 2 && isColor[j][0] == true) {
			gotoxy(50, (j + 1) * 2);
			printf("빨강색");
			for (int i = 0; i < colorGauge[j] / 3; i++)
				printf(FG_COLOR(255, 0, 0) "■" RESET);
		}
		else if (j == 3 && isColor[j][0] == true) {
			gotoxy(50, (j + 1) * 2);
			printf("하얀색");
			for (int i = 0; i < colorGauge[j] / 3; i++)
				printf(FG_COLOR(255, 255, 255) "■" RESET);
		}
		else if (j == 4 && isColor[j][0] == true) {
			gotoxy(52, (j + 1) * 2);
			printf("갈색");
			for (int i = 0; i < colorGauge[j] / 3; i++)
				printf(FG_COLOR(150, 75, 0) "■" RESET);
		}
		else if (j == 5 && isColor[j][0] == true) {
			gotoxy(50, (j + 1) * 2);
			printf("주황색");
			for (int i = 0; i < colorGauge[j] / 3; i++)
				printf(FG_COLOR(255, 127, 0) "■" RESET);
		}
		else if (j == 6 && isColor[j][0] == true) {
			gotoxy(50, (j + 1) * 2);
			printf("노랑색");
			for (int i = 0; i < colorGauge[j] / 3; i++)
				printf(FG_COLOR(255, 255, 0) "■" RESET);
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
		SET_FG_COLOR(255, 255, 255); // 하얀색, I자 블럭
		break;
	case 4:
		SET_FG_COLOR(150, 75, 0); // 갈색, L자반대블럭
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
	if ((float)(endT - startDropT) >= downspeed) {
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
			y = 0;
			CreateRandomForm();
			CheckEnding();
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
			Gauge(i);
			for (int j = i; j > 1; j--) {
				for (int k = 1; k < 11; k++) {
					space[j][k] = space[j - 1][k];
				}
			}
		}
	}
}

void RemoveSelectedLine() {
	DrawMap();
	DrawUI();
	DrawBlock();
	ShowBlockArrivePosition();

	gotoxy(boardWidth, boardHeight - 1); // boardWidth = 30, boardHeight = 27;
	printf("◀");
	
	gotoxy(81, 18);
	printf(FG_COLOR(255, 0, 0) "사용중" RESET);

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
					if (isFirst == true) {
						gotoxy(30, 26);
						printf("  ");
						return_n -= 2;
						gotoxy(30, 25);
						printf("◀");
						isFirst = false;
					}
					else {
						gotoxy(boardWidth, boardHeight + return_n); //원래 자리로 이동
						printf("  "); //삭제
						return_n -= 1; //화살표의 좌표를 위로 3변경시키고
						if (return_n < -(boardHeight - 8)) return_n = -(boardHeight - 8); //범위밖으로 나가지 못하게
						gotoxy(boardWidth, boardHeight + return_n);;
						printf("◀"); //바뀐 좌표에 방향키 출력
					}
					break;
				case 80: //아래쪽 방향키를 누른 경우
					gotoxy(boardWidth, boardHeight + return_n);; //원래 자리로 이동
					printf("  "); //삭제
					return_n += 1; //화살표의 좌표를 아래로 3변경시키고
					if (return_n > -1) return_n = -1; //범위밖으로 나가지 못하게
					gotoxy(boardWidth, boardHeight + return_n);
					printf("◀"); //바뀐 좌표에 방향키 출력
					break;
				default:
					break;
				}
			}

			else
				if (key == 13) //엔터키를 눌렀을 때
				{
					Number_Line = 0;
					if (return_n == 0) {
						int i = 20;
						Gauge(i);
						for (int j = i; j > 1; j--) {
							for (int k = 1; k < 11; k++) {
								space[j][k] = space[j - 1][k];
							}
						}
					}
					if (return_n < 0) {
						int i = 21 + return_n; //return_n -1=첫줄 -19=막줄
						Gauge(i);
						for (int j = i; j > 1; j--) {
							for (int k = 1; k < 11; k++) {
								space[j][k] = space[j - 1][k];
							}
						}
					}
					isFirst = true;
					break;
				}
		}
	}
}

void RemoveSelectedColor() {
	DrawMap();
	DrawUI();
	DrawBlock();
	ShowBlockArrivePosition();

	gotoxy(81, 20);
	printf(FG_COLOR(255, 0, 0) "사용중" RESET);

	gotoxy(boardWidth / 3, (boardHeight / 2) - 3); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) - 2); printf("▤                                        ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) - 1); printf("▤    ★지우고 싶은 색을 선택하세요★     ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 0); printf("▤                                        ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 1); printf("▤                                        ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 2); printf("▤                                        ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 3); printf("▤                                        ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 4); printf("▤                                        ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 5); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

	/*블럭 색상표*/
	gotoxy((boardWidth / 2) + 9, (boardHeight / 2) + 1);
	printf("←");
	gotoxy((boardWidth / 2) + 13, (boardHeight / 2) + 1);
	printf(FG_COLOR(255, 0, 255) "■   " RESET); // 보라색, T자블럭
	gotoxy((boardWidth / 2) + 17, (boardHeight / 2) + 1);
	printf(FG_COLOR(0, 255, 51) "■   " RESET); // 초록색, 오른쪽번개블럭
	gotoxy((boardWidth / 2) + 21, (boardHeight / 2) + 1);
	printf(FG_COLOR(255, 0, 0) "■   " RESET); // 빨간색, 왼쪽번개블럭
	gotoxy((boardWidth / 2) + 9, (boardHeight / 2) + 3);
	printf(FG_COLOR(255, 255, 255) "■   " RESET); // 하얀색, I자 블럭
	gotoxy((boardWidth / 2) + 13, (boardHeight / 2) + 3);
	printf(FG_COLOR(150, 75, 0) "■   " RESET); // 갈색, L자반대블럭
	gotoxy((boardWidth / 2) + 17, (boardHeight / 2) + 3);
	printf(FG_COLOR(255, 127, 0) "■   " RESET); // 주황색, L자블럭
	gotoxy((boardWidth / 2) + 21, (boardHeight / 2) + 3);
	printf(FG_COLOR(255, 255, 0) "■   " RESET); // 노랑색, ㅁ자블럭


	gotoxy((boardWidth / 2) + 9, (boardHeight / 2) + 2);
	printf("▲");

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
				case 77: // 오른쪽 방향키
					if (return_n == 0 || return_n == 4 || return_n == 8 || return_n == 12) {
						gotoxy((boardWidth / 2) + 9 + return_n, (boardHeight / 2) + 2); //원래 자리로 이동
						printf("  "); //삭제
						return_n += 4; //화살표의 좌표를 오른쪽으로 4변경시키고
						if (return_n > 12) return_n = 12; //범위밖으로 나가지 못하게
						gotoxy((boardWidth / 2) + 9 + return_n, (boardHeight / 2) + 2);
						printf("▲"); //바뀐 좌표에 방향키 출력
						break;
					}
					if (return_n == 1 || return_n == 5 || return_n == 9 || return_n == 13) {
						gotoxy((boardWidth / 2) + 8 + return_n, (boardHeight / 2) + 4); //원래 자리로 이동
						printf("  "); //삭제
						return_n += 4; //화살표의 좌표를 오른쪽으로 4변경시키고
						if (return_n > 13) return_n = 13; //범위밖으로 나가지 못하게
						gotoxy((boardWidth / 2) + 8 + return_n, (boardHeight / 2) + 4);
						printf("▲"); //바뀐 좌표에 방향키 출력
						break;
					}

				case 75: //왼쪽 방향키를 누른 경우
					if (return_n == 0 || return_n == 4 || return_n == 8 || return_n == 12) {
						gotoxy((boardWidth / 2) + 9 + return_n, (boardHeight / 2) + 2);
						printf("  "); //삭제
						return_n -= 4; //화살표의 좌표를 오른쪽으로 4변경시키고
						if (return_n < 0) return_n = 0; //범위밖으로 나가지 못하게
						gotoxy((boardWidth / 2) + 9 + return_n, (boardHeight / 2) + 2);
						printf("▲"); //바뀐 좌표에 방향키 출력
						break;
					}
					if (return_n == 1 || return_n == 5 || return_n == 9 || return_n == 13) {
						gotoxy((boardWidth / 2) + 8 + return_n, (boardHeight / 2) + 4); //원래 자리로 이동
						printf("  "); //삭제
						return_n -= 4; //화살표의 좌표를 오른쪽으로 4변경시키고
						if (return_n < 1) return_n = 1; //범위밖으로 나가지 못하게
						gotoxy((boardWidth / 2) + 8 + return_n, (boardHeight / 2) + 4);
						printf("▲"); //바뀐 좌표에 방향키 출력
						break;
					}
				case 80: //아래 방향키를 누른 경우
					if (return_n == 0 || return_n == 4 || return_n == 8 || return_n == 12) {
						gotoxy((boardWidth / 2) + 9 + return_n, (boardHeight / 2) + 2);
						printf("  "); //삭제
						return_n = 1;
						gotoxy((boardWidth / 2) + 9, (boardHeight / 2) + 4);
						printf("▲"); //바뀐 좌표에 방향키 출력
						break;
					}
				case 72: //위 방향키를 누른 경우
					if (return_n == 1 || return_n == 5 || return_n == 9 || return_n == 13) {
						gotoxy((boardWidth / 2) + 8 + return_n, (boardHeight / 2) + 4); //원래 자리로 이동
						printf("  "); //삭제
						return_n = 0;
						gotoxy((boardWidth / 2) + 9, (boardHeight / 2) + 2);
						printf("▲"); //바뀐 좌표에 방향키 출력
						break;
					}
				default:
					break;
				}
			}
			else {
				if (key == 13) {
					isEnter = true;
				}
			}
			
			if (isEnter == true) {
				Number_Color = 0;
				switch (return_n) {
				case 4:
					for (int i = 20; i > 0; i--) {
						for (int j = 1; j < 11; j++) {
							while (space[i][j] == 2) {
								colorGauge[0]++;
								for (int k = i; k > 1; k--) {
									space[k][j] = space[k - 1][j];
								}
							}
						}
					}
					break;
				case 8:
					for (int i = 20; i > 0; i--) {
						for (int j = 1; j < 11; j++) {
							while (space[i][j] == 3) {
								colorGauge[1]++;
								for (int k = i; k > 1; k--) {
									space[k][j] = space[k - 1][j];
								}

							}
						}
					}
					break;
				case 12:
					for (int i = 20; i > 0; i--) {
						for (int j = 1; j < 11; j++) {
							while (space[i][j] == 4) {
								colorGauge[2]++;
								for (int k = i; k > 1; k--) {
									space[k][j] = space[k - 1][j];
								}

							}
						}
					}
					break;
				case 1:
					for (int i = 20; i > 0; i--) {
						for (int j = 1; j < 11; j++) {
							while (space[i][j] == 5) {
								colorGauge[3]++;
								for (int k = i; k > 1; k--) {
									space[k][j] = space[k - 1][j];
								}

							}
						}
					}
					break;
				case 5:
					for (int i = 20; i > 0; i--) {
						for (int j = 1; j < 11; j++) {
							while (space[i][j] == 6) {
								colorGauge[4]++;
								for (int k = i; k > 1; k--) {
									space[k][j] = space[k - 1][j];
								}

							}
						}
					}
					break;
				case 9:
					for (int i = 20; i > 0; i--) {
						for (int j = 1; j < 11; j++) {
							while (space[i][j] == 7) {
								colorGauge[5]++;
								for (int k = i; k > 1; k--) {
									space[k][j] = space[k - 1][j];
								}

							}
						}
					}
					break;
				case 13:
					for (int i = 20; i > 0; i--) {
						for (int j = 1; j < 11; j++) {
							while (space[i][j] == 8) {
								colorGauge[6]++;
								for (int k = i; k > 1; k--) {
									space[k][j] = space[k - 1][j];
								}

							}
						}
					}
					break;
				default:
					Number_Color = 1;
				}
				isEnter = false;
				break;
			}
		}
	}
}

void Gauge(int line) // 경험치 함수 (칸 최대 16개)
{
	for (int j = 0; j < 10; j++) {
		int color = space[line][j + 1] - 2;
		if (colorGauge[color] >= 36) {
			isColor[color][1] = true;
			continue;
		}
		colorGauge[color]++;
	}
}

void CheckEnding() {
	for (int i = 1; i < 11; i++) {
		if (space[1][i] == 2 || space[1][i] == 3 || space[1][i] == 4 || space[1][i] == 5 || space[1][i] == 6 || space[1][i] == 7 || space[1][i] == 8) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			gotoxy(Width / 2 - 22, Height / 2 - 7);
			printf("                                         ");
			gotoxy(Width / 2 - 22, Height / 2 - 6);
			printf("   ======================================   \n");
			gotoxy(Width / 2 - 23, Height / 2 - 5);
			printf("   ||                                    ||   \n");
			gotoxy(Width / 2 - 23, Height / 2 - 4);
			printf("   ||                                    ||   \n");
			gotoxy(Width / 2 - 23, Height / 2 - 3);
			printf("   ||                                    ||   \n");
			gotoxy(Width / 2 - 23, Height / 2 - 2);
			printf("   ||                                    ||   \n");
			gotoxy(Width / 2 - 23, Height / 2 - 1);
			printf("   ||                                    ||   \n");
			gotoxy(Width / 2 - 23, Height / 2);
			printf("   ||                                    ||   \n");
			gotoxy(Width / 2 - 23, Height / 2 + 1);
			printf("   ||                                    ||   \n");
			gotoxy(Width / 2 - 23, Height / 2 + 2);
			printf("   ||                                    ||   \n");
			gotoxy(Width / 2 - 22, Height / 2 + 3);
			printf("   ======================================   \n");
			gotoxy(Width / 2 - 22, Height / 2 + 4);
			printf("                                         ");
			gotoxy(Width / 2 - 6, Height / 2 - 2);
			printf("게 임 종 료");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			Sleep(10000);
			exit(0);
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
		case 68: // D
		case 100: // d
			if (Number_Line == 1)
				RemoveSelectedLine();
			break;
		case 69: // E
		case 101: // e
			if (Number_Color == 1)
				RemoveSelectedColor();
			break;
		case 83: // S
		case 115: // s
			if (Number_Speed == 1)
				SlowFallSpeed();
			break;
		case 87: // W
		case 119: // w
			if (Number_Block == 1)
				SelectBlock();
			break;
		case 65:
		case 97:
			isStageClear = true;
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
			if (block[blockForm][blockRotation][i][j] == 1) {
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
					printf(FG_COLOR(255, 255, 255) "□" RESET); // 하얀색, I자 블럭
					break;
				case 4:
					printf(FG_COLOR(150, 75, 0) "□" RESET); // 갈색, L자반대블럭
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
			UIspace[17][2] = 6;
			UIspace[17][3] = 6;
			break;
		case 5: // L자블럭
			UIspace[15][3] = 7;
			UIspace[16][3] = 7;
			UIspace[17][3] = 7;
			UIspace[17][4] = 7;
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

void SlowFallSpeed() {
	Number_Speed = 0;
	isSlowItem = true;
	startItemT = clock();
	downspeed = 1300;
}

void SelectBlock() {
	DrawMap();
	DrawUI();
	DrawBlock();
	ShowBlockArrivePosition();

	gotoxy(81, 24);
	printf(FG_COLOR(255, 0, 0) "사용중" RESET);

	gotoxy(boardWidth / 3, (boardHeight / 2) - 3); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) - 2); printf("▤                                            ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) - 1); printf("▤       ★원하는 블록을 선택해주세요★       ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 0); printf("▤                                            ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 1); printf("▤                                            ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 2); printf("▤                                            ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 3); printf("▤                                            ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 4); printf("▤                                            ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 5); printf("▤                                            ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 6); printf("▤                                            ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 7); printf("▤                                            ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 8); printf("▤                                            ▤");
	gotoxy(boardWidth / 3, (boardHeight / 2) + 9); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");


	/*블럭 선택표*/
	gotoxy((boardWidth / 2) + 4, (boardHeight / 2) + 1);
	printf("←");

	// 보라색, T자블럭
	gotoxy((boardWidth / 2) + 10, (boardHeight / 2) + 1);
	printf(FG_COLOR(255, 0, 255) "■■■" RESET);
	gotoxy((boardWidth / 2) + 10, (boardHeight / 2) + 2);
	printf(FG_COLOR(255, 0, 255) "  ■  " RESET);

	// 초록색, 오른쪽번개블럭
	gotoxy((boardWidth / 2) + 18, (boardHeight / 2) + 1);
	printf(FG_COLOR(0, 255, 51) "  ■■" RESET);
	gotoxy((boardWidth / 2) + 18, (boardHeight / 2) + 2);
	printf(FG_COLOR(0, 255, 51) "■■" RESET);

	// 빨간색, 왼쪽번개블럭
	gotoxy((boardWidth / 2) + 26, (boardHeight / 2) + 1);
	printf(FG_COLOR(255, 0, 0) "■■" RESET);
	gotoxy((boardWidth / 2) + 26, (boardHeight / 2) + 2);
	printf(FG_COLOR(255, 0, 0) "  ■■" RESET);

	// 하얀색, I자 블럭
	gotoxy((boardWidth / 2) + 2, (boardHeight / 2) + 5);
	printf(FG_COLOR(255, 255, 255) "■■■■" RESET);

	// 갈색, L자반대블럭
	gotoxy((boardWidth / 2) + 12, (boardHeight / 2) + 5);
	printf(FG_COLOR(150, 75, 0) "■" RESET);
	gotoxy((boardWidth / 2) + 12, (boardHeight / 2) + 6);
	printf(FG_COLOR(150, 75, 0) "■■■" RESET);

	// 주황색, L자블럭
	gotoxy((boardWidth / 2) + 20, (boardHeight / 2) + 5);
	printf(FG_COLOR(255, 127, 0) "    ■" RESET);
	gotoxy((boardWidth / 2) + 20, (boardHeight / 2) + 6);
	printf(FG_COLOR(255, 127, 0) "■■■" RESET);

	// 노랑색, ㅁ자블럭
	gotoxy((boardWidth / 2) + 28, (boardHeight / 2) + 5);
	printf(FG_COLOR(255, 255, 0) "■■" RESET);
	gotoxy((boardWidth / 2) + 28, (boardHeight / 2) + 6);
	printf(FG_COLOR(255, 255, 0) "■■" RESET);

	gotoxy((boardWidth / 2) + 4, (boardHeight / 2) + 3);
	printf("▲");

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
				case 77: // 오른쪽 방향키
					if (return_n == 0 || return_n == 8 || return_n == 16 || return_n == 24) {
						gotoxy((boardWidth / 2) + 4 + return_n, (boardHeight / 2) + 3); //원래 자리로 이동
						printf("  "); //삭제
						return_n += 8; //화살표의 좌표를 오른쪽으로 4변경시키고
						if (return_n > 24) return_n = 24; //범위밖으로 나가지 못하게
						gotoxy((boardWidth / 2) + 4 + return_n, (boardHeight / 2) + 3);
						printf("▲"); //바뀐 좌표에 방향키 출력
						break;
					}
					if (return_n == 1 || return_n == 9 || return_n == 17 || return_n == 25) {
						gotoxy((boardWidth / 2) + 4 + return_n, (boardHeight / 2) + 7); //원래 자리로 이동
						printf("  "); //삭제
						return_n += 8; //화살표의 좌표를 오른쪽으로 4변경시키고
						if (return_n > 25) return_n = 25; //범위밖으로 나가지 못하게
						gotoxy((boardWidth / 2) + 4 + return_n, (boardHeight / 2) + 7);
						printf("▲"); //바뀐 좌표에 방향키 출력
						break;
					}

				case 75: //왼쪽 방향키를 누른 경우
					if (return_n == 0 || return_n == 8 || return_n == 16 || return_n == 24) {
						gotoxy((boardWidth / 2) + 4 + return_n, (boardHeight / 2) + 3); //원래 자리로 이동
						printf("  "); //삭제
						return_n -= 8; //화살표의 좌표를 오른쪽으로 4변경시키고
						if (return_n < 0) return_n = 0; //범위밖으로 나가지 못하게
						gotoxy((boardWidth / 2) + 4 + return_n, (boardHeight / 2) + 3);
						printf("▲"); //바뀐 좌표에 방향키 출력
						break;
					}
					if (return_n == 1 || return_n == 9 || return_n == 17 || return_n == 25) {
						gotoxy((boardWidth / 2) + 4 + return_n, (boardHeight / 2) + 7); //원래 자리로 이동
						printf("  "); //삭제
						return_n -= 8; //화살표의 좌표를 오른쪽으로 4변경시키고
						if (return_n < 1) return_n = 1; //범위밖으로 나가지 못하게
						gotoxy((boardWidth / 2) + 4 + return_n, (boardHeight / 2) + 7);
						printf("▲"); //바뀐 좌표에 방향키 출력
						break;
					}
				case 80: //아래 방향키를 누른 경우
					if (return_n == 0 || return_n == 8 || return_n == 16 || return_n == 24) {
						gotoxy((boardWidth / 2) + 4 + return_n, (boardHeight / 2) + 3); //원래 자리로 이동
						printf("  "); //삭제
						return_n = 1; //화살표의 좌표를 오른쪽으로 4변경시키고
						gotoxy((boardWidth / 2) + 5, (boardHeight / 2) + 7);
						printf("▲"); //바뀐 좌표에 방향키 출력
						break;
					}
					else
						break;

				case 72: //위 방향키를 누른 경우
					if (return_n == 1 || return_n == 9 || return_n == 17 || return_n == 25) {
						gotoxy((boardWidth / 2) + 4 + return_n, (boardHeight / 2) + 7); //원래 자리로 이동
						printf("  "); //삭제
						return_n = 0; //화살표의 좌표를 오른쪽으로 4변경시키고
						gotoxy((boardWidth / 2) + 4, (boardHeight / 2) + 3);
						printf("▲"); //바뀐 좌표에 방향키 출력
						break;
					}
					else
						break;
				default:
					break;
				}
			}
			else {
				if (key == 13) {
					isBlock = true;
				}
				break;
			}

			if (isBlock == true) {
				Number_Block = 0;
				x = 8, y = 0;
				switch (return_n) {
				case 8:
					blockForm = 0;
					break;
				case 16:
					blockForm = 1;
					break;
				case 24:
					blockForm = 2;
					break;
				case 1:
					blockForm = 3;
					break;
				case 9:
					blockForm = 4;
					break;
				case 17:
					blockForm = 5;
					break;
				case 25:
					blockForm = 6;
					break;
				}
			}
		}
	}
}

void CheckClear() {
	if (isStageClear == true) {
		system("cls");
		if (themenum == 1) {
			if (stagenum == 1) {
				Height = 35;
				Console_Size();
				gotoxy(Width / 2 - 12, 2);
				printf(FG_COLOR(255, 255, 0) "< 도너츠 >  " RESET);
				printf("조리법 획득 !!");
				doughnut(20, 5);
			}
			else if (stagenum == 2) {
				Height = 37;
				Console_Size();
				gotoxy(Width / 2 - 11, 2);
				printf(FG_COLOR(255, 255, 0) "< 치킨 >  " RESET);
				printf("조리법 획득 !!");
				chicken(8, 5);
			}
			else if (stagenum == 3) {
				Height = 42;
				Console_Size();
				gotoxy(Width / 2 - 12, 2);
				printf(FG_COLOR(255, 255, 0) "< 핫도그 >  " RESET);
				printf("조리법 획득 !!");
				hot_dog(12, 5);
			}
			else {
				Height = 39;
				Console_Size();
				gotoxy(Width / 2 - 12, 2);
				printf(FG_COLOR(255, 255, 0) "< 토스트 >  " RESET);
				printf("조리법 획득 !!");
				toast(16, 5);
			}
		}
		else if (themenum == 2) {
			if (stagenum == 1) {
				Height = 30;
				Console_Size();
				gotoxy(Width / 2 - 13, 2);
				printf(FG_COLOR(255, 255, 0) "< 새우초밥 >  " RESET);
				printf("조리법 획득 !!");
				shrimp_sushi(14, 5);
			}
			else if (stagenum == 2) {
				Height = 41;
				Console_Size();
				gotoxy(Width / 2 - 13, 2);
				printf(FG_COLOR(255, 255, 0) "< 팬케이크 >  " RESET);
				printf("조리법 획득 !!");
				pancake(10, 5);
			}
			else if (stagenum == 3) {
				Height = 30;
				Console_Size();
				gotoxy(Width / 2 - 12, 2);
				printf(FG_COLOR(255, 255, 0) "< 주먹밥 >  " RESET);
				printf("조리법 획득 !!");
				rice_ball(14, 7);
			}
			else {
				Height = 30;
				Console_Size();
				gotoxy(Width / 2 - 12, 2);
				printf(FG_COLOR(255, 255, 0) "< 햄버거 >  " RESET);
				printf("조리법 획득 !!");
				hamburger(28, 5);
			}
		}
		else {
			if (stagenum == 1) {
				Height = 50;
				Console_Size();
				gotoxy(Width / 2 - 11, 2);
				printf(FG_COLOR(255, 255, 0) "< 사탕 >  " RESET);
				printf("조리법 획득 !!");
				candy(24, 5);
			}
			else if (stagenum == 2) {
				Height = 32;
				Console_Size();
				gotoxy(Width / 2 - 14, 2);
				printf(FG_COLOR(255, 255, 0) "< 아이스크림 >  " RESET);
				printf("조리법 획득 !!");
				Ice_cream(33, 5);
			}
			else if (stagenum == 3) {
				Height = 32;
				Console_Size();
				gotoxy(Width / 2 - 12, 2);
				printf(FG_COLOR(255, 255, 0) "< 마카롱 >  " RESET);
				printf("조리법 획득 !!");
				macaroon(20, 5);
			}
			else {
				Height = 60;
				Width = 110;
				Console_Size();
				gotoxy(Width / 2 - 12, 2);
				printf(FG_COLOR(255, 255, 0) "< 케이크 >  " RESET);
				printf("조리법 획득 !!");
				cake(3, 4);
			}
		}

		while (1)
		{
			gotoxy(Width / 2 - 19, Height - 3);
			printf("다음 스테이지로 가려면 아무키나 누르세요!");
			Sleep(500);
			if (kbhit()) break;
			gotoxy(Width / 2 - 19, Height - 3);
			printf("                                             ");
			Sleep(500);
		} //키보드 입력이 들어올 때 까지 글씨가 깜빡거림

		for (int i = 0; i < 22; i++) {
			for (int j = 0; j < 12; j++) {
				if (i == 0 || i == 21) space[i][j] = 1;
				else if (j == 0 || j == 11) space[i][j] = 1;
				else space[i][j] = 0;
			}
		}

		for (int i = 0; i < 22; i++) {
			for (int j = 0; j < 20; j++) {
				if ((i == 3 || i == 8 || i == 14 || i == 19) && j >= 0 && j <= 6) UIspace[i][j] = 1;
				else if ((i >= 4 && i <= 7 && (j == 0 || j == 6)) || (i >= 15 && i <= 18 && (j == 0 || j == 6))) UIspace[i][j] = 1;
				else UIspace[i][j] = 0;
			}
		}

		isStageClear = false;
		Number_Line = 1, Number_Color = 1, Number_Speed = 1, Number_Block = 1;
		blockCnt = 8;
		CreateRandomForm();
		x = 8, y = 0;
		for (int i = 0; i < 7; i++) colorGauge[i] = 0;

		isSpace = false;
		isHold = false;
		isHoldAlready = false;
		isMusic = true;
		isFirst = true;
		isEnter = false;
		isBlock = false;
		isSlowItem = false;
		for (int i = 0; i < 7; i++) {
			isColor[i][0] = false;
			isColor[i][1] = false;
		}

		stagenum++;
		if (stagenum >= 5) {
			isThemeClear[themenum - 1] = true;
			stagenum = 1;
		}

		Width = 90;
		Height = 30;
		Console_Size();
	}
}