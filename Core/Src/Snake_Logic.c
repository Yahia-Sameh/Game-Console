#include "Snake_Logic.h"
#include <stdlib.h> // لاستخدام rand()

// متغيرات static خاصة باللعبة
static Point_t G_stSnakeBody[MAX_SNAKE_LENGTH];
static u16 G_u16SnakeLength;
static SnakeDirection_t G_enSnakeDirection;
static Point_t G_stFood;
static u16 G_u16Score;
static u8 G_u8GameState;
static u8 G_u8FoodEatenFlag;
static Point_t G_stLastTailPosition;
static u16 G_u16HighScore = 0; // متغير لتخزين أعلى درجة

// دالة خاصة لتوليد الطعام في مكان عشوائي
static void Snake_voidGenerateFood(void) {
    u8 is_on_snake;
    do {
        is_on_snake = 0;
        G_stFood.x = rand() % SNAKE_GRID_WIDTH;
        G_stFood.y = rand() % SNAKE_GRID_HEIGHT;

        for (u16 i = 0; i < G_u16SnakeLength; i++) {
            if (G_stSnakeBody[i].x == G_stFood.x && G_stSnakeBody[i].y == G_stFood.y) {
                is_on_snake = 1;
                break;
            }
        }
    } while (is_on_snake);
}

void Snake_voidInit(void) {
    // تهيئة الثعبان في المنتصف
    G_stSnakeBody[0].x = SNAKE_GRID_WIDTH / 2;
    G_stSnakeBody[0].y = SNAKE_GRID_HEIGHT / 2;
    G_u16SnakeLength = 3;
    G_stSnakeBody[1].x = G_stSnakeBody[0].x - 1;
    G_stSnakeBody[1].y = G_stSnakeBody[0].y;
    G_stSnakeBody[2].x = G_stSnakeBody[0].x - 2;
    G_stSnakeBody[2].y = G_stSnakeBody[0].y;

    G_enSnakeDirection = DIR_RIGHT;
    G_u16Score = 0;
    G_u8GameState = SNAKE_GAME_ONGOING;
    G_u8FoodEatenFlag = 0;
    Snake_voidGenerateFood();
}

void Snake_voidChangeDirection(u8 Copy_u8TurnDirection) {
    if (Copy_u8TurnDirection == TURN_CLOCKWISE) {
        switch (G_enSnakeDirection) {
            case DIR_UP:    G_enSnakeDirection = DIR_RIGHT; break;
            case DIR_RIGHT: G_enSnakeDirection = DIR_DOWN;  break;
            case DIR_DOWN:  G_enSnakeDirection = DIR_LEFT;  break;
            case DIR_LEFT:  G_enSnakeDirection = DIR_UP;    break;
        }
    } else if (Copy_u8TurnDirection == TURN_COUNTER_CLOCKWISE) {
        switch (G_enSnakeDirection) {
            case DIR_UP:    G_enSnakeDirection = DIR_LEFT;  break;
            case DIR_LEFT:  G_enSnakeDirection = DIR_DOWN;  break;
            case DIR_DOWN:  G_enSnakeDirection = DIR_RIGHT; break;
            case DIR_RIGHT: G_enSnakeDirection = DIR_UP;    break;
        }
    }
}

u8 Snake_u8Move(void) {
    if (G_u8GameState == SNAKE_GAME_OVER) {
        return SNAKE_GAME_OVER;
    }

    G_stLastTailPosition = G_stSnakeBody[G_u16SnakeLength - 1];

    for (s16 i = G_u16SnakeLength - 1; i > 0; i--) {
        G_stSnakeBody[i] = G_stSnakeBody[i - 1];
    }

    Point_t* head = &G_stSnakeBody[0];
    switch (G_enSnakeDirection) {
        case DIR_UP:    head->y--; break;
        case DIR_RIGHT: head->x++; break;
        case DIR_DOWN:  head->y++; break;
        case DIR_LEFT:  head->x--; break;
    }

    /* --- منطق الالتفاف حول الشاشة --- */
    if (head->x >= SNAKE_GRID_WIDTH)  head->x = 0;
    else if (head->x < 0)             head->x = SNAKE_GRID_WIDTH - 1;

    if (head->y >= SNAKE_GRID_HEIGHT) head->y = 0;
    else if (head->y < 0)             head->y = SNAKE_GRID_HEIGHT - 1;
    /* --- نهاية منطق الالتفاف --- */

    // التحقق من الاصطدام بالجسم (الخسارة الوحيدة الممكنة)
    for (u16 i = 1; i < G_u16SnakeLength; i++) {
        if (head->x == G_stSnakeBody[i].x && head->y == G_stSnakeBody[i].y) {
            G_u8GameState = SNAKE_GAME_OVER;
            // تحديث أعلى درجة عند الخسارة
            if (G_u16Score > G_u16HighScore) {
                G_u16HighScore = G_u16Score;
            }
            return SNAKE_GAME_OVER;
        }
    }

    // التحقق من أكل الطعام
    G_u8FoodEatenFlag = 0;
    if (head->x == G_stFood.x && head->y == G_stFood.y) {
        G_u8FoodEatenFlag = 1;
        G_u16Score++;
        if (G_u16SnakeLength < MAX_SNAKE_LENGTH) {
            G_u16SnakeLength++;
            G_stSnakeBody[G_u16SnakeLength - 1] = G_stLastTailPosition;
        }
        Snake_voidGenerateFood();
    }

    return SNAKE_GAME_ONGOING;
}

Point_t Snake_stGetHeadPosition(void) {
    return G_stSnakeBody[0];
}

Point_t Snake_stGetTailPosition(void) {
    return G_stLastTailPosition;
}

Point_t Snake_stGetFoodPosition(void) {
    return G_stFood;
}

u16 Snake_u16GetLength(void) {
    return G_u16SnakeLength;
}

u16 Snake_u16GetScore(void) {
    return G_u16Score;
}

u8 Snake_u8IsFoodEaten(void) {
    return G_u8FoodEatenFlag;
}

u16 Snake_u16GetHighScore(void) {
    return G_u16HighScore;
}
