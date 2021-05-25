#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib") //����

// �⺻ ���� ���
#define Width 120  // â ���� ũ��
#define Height 40  // â ���� ũ��
#define kbhit _kbhit
#define getch _getch
//�� ����� ���� �ذ��� ����

// �⺻ ���� �Լ�
void Console_Size(); // �ܼ� ������ ����
void CursorView(char show); // Ŀ�� ������ �����. 0�̸� ����, 1�̸� ����
void gotoxy(int x, int y); //Ŀ�� �̵� �Լ�

// �޴� ���� �Լ�
void DesignMainMenu(); // ���� �޴� ������
int MainMenu(); // ���� �޴�
void MenuTwo(); // ���۹� �޴�
void MenuThree(); // ������ �޴�


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
			printf("���ӽ���");
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
	printf("\n\n\n\n");
	printf("			������   ����   ������   �����      ������     �����  \n");
	printf("			    ��       ��           ��       ��      ��        ��        ��         \n");
	printf("			    ��       ����       ��       �����          ��         �����  \n");
	printf("			    ��       ��           ��       ��     ��         ��                ��  \n");
	printf("			    ��       ����       ��       ��      ��    ������     �����  \n");
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
	gotoxy(Width / 2 - 3, Height / 2 - 9);
	printf("����Ű");
	gotoxy(Width / 2 - 10, Height / 2 - 5);
	printf("��, �� : Move Left, Right");
	gotoxy(Width / 2 - 6, Height / 2 - 3);
	printf("�� : Soft Drop");
	gotoxy(Width / 2 - 9, Height / 2 - 1);
	printf("Space : Hard Drop");
	gotoxy(Width / 2 - 6, Height / 2 + 1);
	printf("�� : Rotate");
	gotoxy(Width / 2 - 9, Height / 2 + 3);
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
	gotoxy(Width / 2 - 2, Height / 2 - 7);
	printf("����");
	gotoxy(Width / 2 - 15, Height / 2 - 3);
	printf("�߾Ӵ��б� ����Ʈ�����к� 21�й�");
	gotoxy(Width / 2 - 7, Height / 2 );
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