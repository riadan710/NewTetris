#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h> 
#include <stdbool.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib") //紫錘球

clock_t startDropT, endT, startGroundT;

int x = 8, y = 0;
RECT blockSize;
int blockForm;
int blockRotation = 0;
int key;

#define Width 90  // 但 亜稽 滴奄
#define Height 30  // 但 室稽 滴奄
#define kbhit _kbhit
#define getch _getch

int block[7][4][4][4] = {  // [7]精 鷺薫姐呪. 沓棋生稽 痕発, 2託据 [4]澗 装姶拝凶原陥 噺穿
	{ // T乞丞 鷺薫
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
	{    // 腰鯵 鷺薫
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
	{   // 腰鯵 鷺薫 鋼企
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
	{   // 1切莫 鷺薫
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
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		}
	},
	{   // L切莫 鷺薫
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
	{   // L切莫 鷺薫 鋼企
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
	{   // 革乞 鷺薫
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

int space[15 + 1][10 + 2] = {  // 室稽 15+1(焼掘混)牒, 亜稽 10+2(丞楕 混)牒  
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

void Console_Size(); // 嬬車 紫戚綜 竺舛
void CursorView(char show); // 朕辞 鉛冊績 需奄奄. 0戚檎 需沿, 1戚檎 左績
void gotoxy(int x, int y); //朕辞 戚疑 敗呪

void DesignMainMenu(); // 五昔 五敢 巨切昔
int MainMenu(); // 五昔 五敢
void MenuTwo(); // 繕拙狛 五敢
void MenuThree(); // 薦拙切 五敢
void MenuOne(); // 惟績獣拙 五敢
void DrawMap(); // 己税 莫殿人 竣昔 鷺薫 益顕
void DrawBlock(); // 薄仙 鷺薫 益軒奄 (4託据壕伸)
void DropBlock(); // 0.8段原陥 鷺薫聖 廃牒梢 購生稽 鎧顕
void BlockToGround(); // 1段疑照 競拭 願焼赤聖凶 疑拙戚 蒸生檎 競生稽 痕敗, 沓棋廃 鷺薫聖 幻級壱 是稽 臣顕
void RemoveLine(); // 鷺薫薦暗 板 廃牒梢 強沿
void InputKey();
void CreateRandomForm(); // 鷺薫戚 鎧形臣凶原陥 沓棋生稽 郊会. 0~6
bool CheckCrash(int x, int y); // 中宜姶走 違帖澗惟 赤生檎 true研 鋼発

int main() {
	CursorView(0);  // 朕辞 鉛冊績 需奄奄. 0戚檎 需沿, 1戚檎 左績
	Console_Size(); // 嬬車 紫戚綜 竺舛
	DesignMainMenu(); // 五昔五敢 巨切昔 窒径
	PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // 壕井製焦 仙持

	while (1) // 惟績 五敢 識澱
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

void gotoxy(int x, int y) //朕辞 戚疑 敗呪
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Console_Size() // 嬬車 紫戚綜 竺舛
{
	char temp[50];
	sprintf(temp, "Mode con cols=%d lines=%d", Width, Height);
	system(temp);
}

void CursorView(char show)  // 朕辞 鉛冊績 需奄奄. 0戚檎 需沿, 1戚檎 左績
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void DesignMainMenu() // 五昔 五敢 巨切昔
{
	printf("\n\n");
	printf("         ＝＝＝＝＝   ＝＝＝   ＝＝＝＝＝   ＝＝＝＝      ＝＝＝＝＝     ＝＝＝＝  \n");
	printf("             ＝       ＝           ＝       ＝      ＝        ＝        ＝         \n");
	printf("             ＝       ＝＝＝       ＝       ＝＝＝＝          ＝         ＝＝＝＝  \n");
	printf("             ＝       ＝           ＝       ＝     ＝         ＝                ＝  \n");
	printf("             ＝       ＝＝＝       ＝       ＝      ＝    ＝＝＝＝＝     ＝＝＝＝  \n");
	printf("\n\n\n");
}

int MainMenu()
{
	system("cls");
	DesignMainMenu();

	gotoxy(Width / 2 - 4, Height / 2);
	printf("惟績 獣拙");
	gotoxy(Width / 2 - 4, Height / 2 + 3);
	printf("繕拙狛");
	gotoxy(Width / 2 - 4, Height / 2 + 6);
	printf("薦拙切");
	gotoxy(Width / 2 - 4, Height / 2 + 9);
	printf("惟績 曽戟");
	gotoxy(Width / 2 + 8, Height / 2);
	printf("��");

	int return_n = 0;
	while (1) // 徹左球 崇送績
	{
		int key;
		if (kbhit()) // 徹左球 脊径戚 級嬢尽聖 井酔
		{
			key = getch(); // 益 徹税 焼什徹坪球葵聖 閤焼 key拭 煽舌
			if (key == 224 || key == 0) // 益 徹亜 号狽徹昔 井酔 拙疑
			{
				key = getch();
				switch (key)
				{
				case 72: // 是楕 号狽徹
					gotoxy(Width / 2 + 8, Height / 2 + return_n); //据掘 切軒稽 戚疑
					printf("  "); //肢薦
					return_n -= 3; //鉢詞妊税 疎妊研 是稽 3痕井獣徹壱
					if (return_n < 0) return_n = 0; //骨是鉱生稽 蟹亜走 公馬惟
					gotoxy(Width / 2 + 8, Height / 2 + return_n);
					printf("��"); //郊駕 疎妊拭 号狽徹 窒径
					break;
				case 80: //焼掘楕 号狽徹研 刊献 井酔
					gotoxy(Width / 2 + 8, Height / 2 + return_n); //据掘 切軒稽 戚疑
					printf("  "); //肢薦
					return_n += 3; //鉢詞妊税 疎妊研 焼掘稽 3痕井獣徹壱
					if (return_n > 6) return_n = 9; //骨是鉱生稽 蟹亜走 公馬惟
					gotoxy(Width / 2 + 8, Height / 2 + return_n);
					printf("��"); //郊駕 疎妊拭 号狽徹 窒径
					break;
				default:
					break;
				}
			}
			else
				if (key == 13) //殖斗徹研 喚袈聖 凶
					return return_n; //鉢詞妊税 疎妊葵聖 鋼発
		}
	}
}

void MenuTwo() // 繕拙狛 五敢
{
	system("cls");
	DesignMainMenu();
	gotoxy(Width / 2 - 3, Height / 2 - 6);
	printf("繕拙徹");
	gotoxy(Width / 2 - 10, Height / 2 - 4);
	printf("∞, ≧ : Move Left, Right");
	gotoxy(Width / 2 - 6, Height / 2 - 2);
	printf("♂ : Soft Drop");
	gotoxy(Width / 2 - 9, Height / 2);
	printf("Space : Hard Drop");
	gotoxy(Width / 2 - 6, Height / 2 + 2);
	printf("∴ : Rotate");
	gotoxy(Width / 2 - 9, Height / 2 + 4);
	printf("Shift : Hold");

	gotoxy(Width / 2 - 8, Height / 2 + 7);
	printf("曽戟馬獣畏柔艦猿?");
	gotoxy(Width / 2 - 6, Height / 2 + 9);
	printf("森");
	gotoxy(Width / 2 + 1, Height / 2 + 9);
	printf("焼艦神");
	gotoxy(Width / 2 - 3, Height / 2 + 9);
	printf("��");

	int return_n = 0;
	while (1) //徹左球 崇送績
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
				case 75: //疎 奄沙
					gotoxy(Width / 2 - 3 + return_n, Height / 2 + 9);
					printf("  ");
					return_n = 0;
					gotoxy(Width / 2 - 3, Height / 2 + 9);
					printf("��");
					break;
				case 77: //酔 11牒蒋
					return_n = 11;
					gotoxy(Width / 2 + 8 - return_n, Height / 2 + 9);
					printf("  ");
					gotoxy(Width / 2 + 8, Height / 2 + 9);
					printf("��");
					break;
				default:
					break;
				}
			}
			else
				if (key == 13)//殖斗
				{
					if (return_n == 0)
						exit(1);
					else
						break;
				}
		}
	}
}

void MenuThree() // 薦拙切 五敢
{
	system("cls");
	DesignMainMenu();
	gotoxy(Width / 2 - 2, Height / 2 - 6);
	printf("薦拙");
	gotoxy(Width / 2 - 15, Height / 2 - 3);
	printf("掻肖企俳嘘 社覗闘裾嬢俳採 21俳腰");
	gotoxy(Width / 2 - 7, Height / 2);
	printf("沿仙神, 沿食遭");
	gotoxy(Width / 2 - 7, Height / 2 + 2);
	printf("戚酔遭, 穿呪朔");

	gotoxy(Width / 2 - 8, Height / 2 + 7);
	printf("曽戟馬獣畏柔艦猿?");
	gotoxy(Width / 2 - 6, Height / 2 + 9);
	printf("森");
	gotoxy(Width / 2 + 1, Height / 2 + 9);
	printf("焼艦神");
	gotoxy(Width / 2 - 3, Height / 2 + 9);
	printf("��");

	int return_n = 0;
	while (1) //徹左球 崇送績
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
				case 75: //疎 奄沙
					gotoxy(Width / 2 - 3 + return_n, Height / 2 + 9);
					printf("  ");
					return_n = 0;
					gotoxy(Width / 2 - 3, Height / 2 + 9);
					printf("��");
					break;
				case 77: //酔 11牒蒋
					return_n = 11;
					gotoxy(Width / 2 + 8 - return_n, Height / 2 + 9);
					printf("  ");
					gotoxy(Width / 2 + 8, Height / 2 + 9);
					printf("��");
					break;
				default:
					break;
				}
			}
			else
				if (key == 13)//殖斗
				{
					if (return_n == 0)
						exit(1);
					else
						break;
				}
		}
	}
}

