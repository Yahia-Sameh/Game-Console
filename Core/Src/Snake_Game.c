#include "STD_TYPES.h"
#include <stdio.h>

#include "MGPIO_Interface.h"
#include "MSTK_Interface.h"
#include "HTFT_Interface.h"
#include "Snake_Logic.h"
#include "Snake_Game.h"

#define BLACK       0x0000
#define WHITE       0xFFFF
#define GREEN       0x07E0
#define RED         0xF800
#define YELLOW      0xFFE0
#define GRAY        0x8410
#define BLUE_SNAKE  0x041F
#define STEM_GREEN  0x04A0

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   160
#define GAME_AREA_Y     20

#define CLOCKWISE_BUTTON_PORT      PORTB
#define CLOCKWISE_BUTTON_PIN       PIN8
#define COUNTER_CW_BUTTON_PORT     PORTB
#define COUNTER_CW_BUTTON_PIN      PIN6

/* NEW - Buzzer Pin Definition */
#define BUZZER_PORT     PORTB
#define BUZZER_PIN      PIN10

/* Static Prototypes -- MODIFIED */
static void GUI_voidDrawInitialState(void);
static void GUI_voidUpdateScreen(void);
static void GUI_voidShowGameOver(void);
static void GUI_voidUpdateScore(void);
static void GUI_voidDrawSnakeSegment(u8 x, u8 y, u16 color);
static void GUI_voidDrawFood(u8 x, u8 y, u16 color);
static void GUI_voidEraseCell(u8 x, u8 y);
static void GUI_voidDrawString(u8 x, u8 y, char* str, u16 color, u8 size);
static u8 debounce(Port_t port, PIN_t pin);

/* NEW - Sound Function Prototypes */
static void Sound_voidInit(void);
static void Sound_voidBeep(u16 frequency, u16 duration_ms);
static void Sound_voidPlayClick(void);
static void Sound_voidPlayWin(void);
static void Sound_voidPlayGameOver(void);


/* 5x7 Font */
static const unsigned char font5x7[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, 0x07, 0x00, 0x07, 0x00, 0x14, 0x7F, 0x14, 0x7F, 0x14,
	0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x23, 0x13, 0x08, 0x64, 0x62, 0x36, 0x49, 0x55, 0x22, 0x50, 0x00, 0x05, 0x03, 0x00, 0x00,
	0x00, 0x1C, 0x22, 0x41, 0x00, 0x00, 0x41, 0x22, 0x1C, 0x00, 0x08, 0x2A, 0x1C, 0x2A, 0x08, 0x08, 0x08, 0x3E, 0x08, 0x08,
	0x00, 0x50, 0x30, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x60, 0x60, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x02,
	0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x42, 0x7F, 0x40, 0x00, 0x42, 0x61, 0x51, 0x49, 0x46, 0x21, 0x41, 0x45, 0x4B, 0x31,
	0x18, 0x14, 0x12, 0x7F, 0x10, 0x27, 0x45, 0x45, 0x45, 0x39, 0x3C, 0x4A, 0x49, 0x49, 0x30, 0x01, 0x71, 0x09, 0x05, 0x03,
	0x36, 0x49, 0x49, 0x49, 0x36, 0x06, 0x49, 0x49, 0x29, 0x1E, 0x00, 0x36, 0x36, 0x00, 0x00, 0x00, 0x56, 0x36, 0x00, 0x00,
	0x00, 0x08, 0x14, 0x22, 0x41, 0x14, 0x14, 0x14, 0x14, 0x14, 0x41, 0x22, 0x14, 0x08, 0x00, 0x02, 0x01, 0x51, 0x09, 0x06,
	0x32, 0x49, 0x79, 0x41, 0x3E, 0x7E, 0x11, 0x11, 0x11, 0x7E, 0x7F, 0x49, 0x49, 0x49, 0x36, 0x3E, 0x41, 0x41, 0x41, 0x22,
	0x7F, 0x41, 0x41, 0x22, 0x1C, 0x7F, 0x49, 0x49, 0x49, 0x41, 0x7F, 0x09, 0x09, 0x01, 0x01, 0x3E, 0x41, 0x41, 0x51, 0x72,
	0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x41, 0x7F, 0x41, 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01, 0x7F, 0x08, 0x14, 0x22, 0x41,
	0x7F, 0x40, 0x40, 0x40, 0x40, 0x7F, 0x02, 0x04, 0x02, 0x7F, 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x3E, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x09, 0x09, 0x09, 0x06, 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x7F, 0x09, 0x19, 0x29, 0x46, 0x46, 0x49, 0x49, 0x49, 0x31,
	0x01, 0x01, 0x7F, 0x01, 0x01, 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x3F, 0x40, 0x38, 0x40, 0x3F,
	0x63, 0x14, 0x08, 0x14, 0x63, 0x03, 0x04, 0x78, 0x04, 0x03, 0x61, 0x51, 0x49, 0x45, 0x43
};

