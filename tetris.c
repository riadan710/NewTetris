#include <stdio.h>
#include <Windows.h>

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


int main() {
	CursorView(0);  // Ŀ�� ������ �����. 0�̸� ����, 1�̸� ����
	Console_Size(); // �ܼ� ������ ����
	DesignMainMenu(); // ���θ޴� ������ ���

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
			printf("���۹�");
			Sleep(300);
			break;
		case 6:
			printf("������");
			Sleep(300);
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