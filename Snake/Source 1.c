#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH       100
#define HEIGHT      50
#define UP    72
#define DOWN  80
#define LEFT  75
#define RIGHT 77

int screen_index;

HANDLE screen[2];

typedef struct
{
	int x;
	int y;

} Position;

typedef struct
{
	int length;
	int direction;
	Position position[100];

} Snake;

void initialize()
{
	CONSOLE_CURSOR_INFO cursor;

	screen[0] = CreateConsoleScreenBuffer
	(
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CONSOLE_TEXTMODE_BUFFER, NULL
	);

	screen[1] = CreateConsoleScreenBuffer
	(
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CONSOLE_TEXTMODE_BUFFER, NULL
	);

	cursor.dwSize = 1;
	cursor.bVisible = FALSE;

	SetConsoleCursorInfo(screen[0], &cursor);
	SetConsoleCursorInfo(screen[1], &cursor);
}

void flip()
{
	SetConsoleActiveScreenBuffer(screen[screen_index]);

	screen_index = !screen_index;
}

void clear()
{
	COORD position = { 0, 0 };

	DWORD dword;

	CONSOLE_SCREEN_BUFFER_INFO console;

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(handle, &console);

	int width = console.srWindow.Right - console.srWindow.Left + 1;
	int height = console.srWindow.Bottom - console.srWindow.Top + 1;

	FillConsoleOutputCharacter(screen[screen_index], ' ', width * height, position, &dword);
}

void release()
{
	CloseHandle(screen[0]);
	CloseHandle(screen[1]);
}

void render(Snake snake)
{
	DWORD dword;
	COORD position = { snake.position[0].x, snake.position[0].y };
	char c = '@';
	SetConsoleCursorPosition(screen[screen_index], position);
	WriteFile(screen[screen_index], &c, 1, &dword, NULL);
}

// void Field(Snake snake, Position position)
// {
// 	for (int i = 0; i < HEIGHT; i++)
// 	{
// 		for (int j = 0; j < WIDTH; j++)
// 		{
// 
// 		}
// 	}
// }


int main()
{
	 Snake snake;
	 snake.length = 1;
	 snake.direction = RIGHT;

	 snake.position[0].x = 10;
	 snake.position[0].y = 10;

	initialize();

	while (1)
	{
		if (_kbhit())
		{
			int key = _getch();
			if (key == 0 || key == 224) key = _getch();
			// switch (key)
			// {
			// 	case UP :  if (snake.direction != DOWN) snake.direction = UP; break;
			// 	case LEFT: if (snake.direction != RIGHT) snake.direction = LEFT; break;
			// 	case RIGHT: if (snake.direction != LEFT) snake.direction = RIGHT; break;
			// 	case DOWN: if (snake.direction != UP) snake.direction = DOWN; break;
			// }
			// switch()

			switch (key)
			{
			case UP:	 snake.direction = UP; break;
			case LEFT:	 snake.direction = LEFT; break;
			case RIGHT:	 snake.direction = RIGHT; break;
			case DOWN:	 snake.direction = DOWN; break;
			}

			switch (snake.direction)
			{
				case UP:	 snake.position[0].y--; break;
				case LEFT:	 snake.position[0].x--; break;
				case RIGHT:	 snake.position[0].x++; break;
				case DOWN:	 snake.position[0].y++; break;
			}
		}
		
		clear();
		render(snake);
		flip();
	}
	release();
	return 0;
}