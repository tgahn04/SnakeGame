#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH	100
#define HEIGHT  50
#define UP		72
#define DOWN	80
#define LEFT	75
#define RIGHT	77
#define MAX_FOOD 15
#define MAX_POISON 20

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
	const char* shape;
} Food;

typedef struct
{
	int x;
	int y;
	const char* shape;
} Poison;

Food* food = NULL;
int food_count = 0;

Poison* poison = NULL;
int poison_count = 0;

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

void renderFood(Food *food)
{
	DWORD dword;
	COORD position = {food->x, food->y};

	SetConsoleCursorPosition(screen[screen_index], position);
	WriteFile(screen[screen_index], food->shape, 1, &dword, NULL);
}

void renderFoods()
{
	for (int i = 0; i < food_count; i++)
	{
		renderFood(&food[i]);
	}
}

void renderPoison(Poison* poison)
{
	DWORD dword;
	COORD position = { poison->x, poison->y };

	SetConsoleCursorPosition(screen[screen_index], position);
	WriteFile(screen[screen_index], poison->shape, 1, &dword, NULL);
}

void renderPoisons()
{
	for (int i = 0; i < poison_count; i++)
	{
		renderPoison(&poison[i]);
	}
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

 void SnakeMove(Snake* snake)
 {
	 for (int i = snake->length - 1; i > 0; i--)
	 {
		 snake->position[i] = snake->position[i - 1];
	 }
	 
	 switch (snake->direction)
	 {
	 case UP	:	snake->position[0].y--; break;
	 case DOWN	:	snake->position[0].y++; break;
	 case LEFT	:	snake->position[0].x--; break;
	 case RIGHT	:	snake->position[0].x++; break;
	 }
 }

 int HitCheck(int x, int y, Snake* snake)
 {
	 for (int i = 0; i < snake->length; i++)
		 if (snake->position[i].x == x && snake->position[i].y == y) 
			 return 1;

	 for (int i = 0; i < food_count; i++)
		 if (food[i].x == x && food[i].y == y) 
			 return 1;

	 for (int i = 0; i < poison_count; i++)
		 if (poison[i].x == x && poison[i].y == y) 
			 return 1;

	 return 0;
 }

 void SpawnFood(Snake* snake)
 {
	 if (food_count >= MAX_FOOD)
		 return;

	 int x, y;
	 do 
	 {
		 x = rand() % (WIDTH - 3) + 1;
		 y = rand() % (HEIGHT - 3) + 1;
	 } 
	 while (HitCheck(x, y, snake));

	 food[food_count].x = rand() % (WIDTH - 3) + 1;
	 food[food_count].y = rand() % (WIDTH - 3) + 1;
	 food[food_count].shape = "F";
	 food_count++;
 }

 void SpawnPoison(Snake* snake)
 {
	 if (poison_count >= MAX_POISON)
		 return;

	 int x, y;
	 do
	 {
		 x = rand() % (WIDTH - 3) + 1;
		 y = rand() % (HEIGHT - 3) + 1;
	 } while (HitCheck(x, y, snake));

	 poison[poison_count].x = rand() % (WIDTH - 3) + 1;
	 poison[poison_count].y = rand() % (WIDTH - 3) + 1;
	 poison[poison_count].shape = "P";
	 poison_count++;
 }
 
 void RemoveFood(int i)
 {
	 if (i < 0 || i >= food_count)
		 return;
	 food[i] = food[food_count - 1];
	 food_count--;
 }

 void RemovePoison(int i)
 {
	 if (i < 0 || i >= poison_count)
		 return;
	 poison[i] = poison[poison_count - 1];
	 poison_count--;
 }

int main()
{
	srand((unsigned int)time(NULL));

	printf("---- Snake_Game ----\n\n");
	printf("조작법 : 방향키\n\n");
	printf("F를 먹으면 길이 + 1\n");
	printf("P를 먹으면 길이 - 1\n");
	printf("벽이나 자신의 몸에 부딪히면 게임오버입니다.\n\n");
	printf("아무 키나 누르면 시작합니다.\n");

	_getch();

	system("cls");

	Snake snake;
	snake.length = 3;
	snake.direction = RIGHT;

	for (int i = 0; i < snake.length; i++)
	{
		snake.position[i].x = WIDTH / 2 - i;
		snake.position[i].y = HEIGHT / 2;
	}

	food = (Food*)malloc(MAX_FOOD * sizeof(Food));
	poison = (Poison*)malloc(MAX_POISON * sizeof(Poison));
	
	initialize();
	SpawnFood(&snake);
	SpawnPoison(&snake);

	DWORD last_time = GetTickCount();

	while (1)
	{
		DWORD current_time = GetTickCount();
		if (current_time - last_time >= 10000)
		{
			SpawnFood(&snake);
			SpawnPoison(&snake);
			last_time = current_time;
		}

		if (_kbhit())
		{
			int key = _getch();

			if (key == 0 || key == 224) key = _getch();
			
			switch (key)
			{
				case UP		:	if (snake.direction != DOWN) snake.direction = UP; break;
				case LEFT	:	if (snake.direction != RIGHT) snake.direction = LEFT; break;
				case RIGHT	:	if (snake.direction != LEFT) snake.direction = RIGHT; break;
				case DOWN	:	if (snake.direction != UP) snake.direction = DOWN; break;
			}
		}

		SnakeMove(&snake);

		for (int i = 0; i < food_count; i++)
		{
			if (snake.position[0].x == food[i].x && snake.position[0].y == food[i].y)
			{
				snake.length++;
				RemoveFood(i);
				break;
			}
		}

		for (int i = 0; i < poison_count; i++)
		{
			if (snake.position[0].x == poison[i].x && snake.position[0].y == poison[i].y)
			{
				snake.length--;
				RemovePoison(i);
				break;
			}
		}
		
		if (snake.position[0].x <= 0 || snake.position[0].x >= WIDTH - 1 ||
			snake.position[0].y <= 0 || snake.position[0].y >= HEIGHT - 1)
		{
			printf("\nGAME_OVER\n");
			break;
		}

		for (int i = 1; i < snake.length; i++)
		{
			if (snake.position[0].x == snake.position[i].x &&
				snake.position[0].y == snake.position[i].y)
			{
				printf("\nGAME_OVER\n");
				exit(0);
				break;
			}
		}

		clear();
		renderField();
		renderSnake(snake);
		renderFoods();	
		renderPoisons();
		flip();
	}

	free(food);
	free(poison);
	release();
	

	return 0;
}