#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib") //사운드

// 기본 설정 상수
#define Width 120  // 창 가로 크기
#define Height 40  // 창 세로 크기
#define kbhit _kbhit
#define getch _getch
//↑ 언더바 문제 해결을 위함

// 기본 세팅 함수
void Console_Size(); // 콘솔 사이즈 설정
void CursorView(char show); // 커서 깜빡임 숨기기. 0이면 숨김, 1이면 보임
void gotoxy(int x, int y); //커서 이동 함수

// 메뉴 관련 함수
void DesignMainMenu(); // 메인 메뉴 디자인
int MainMenu(); // 메인 메뉴
void MenuTwo(); // 조작법 메뉴
void MenuThree(); // 제작자 메뉴


int main() {
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
			printf("게임시작");
			Sleep(300);
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
	printf("\n\n\n\n");
	printf("			■■■■■   ■■■   ■■■■■   ■■■■      ■■■■■     ■■■■  \n");
	printf("			    ■       ■           ■       ■      ■        ■        ■         \n");
	printf("			    ■       ■■■       ■       ■■■■          ■         ■■■■  \n");
	printf("			    ■       ■           ■       ■     ■         ■                ■  \n");
	printf("			    ■       ■■■       ■       ■      ■    ■■■■■     ■■■■  \n");
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
	gotoxy(Width / 2 - 3, Height / 2 - 9);
	printf("조작키");
	gotoxy(Width / 2 - 10, Height / 2 - 5);
	printf("←, → : Move Left, Right");
	gotoxy(Width / 2 - 6, Height / 2 - 3);
	printf("↓ : Soft Drop");
	gotoxy(Width / 2 - 9, Height / 2 - 1);
	printf("Space : Hard Drop");
	gotoxy(Width / 2 - 6, Height / 2 + 1);
	printf("↑ : Rotate");
	gotoxy(Width / 2 - 9, Height / 2 + 3);
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
	gotoxy(Width / 2 - 2, Height / 2 - 7);
	printf("제작");
	gotoxy(Width / 2 - 15, Height / 2 - 3);
	printf("중앙대학교 소프트웨어학부 21학번");
	gotoxy(Width / 2 - 7, Height / 2 );
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