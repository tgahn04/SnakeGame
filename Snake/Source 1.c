#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH	100
#define HEIGHT  25
#define UP		72
#define DOWN	80
#define LEFT	75
#define RIGHT	77

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

typedef struct
{
	int x;
	int y;

} Food;

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

void renderSnake(Snake snake)
{
	DWORD dword;

	COORD head_position = { snake.position[0].x, snake.position[0].y };
	char head = '@';
	SetConsoleCursorPosition(screen[screen_index], head_position);
	WriteFile(screen[screen_index], &head, 1, &dword, NULL);
	
	for (int i = 1; i < snake.length; i++)
	{
		COORD body_position = { snake.position[i].x, snake.position[i].y };
		char body = '0';
		SetConsoleCursorPosition(screen[screen_index], body_position);
		WriteFile(screen[screen_index], &body, 1, &dword, NULL);
	}
}

void renderFood(Food food)
{
	DWORD dword;

	COORD position = {food.x, food.y};
	char f = '¢¾';
	SetConsoleCursorPosition(screen[screen_index], position);
	WriteFile(screen[screen_index], &f, 1, &dword, NULL);
}

 void renderField()
 {
	 DWORD dword;

	 char wall = '#';
	 char emp = ' ';

 	for (int i = 0; i < HEIGHT; i++)
 	{
 		for (int j = 0; j < WIDTH; j++)
 		{
			COORD position = { j, i };

			if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
			{
				SetConsoleCursorPosition(screen[screen_index], position);
				WriteFile(screen[screen_index], &wall, 1, &dword, NULL);
			}
			else
			{
				SetConsoleCursorPosition(screen[screen_index], position);
				WriteFile(screen[screen_index], &emp, 1, &dword, NULL);
			}
 		}
 	}
 }

 void SnakeMove(Snake snake)
 {
	 for (int i = snake.length - 1; i > 0; i--)
	 {
		 snake.position[i] = snake.position[i - 1];
	 }

	 switch (snake.direction)
	 {
	 case UP:    snake.position[0].y--; break;
	 case DOWN:  snake.position[0].y++; break;
	 case LEFT:  snake.position[0].x--; break;
	 case RIGHT: snake.position[0].x++; break;
	 }
 }

 void SpawnFood(Food* f)
 {
	 f->x = rand() % (WIDTH - 2) + 1;
	 f->y = rand() % (HEIGHT - 2) + 1;
 }

int main()
{
	srand((unsigned int)time(NULL));

	Snake snake;
	snake.length = 3;
	snake.direction = RIGHT;
	snake.position[0].x = WIDTH / 2;
	snake.position[0].y = HEIGHT / 2;
	snake.position[1].x = WIDTH / 2 - 1;
	snake.position[1].y = HEIGHT / 2;
	snake.position[2].x = WIDTH / 2 - 2;
	snake.position[2].y = HEIGHT / 2;

	initialize();

	while (1)
	{
		if (_kbhit())
		{
			int key = _getch();

			if (key == 0 || key == 224) key = _getch();

			switch (key)
			{
				case UP :	if (snake.direction != DOWN) snake.direction = UP; break;
				case LEFT:	if (snake.direction != RIGHT) snake.direction = LEFT; break;
				case RIGHT: if (snake.direction != LEFT) snake.direction = RIGHT; break;
				case DOWN:	if (snake.direction != UP) snake.direction = DOWN; break;
			}

			switch (snake.direction)
			{
			case UP:    snake.position[0].y--; break;
			case DOWN:  snake.position[0].y++; break;
			case LEFT:  snake.position[0].x--; break;
			case RIGHT: snake.position[0].x++; break;
			}
			
			for (int i = snake.length; i > 0; i--)
			{
				snake.position[i] = snake.position[i - 1];
			}
			
			
		}
		
		clear();
		renderField();
		renderSnake(snake);
		flip();
	}
	release();
	return 0;
}