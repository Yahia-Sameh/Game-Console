#ifndef SNAKE_LOGIC_H
#define SNAKE_LOGIC_H

#include "STD_TYPES.h"

// أبعاد شبكة اللعبة
#define SNAKE_CELL_SIZE     10
#define SNAKE_GRID_WIDTH    (128 / SNAKE_CELL_SIZE)
#define SNAKE_GRID_HEIGHT   ((160 - 20) / SNAKE_CELL_SIZE)
#define MAX_SNAKE_LENGTH    (SNAKE_GRID_WIDTH * SNAKE_GRID_HEIGHT)

// اتجاهات الحركة
typedef enum {
    DIR_RIGHT,
    DIR_UP,
    DIR_LEFT,
    DIR_DOWN
} SnakeDirection_t;

// اتجاهات الدوران
#define TURN_CLOCKWISE        1
#define TURN_COUNTER_CLOCKWISE 2

// حالات اللعبة
#define SNAKE_GAME_ONGOING    0
#define SNAKE_GAME_OVER       1

// بنية لتخزين إحداثيات أي نقطة
typedef struct {
    s8 x;
    s8 y;
} Point_t;

void Snake_voidInit(void);
void Snake_voidChangeDirection(u8 Copy_u8TurnDirection);
u8 Snake_u8Move(void);
Point_t Snake_stGetHeadPosition(void);
Point_t Snake_stGetTailPosition(void);
Point_t Snake_stGetFoodPosition(void);
u16 Snake_u16GetLength(void);
u16 Snake_u16GetScore(void);
u8 Snake_u8IsFoodEaten(void);
u16 Snake_u16GetHighScore(void);

#endif /* SNAKE_LOGIC_H */