void MenuOne() // 惟績獣拙 五敢
{
	system("cls");
	startDropT = clock();
	CreateRandomForm();

	while (1)
	{
		DrawMap();
		DrawBlock();
		DropBlock();
		BlockToGround();
		RemoveLine();
		InputKey();
	}
}

void CreateRandomForm() { // 鷺薫戚 鎧形臣凶原陥 沓棋生稽 郊会. 0~6
	blockForm = rand() % 7;
}

void DrawMap()
{
	gotoxy(0, 0);
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 12; j++) {
			switch (space[i][j]) {
			case 1:
				gotoxy(j * 2, i);
				printf("÷");
				break;
			case 2: // blockForm = 0
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13); // 左虞事, T切鷺薫
				gotoxy(j * 2, i);
				printf("＝");
				break;
			case 3: // blockForm = 1
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // 段系事,    けけ 鷺薫
				gotoxy(j * 2, i);									       	 //           けけ
				printf("＝");
				break;
			case 4: // blockForm = 2
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // 察悪事,  けけ   鷺薫
				gotoxy(j * 2, i);									       	 //             けけ
				printf("＝");
				break;
			case 5: // blockForm = 3
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); // 馬潅事, 掩貸廃 鷺薫
				gotoxy(j * 2, i);
				printf("＝");
				break;
			case 6: // blockForm = 4                                         //            け
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9); // 督櫛事,    け  鷺薫
				gotoxy(j * 2, i);                                           //           けけ
				printf("＝");
				break;
			case 7: // blockForm = 5                                         //          け
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); // 噺事,    け    鷺薫
				gotoxy(j * 2, i);                                            //          けけ
				printf("＝");
				break;
			case 8: // blockForm = 6
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // 葛櫛事, 革乞鷺薫
				gotoxy(j * 2, i);
				printf("＝");
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
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
		break;
	case 1:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		break;
	case 2:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		break;
	case 3:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
		break;
	case 4:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		break;
	case 5:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		break;
	case 6:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		break;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
				gotoxy(x + j * 2, y + i);
				printf("＝");
			}
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void DropBlock() // 0.8段原陥 鷺薫聖 廃牒梢 購生稽 鎧顕
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