/*******************************************************************************
* THE MODIFIED GAME LOOP                             *
*******************************************************************************/
void Snake_Game_voidRun(void) {
    Snake_voidInit();
    Sound_voidInit(); /* NEW - Initialize Buzzer Pin */
    GUI_voidDrawInitialState();

    u32 move_interval = 300; // ms, a smaller value means a faster snake

    while (1) {
        if (debounce(CLOCKWISE_BUTTON_PORT, CLOCKWISE_BUTTON_PIN)) {
            Sound_voidPlayClick(); /* NEW - Play click sound */
            Snake_voidChangeDirection(TURN_CLOCKWISE);
        }
        if (debounce(COUNTER_CW_BUTTON_PORT, COUNTER_CW_BUTTON_PIN)) {
            Sound_voidPlayClick(); /* NEW - Play click sound */
            Snake_voidChangeDirection(TURN_COUNTER_CLOCKWISE);
        }

        if (Snake_u8Move() == SNAKE_GAME_OVER) {
            Sound_voidPlayGameOver(); /* NEW - Play game over sound */
            GUI_voidShowGameOver();
            MSTK_voidDelayms(4000);
            return;
        }

        GUI_voidUpdateScreen();

        if (Snake_u16GetScore() > 0 && Snake_u16GetScore() % 5 == 0 && move_interval > 100) {
             move_interval -= 20;
        }

        MSTK_voidDelayms(move_interval);
    }
}

/*******************************************************************************
* DRAWING FUNCTIONS                                                            *
*******************************************************************************/
static void GUI_voidDrawSnakeSegment(u8 x_grid, u8 y_grid, u16 color) {
    u16 x_pos = x_grid * SNAKE_CELL_SIZE;
    u16 y_pos = GAME_AREA_Y + (y_grid * SNAKE_CELL_SIZE);
    u8 size = SNAKE_CELL_SIZE;

    if (size < 5) {
        HTFT_voidFillRect(x_pos, y_pos, size, size, color);
        return;
    }

    u8 offset = size / 4;
    u8 length = size - (2 * offset);
    HTFT_voidFillRect(x_pos + offset, y_pos, length, size, color);
    HTFT_voidFillRect(x_pos, y_pos + offset, size, length, color);
}

static void GUI_voidDrawFood(u8 x_grid, u8 y_grid, u16 color) {
    GUI_voidDrawSnakeSegment(x_grid, y_grid, color);
    u16 x_pos = x_grid * SNAKE_CELL_SIZE;
    u16 y_pos = GAME_AREA_Y + (y_grid * SNAKE_CELL_SIZE);
    u8 size = SNAKE_CELL_SIZE;

    if (size > 6) {
        u8 stem_width = (size / 4 > 0) ? size / 4 : 1;
        u8 stem_height = stem_width;
        u8 stem_x_offset = (size - stem_width) / 2;
        HTFT_voidFillRect(x_pos + stem_x_offset, y_pos, stem_width, stem_height, STEM_GREEN);
    }
}

static void GUI_voidEraseCell(u8 x_grid, u8 y_grid) {
    u16 x_pos = x_grid * SNAKE_CELL_SIZE;
    u16 y_pos = GAME_AREA_Y + (y_grid * SNAKE_CELL_SIZE);
    HTFT_voidFillRect(x_pos, y_pos, SNAKE_CELL_SIZE, SNAKE_CELL_SIZE, BLACK);
}

static void GUI_voidUpdateScore(void) {
    char score_str[15];
    HTFT_voidFillRect(0, 0, SCREEN_WIDTH, GAME_AREA_Y - 1, BLACK);
    sprintf(score_str, "SCORE: %d", Snake_u16GetScore());
    GUI_voidDrawString(10, 5, score_str, YELLOW, 2);
}

static void GUI_voidDrawInitialState(void) {
    HTFT_voidFillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
    GUI_voidUpdateScore();
    Point_t head = Snake_stGetHeadPosition();
    GUI_voidDrawSnakeSegment(head.x, head.y, GREEN);
    Point_t food = Snake_stGetFoodPosition();
    GUI_voidDrawFood(food.x, food.y, RED);
}

