#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h> 
#include <stdbool.h>
#include <mmsystem.h>
#include <stdlib.h>

#pragma comment(lib, "winmm.lib") //����

clock_t startDropT, endT, startGroundT;

int x = 8, y = 0;
RECT blockSize;
int blockForm;
int blockRotation = 0;
int key;

#define Width 90  // â ���� ũ��
#define Height 30  // â ���� ũ��
#define kbhit _kbhit
#define getch _getch

int block[7][4][4][4] = {  // [7]�� ������. �������� ��ȯ, 2���� [4]�� �����Ҷ����� ȸ��
	{ // T��� ��
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
	{    // ���� ��
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
	{   // ���� �� �ݴ�
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
	{   // 1���� ��
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
	{   // L���� ��
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
	{   // L���� �� �ݴ�
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
	{   // �׸� ��
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

int space[15 + 1][10 + 2] = {  // ���� 15+1(�Ʒ���)ĭ, ���� 10+2(���� ��)ĭ  
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

void Console_Size(); // �ܼ� ������ ����
void CursorView(char show); // Ŀ�� ������ �����. 0�̸� ����, 1�̸� ����
void gotoxy(int x, int y); //Ŀ�� �̵� �Լ�

void DesignMainMenu(); // ���� �޴� ������
int MainMenu(); // ���� �޴�
void MenuTwo(); // ���۹� �޴�
void MenuThree(); // ������ �޴�
void MenuOne(); // ���ӽ��� �޴�
void DrawMap(); // ���� ���¿� ���� �� �׸�
void DrawBlock(); // ���� �� �׸��� (4�����迭)
void DropBlock(); // 0.8�ʸ��� ���� ��ĭ�� ������ ����
void BlockToGround(); // 1�ʵ��� ���� ��������� ������ ������ ������ ����, ������ ���� ����� ���� �ø�
void RemoveLine(); // ������ �� ��ĭ�� ����
void InputKey();
void CreateRandomForm(); // ���� �����ö����� �������� �ٲ�. 0~6
bool CheckCrash(int x, int y); // �浹���� ��ġ�°� ������ true�� ��ȯ

int main() {
	CursorView(0);  // Ŀ�� ������ �����. 0�̸� ����, 1�̸� ����
	Console_Size(); // �ܼ� ������ ����
	DesignMainMenu(); // ���θ޴� ������ ���
	//PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // ������� ���

	while (1) // ���� �޴� ����
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

void gotoxy(int x, int y) //Ŀ�� �̵� �Լ�
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Console_Size() // �ܼ� ������ ����
{
	char temp[50];
	sprintf(temp, "Mode con cols=%d lines=%d", Width, Height);
	system(temp);
}

void CursorView(char show)  // Ŀ�� ������ �����. 0�̸� ����, 1�̸� ����
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void DesignMainMenu() // ���� �޴� ������
{
	printf("\n\n");
	printf("         ������   ����   ������   �����      ������     �����  \n");
	printf("             ��       ��           ��       ��      ��        ��        ��         \n");
	printf("             ��       ����       ��       �����          ��         �����  \n");
	printf("             ��       ��           ��       ��     ��         ��                ��  \n");
	printf("             ��       ����       ��       ��      ��    ������     �����  \n");
	printf("\n\n\n");
}

int MainMenu()
{
	system("cls");
	DesignMainMenu();

	gotoxy(Width / 2 - 4, Height / 2);
	printf("���� ����");
	gotoxy(Width / 2 - 4, Height / 2 + 3);
	printf("���۹�");
	gotoxy(Width / 2 - 4, Height / 2 + 6);
	printf("������");
	gotoxy(Width / 2 - 4, Height / 2 + 9);
	printf("���� ����");
	gotoxy(Width / 2 + 8, Height / 2);
	printf("��");

	int return_n = 0;
	while (1) // Ű���� ������
	{
		int key;
		if (kbhit()) // Ű���� �Է��� ������ ���
		{
			key = getch(); // �� Ű�� �ƽ�Ű�ڵ尪�� �޾� key�� ����
			if (key == 224 || key == 0) // �� Ű�� ����Ű�� ��� �۵�
			{
				key = getch();
				switch (key)
				{
				case 72: // ���� ����Ű
					gotoxy(Width / 2 + 8, Height / 2 + return_n); //���� �ڸ��� �̵�
					printf("  "); //����
					return_n -= 3; //ȭ��ǥ�� ��ǥ�� ���� 3�����Ű��
					if (return_n < 0) return_n = 0; //���������� ������ ���ϰ�
					gotoxy(Width / 2 + 8, Height / 2 + return_n);
					printf("��"); //�ٲ� ��ǥ�� ����Ű ���
					break;
				case 80: //�Ʒ��� ����Ű�� ���� ���
					gotoxy(Width / 2 + 8, Height / 2 + return_n); //���� �ڸ��� �̵�
					printf("  "); //����
					return_n += 3; //ȭ��ǥ�� ��ǥ�� �Ʒ��� 3�����Ű��
					if (return_n > 6) return_n = 9; //���������� ������ ���ϰ�
					gotoxy(Width / 2 + 8, Height / 2 + return_n);
					printf("��"); //�ٲ� ��ǥ�� ����Ű ���
					break;
				default:
					break;
				}
			}
			else
				if (key == 13) //����Ű�� ������ ��
					return return_n; //ȭ��ǥ�� ��ǥ���� ��ȯ
		}
	}
}

void MenuTwo() // ���۹� �޴�
{
	system("cls");
	DesignMainMenu();
	gotoxy(Width / 2 - 3, Height / 2 - 6);
	printf("����Ű");
	gotoxy(Width / 2 - 10, Height / 2 - 4);
	printf("��, �� : Move Left, Right");
	gotoxy(Width / 2 - 6, Height / 2 - 2);
	printf("�� : Soft Drop");
	gotoxy(Width / 2 - 9, Height / 2);
	printf("Space : Hard Drop");
	gotoxy(Width / 2 - 6, Height / 2 + 2);
	printf("�� : Rotate");
	gotoxy(Width / 2 - 9, Height / 2 + 4);
	printf("Shift : Hold");

	gotoxy(Width / 2 - 8, Height / 2 + 7);
	printf("�����Ͻðڽ��ϱ�?");
	gotoxy(Width / 2 - 6, Height / 2 + 9);
	printf("��");
	gotoxy(Width / 2 + 1, Height / 2 + 9);
	printf("�ƴϿ�");
	gotoxy(Width / 2 - 3, Height / 2 + 9);
	printf("��");

	int return_n = 0;
	while (1) //Ű���� ������
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
				case 75: //�� �⺻
					gotoxy(Width / 2 - 3 + return_n, Height / 2 + 9);
					printf("  ");
					return_n = 0;
					gotoxy(Width / 2 - 3, Height / 2 + 9);
					printf("��");
					break;
				case 77: //�� 11ĭ��
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
				if (key == 13)//����
				{
					if (return_n == 0)
						exit(1);
					else
						break;
				}
		}
	}
}

void MenuThree() // ������ �޴�
{
	system("cls");
	DesignMainMenu();
	gotoxy(Width / 2 - 2, Height / 2 - 6);
	printf("����");
	gotoxy(Width / 2 - 15, Height / 2 - 3);
	printf("�߾Ӵ��б� ����Ʈ�����к� 21�й�");
	gotoxy(Width / 2 - 7, Height / 2);
	printf("�����, �迩��");
	gotoxy(Width / 2 - 7, Height / 2 + 2);
	printf("�̿���, ������");

	gotoxy(Width / 2 - 8, Height / 2 + 7);
	printf("�����Ͻðڽ��ϱ�?");
	gotoxy(Width / 2 - 6, Height / 2 + 9);
	printf("��");
	gotoxy(Width / 2 + 1, Height / 2 + 9);
	printf("�ƴϿ�");
	gotoxy(Width / 2 - 3, Height / 2 + 9);
	printf("��");

	int return_n = 0;
	while (1) //Ű���� ������
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
				case 75: //�� �⺻
					gotoxy(Width / 2 - 3 + return_n, Height / 2 + 9);
					printf("  ");
					return_n = 0;
					gotoxy(Width / 2 - 3, Height / 2 + 9);
					printf("��");
					break;
				case 77: //�� 11ĭ��
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
				if (key == 13)//����
				{
					if (return_n == 0)
						exit(1);
					else
						break;
				}
		}
	}
}

void MenuOne() // ���ӽ��� �޴�
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

void CreateRandomForm() { // ���� �����ö����� �������� �ٲ�. 0~6
	srand(time(NULL));

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
				printf("��");
				break;
			case 2: // blockForm = 0
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13); // �����, T�ں�
				gotoxy(j * 2, i);
				printf("��");
				break;
			case 3: // blockForm = 1
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // �ʷϻ�,    ���� ��
				gotoxy(j * 2, i);									       	 //           ����
				printf("��");
				break;
			case 4: // blockForm = 2
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // ������,  ����   ��
				gotoxy(j * 2, i);									       	 //             ����
				printf("��");
				break;
			case 5: // blockForm = 3
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); // �ϴû�, ������ ��
				gotoxy(j * 2, i);
				printf("��");
				break;
			case 6: // blockForm = 4                                         //            ��
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9); // �Ķ���,    ��  ��
				gotoxy(j * 2, i);                                           //           ����
				printf("��");
				break;
			case 7: // blockForm = 5                                         //          ��
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); // ȸ��,    ��    ��
				gotoxy(j * 2, i);                                            //          ����
				printf("��");
				break;
			case 8: // blockForm = 6
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // �����, �׸��
				gotoxy(j * 2, i);
				printf("��");
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
				printf("��");
			}
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void DropBlock() // 0.8�ʸ��� ���� ��ĭ�� ������ ����
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

void BlockToGround() { // 1�ʵ��� ���� ��������� ������ ������ ������ ����, ������ ���� ����� ���� �ø�
	if (CheckCrash(x, y + 1) == true) {
		if ((float)(endT - startGroundT) > 1000) {
			// ���� ��� ����
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
	for (int i = 15; i >= 0; i--) { // ������ ������ ��
		int cnt = 0;
		for (int j = 1; j < 11; j++) { // 
			if (space[i][j] == 2 || space[i][j] == 3 || space[i][j] == 4 || space[i][j] == 5 || space[i][j] == 6 || space[i][j] == 7 || space[i][j] == 8) {
				cnt++;
			}
		}
		if (cnt >= 10) { // ������ �� ���ִٸ�
			for (int j = 0; i - j >= 0; j++) {
				for (int x = 1; x < 11; x++) {
					if (i - j - 1 >= 0)
						space[i - j][x] = space[i - j - 1][x];
					else      // õ���̸� 0����
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

bool CheckCrash(int x, int y) { // �浹���� ��ġ�°� ������ true�� ��ȯ
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
				int t = space[i + y][j + x / 2];
				if (t == 1 || t == 2 || t == 3 || t == 4 || t == 5 || t == 6 || t == 7 || t == 8) { // ���� ��, ���� ��
					return true;
				}
			}
		}
	}
	return false;
}