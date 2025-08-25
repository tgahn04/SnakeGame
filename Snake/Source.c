#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 100
#define HEIGHT 100
#define MAX_Length 500



void Field(int field[HEIGHT][WIDTH], Snake snake, Food food)
{
	system("cls");
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
				printf("■"); // 벽
			else if (i == snake.y[0] && j == snake.x[0])
				printf("@"); // 머리
			else if (i == food.y && j == food.x)
				printf("♡"); // 먹이
			else
			{
				for(int k = 1; k < snake.length; k++)
					if (i == snake.y[k] && j == snake.x[k])
					{
						printf("0"); // 몸통
						break;
					}
			}
		}
		printf("\n");
	}
}

typedef struct
{
	int x[MAX_Length];
	int y[MAX_Length];
	int length;
} Snake;

typedef struct 
{
	int x;
	int y;
} Food;

void SpawnFood(Food *f)
{
	f->x = rand() % WIDTH;
	f->x = rand() % HEIGHT;
}

void SnakeMove(Snake* snake, int dx, int dy)
{
	for (int i = snake->length; i > 0; i--)
	{
		snake->x[i] = snake->x[i - 1];
		snake->y[i] = snake->y[i - 1];
	}
	snake->x[0] += dx;
	snake->y[0] += dy;
}

int main()
{






	return 0;
}