static void GUI_voidUpdateScreen(void) {
    Point_t head = Snake_stGetHeadPosition();
    GUI_voidDrawSnakeSegment(head.x, head.y, GREEN);

    if (!Snake_u8IsFoodEaten()) {
        Point_t tail = Snake_stGetTailPosition();
        GUI_voidEraseCell(tail.x, tail.y);
    } else {
        Sound_voidPlayWin(); /* NEW - Play win sound */
        GUI_voidUpdateScore();
        Point_t food = Snake_stGetFoodPosition();
        GUI_voidDrawFood(food.x, food.y, RED);
    }
}

static void GUI_voidShowGameOver(void) {
    char score_str[20];
    char high_score_str[20];
    HTFT_voidFillRect(0, 40, SCREEN_WIDTH, 100, GRAY);
    GUI_voidDrawString(15, 50, "GAME OVER", RED, 2);
    sprintf(score_str, "SCORE: %d", Snake_u16GetScore());
    GUI_voidDrawString(20, 80, score_str, YELLOW, 2);
    sprintf(high_score_str, "HIGH: %d", Snake_u16GetHighScore());
    GUI_voidDrawString(20, 105, high_score_str, WHITE, 2);
}

static u8 debounce(Port_t port, PIN_t pin) {
    if (MGPIO_u8GetPinValue(port, pin) == HIGH) {
        MSTK_voidDelayms(50);
        if (MGPIO_u8GetPinValue(port, pin) == HIGH) {
            while(MGPIO_u8GetPinValue(port, pin) == HIGH);
            return 1;
        }
    }
    return 0;
}

static void GUI_voidDrawString(u8 x, u8 y, char* str, u16 color, u8 size) {
    while (*str) {
        if (*str < ' ' || *str > 'Z') { str++; continue; }
        const unsigned char* glyph = font5x7 + (*str - ' ') * 5;
        for (u8 col = 0; col < 5; col++) {
            u8 bits = glyph[col];
            for (u8 row = 0; row < 7; row++) {
                if ((bits >> row) & 1) {
                    HTFT_voidFillRect(x + col * size, y + row * size, size, size, color);
                }
            }
        }
        x += 6 * size;
        if (x > SCREEN_WIDTH - (6 * size)) { x = 0; y += 8 * size; }
        str++;
    }
}

/*******************************************************************************
* CORRECTED SOUND FUNCTIONS                                                    *
*******************************************************************************/

/*
 * @brief Initializes the buzzer pin using the correct 2-step functions.
 */
static void Sound_voidInit(void) {
    // Step 1: Set the pin mode to General Purpose Output
    MGPIO_voidSetMode(BUZZER_PORT, BUZZER_PIN, OUTPUT);

    // Step 2: Configure the output type as Push-Pull and set the speed
    MGPIO_voidSetOutputConfig(BUZZER_PORT, BUZZER_PIN, Push_Pull, Low_Speed);
}

/*
 * @brief Generates a square wave using available SetPinValue functions.
 */
static void Sound_voidBeep(u16 frequency, u16 duration_ms) {
    if (frequency == 0) return;

    // Calculate the half period in microseconds
    u32 half_period_us = 500000 / frequency;

    // Calculate how many full cycles (HIGH-LOW) are needed for the duration
    if (half_period_us == 0) return; // Avoid division by zero
    u32 total_cycles = ((u32)duration_ms * 1000) / (half_period_us * 2);

    for (u32 i = 0; i < total_cycles; i++) {
        // Set pin HIGH for the first half of the period
        MGPIO_voidSetPinValue(BUZZER_PORT, BUZZER_PIN, HIGH);
        MSTK_voidDelayus(half_period_us);

        // Set pin LOW for the second half of the period
        MGPIO_voidSetPinValue(BUZZER_PORT, BUZZER_PIN, LOW);
        MSTK_voidDelayus(half_period_us);
    }

    // Ensure the pin is left in a LOW state
    MGPIO_voidSetPinValue(BUZZER_PORT, BUZZER_PIN, LOW);
}

/*
 * @brief Plays a short, high-pitched click sound.
 */
static void Sound_voidPlayClick(void) {
    Sound_voidBeep(2500, 40);
}

/*
 * @brief Plays a rising two-tone "success" sound.
 */
static void Sound_voidPlayWin(void) {
    Sound_voidBeep(1000, 100);
    MSTK_voidDelayms(50);
    Sound_voidBeep(1500, 100);
}

/*
 * @brief Plays a low-pitched, longer "failure" sound.
 */
static void Sound_voidPlayGameOver(void) {
    Sound_voidBeep(400, 500);
}