void BlockToGround() { // 1段疑照 競拭 願焼赤聖凶 疑拙戚 蒸生檎 競生稽 痕敗, 沓棋廃 鷺薫聖 幻級壱 是稽 臣顕
	if (CheckCrash(x, y + 1) == true) {
		if ((float)(endT - startGroundT) > 1000) {
			// 薄仙 鷺系 煽舌
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
			x = 8;
			y = 0;
			CreateRandomForm();
		}
	}
}

void RemoveLine() {
	for (int i = 15; i >= 0; i--) { // 混虞昔 薦須廃 葵
		int cnt = 0;
		for (int j = 1; j < 11; j++) { // 
			if (space[i][j] == 2 || space[i][j] == 3 || space[i][j] == 4 || space[i][j] == 5 || space[i][j] == 6 || space[i][j] == 7 || space[i][j] == 8) {
				cnt++;
			}
		}
		if (cnt >= 10) { // 混宜戚 陥 託赤陥檎
			for (int j = 0; i - j >= 0; j++) {
				for (int x = 1; x < 11; x++) {
					if (i - j - 1 >= 0)
						space[i - j][x] = space[i - j - 1][x];
					else      // 探舌戚檎 0煽舌
						space[i - j][x] = 0;
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
		}
		system("cls");
	}
}

bool CheckCrash(int x, int y) { // 中宜姶走 違帖澗惟 赤生檎 true研 鋼発
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
				int t = space[i + y][j + x / 2];
				if (t == 1 || t == 2 || t == 3 || t == 4 || t == 5 || t == 6 || t == 7 || t == 8) { // 混析 凶, 鷺薫析 凶
					return true;
				}
			}
		}
	}
	return false;
}