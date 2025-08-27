#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH       100
#define HEIGHT      50
#define MAX_Length  500

#define UP    72
#define DOWN  80
#define LEFT  75
#define RIGHT 77

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
    int active;
} Food;


CHAR_INFO buffer[WIDTH * HEIGHT];
HANDLE hConsole;
COORD bufferSize = { WIDTH, HEIGHT };
COORD bufferCoord = { 0, 0 };
SMALL_RECT writeRegion = { 0, 0, WIDTH - 1, HEIGHT - 1 };

void InitConsoleBuffer()
{
    hConsole = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
    SetConsoleActiveScreenBuffer(hConsole);
}

void DrawBuffer() 
{
    WriteConsoleOutput(hConsole, buffer, bufferSize, bufferCoord, &writeRegion);
}

void Field(Snake snake, Food foods[], int foodCount) 
{
   
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++) 
        {
            int idx = i * WIDTH + j;
            buffer[idx].Char.AsciiChar = ' ';
            buffer[idx].Attributes = 0x07;
        }
    }

    for (int i = 0; i < HEIGHT; i++) 
    {
        for (int j = 0; j < WIDTH; j++) 
        {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) 
            {
                buffer[i * WIDTH + j].Char.AsciiChar = '#';
            }
        }
    }

    for (int f = 0; f < foodCount; f++)
    {
        if (foods[f].active)
        {
            buffer[foods[f].y * WIDTH + foods[f].x].Char.AsciiChar = 'O';
        }
    }

    for (int k = 0; k < snake.length; k++)
    {
        int x = snake.x[k];
        int y = snake.y[k];
        buffer[y * WIDTH + x].Char.AsciiChar = (k == 0) ? '@' : '0';
    }

    DrawBuffer();
}

void SpawnFood(Food* f) 
{
    f->x = rand() % (WIDTH - 2) + 1;
    f->y = rand() % (HEIGHT - 2) + 1;
    f->active = 1;
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

int HitSelf(Snake snake)
{
    for (int i = 1; i < snake.length; i++) 
    {
        if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i])
            return 1;
    }
    return 0;
}

int main() 
{
    srand((unsigned int)time(NULL));

        system("cls");
        printf("---- Snake_Game ----\n\n");
        printf("조작법 : 방향키\n\n");
        printf("먹이를 3개 먹으면 길이 + 1\n");
        printf("먹이 30개를 먹으면 게임이 클리어됩니다.\n");
        printf("벽이나 자신의 몸에 부딪히면 게임오버입니다.\n\n");
        printf("아무 키나 누르면 시작합니다.\n");

        _getch();

        InitConsoleBuffer();

        Snake snake = { {WIDTH / 2}, {HEIGHT / 2}, 1 };
        Food foods[1000] = { 0 };
        int foodCount = 0;

        int EatFood = 0;
        int dx = 0, dy = -1;
        int growthCount = 0;

        DWORD lastFoodTime = GetTickCount();

        while (1)
        {
            if (_kbhit())
            {
                int key = _getch();
                if (key == 0 || key == 224) key = _getch();
                switch (key) {
                case UP:    if (dy != 1) { dx = 0; dy = -1; } break;
                case DOWN:  if (dy != -1) { dx = 0; dy = 1; } break;
                case LEFT:  if (dx != 1) { dx = -1; dy = 0; } break;
                case RIGHT: if (dx != -1) { dx = 1; dy = 0; } break;
                }
            }

            if (GetTickCount() - lastFoodTime >= 2000)
            {
                if (foodCount < 1000) SpawnFood(&foods[foodCount++]);
                lastFoodTime = GetTickCount();
            }

            SnakeMove(&snake, dx, dy);

            if (snake.x[0] <= 0 || snake.x[0] >= WIDTH - 1 ||
                snake.y[0] <= 0 || snake.y[0] >= HEIGHT - 1)
            {
                system("cls");
                printf("GAME_OVER\n");
                break;
            }

            if (HitSelf(snake))
            {
                system("cls");
                printf("GAME_OVER\n");
                break;
            }

            for (int f = 0; f < foodCount; f++)
            {
                if (foods[f].active && snake.x[0] == foods[f].x && snake.y[0] == foods[f].y)
                {
                    foods[f].active = 0;
                    EatFood++;
                    growthCount++;

                    if (growthCount == 3)
                    {
                        if (snake.length < MAX_Length) snake.length++;
                        growthCount = 0;
                    }
                    if (EatFood >= 30)
                    {
                        system("cls");
                        printf("GAME_CLEAR!\n총 점수: %d\n", EatFood);
                        return 0;
                    }
                }
            }


            Field(snake, foods, foodCount);

            Sleep(33);
        }
    return 0;
}