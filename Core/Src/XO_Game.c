#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include <stdio.h>

// تضمين الدرايفرات وملفات المنطق التي تحتاجها اللعبة
#include "MGPIO_Interface.h"
#include "MSTK_Interface.h"
#include "HTFT_Interface.h"
#include "XO_Logic.h"
#include "XO_Game.h"

/* تعريفات خاصة باللعبة -------------------------------------------------*/
// الألوان
#define BLACK       0x0000
#define WHITE       0xFFFF
#define BLUE        0x001F
#define RED         0xF800
#define YELLOW      0xFFE0
#define GRAY        0x8410

// أبعاد الشاشة واللوحة
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   160
#define BOARD_ORIGIN_Y  20
#define BOARD_ORIGIN_X  4
#define CELL_SIZE       40
#define GRID_THICKNESS  4
#define SYMBOL_PADDING  5

// تعريف بنات الأزرار
#define X_PLAYER_BUTTON_PORT       PORTB
#define X_PLAYER_BUTTON_PIN        PIN6
#define MOVE_BUTTON_PORT           PORTB
#define MOVE_BUTTON_PIN            PIN7
#define O_PLAYER_BUTTON_PORT       PORTB
#define O_PLAYER_BUTTON_PIN        PIN8

/* NEW - Buzzer Pin Definition */
#define BUZZER_PORT     PORTB
#define BUZZER_PIN      PIN10


/* خط 5x7 بكسل (Font data remains the same) */
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

/* نماذج الدوال الخاصة (للاستخدام داخل هذا الملف فقط) */
static void GUI_voidDrawBoard(void);
static void GUI_voidDrawSymbol(u8 Copy_u8Row, u8 Copy_u8Col, u8 Copy_u8Player);
static void GUI_voidDrawCursor(u8 Copy_u8Row, u8 Copy_u8Col, u16 Copy_u16Color);
static void GUI_voidShowEndScreen(u8 game_state);
static void GUI_voidUpdateWinCounters(void);
static void GUI_voidDrawString(u8 x, u8 y, char* str, u16 color, u8 size);
static u8 debounce(Port_t port, PIN_t pin);
static u8 check_long_press(Port_t port, PIN_t pin, u16 delay_ms);

/* NEW - Sound Function Prototypes */
static void Sound_voidInit(void);
static void Sound_voidBeep(u16 frequency, u16 duration_ms);
static void Sound_voidPlayClick(void);
static void Sound_voidPlayMove(void);
static void Sound_voidPlayWin(void);
static void Sound_voidPlayDraw(void);
static void Sound_voidPlayError(void);

// ... (بقية نماذج الدوال)

void XO_Game_voidRun(void)
{
    // تهيئة اللعبة والصوت
    Sound_voidInit(); /* NEW */
    HTFT_voidFillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
    XO_voidInitBoard();
    GUI_voidDrawBoard();
    GUI_voidUpdateWinCounters();

    u8 cursor_row = 0;
    u8 cursor_col = 0;
    u8 game_state = GAME_ONGOING;
    u8 game_ended_sound_played = 0; // Flag to play sound only once

    GUI_voidDrawCursor(cursor_row, cursor_col, YELLOW);

    while (1)
    {
        if (game_state != GAME_ONGOING) {
            if (!game_ended_sound_played) {
                if(game_state == GAME_WIN_X) {
                    XO_voidIncrementWinCount(PLAYER_X);
                    Sound_voidPlayWin(); /* NEW */
                } else if (game_state == GAME_WIN_O) {
                    XO_voidIncrementWinCount(PLAYER_O);
                    Sound_voidPlayWin(); /* NEW */
                } else if (game_state == GAME_DRAW) {
                    Sound_voidPlayDraw(); /* NEW */
                }
                game_ended_sound_played = 1; // Mark sound as played
            }

            GUI_voidShowEndScreen(game_state);
            MSTK_voidDelayms(4000);
            return;
        }

        if (debounce(MOVE_BUTTON_PORT, MOVE_BUTTON_PIN)) {
            Sound_voidPlayClick(); /* NEW */
            u8 old_cell_state = XO_u8GetCellState(cursor_row, cursor_col);
            GUI_voidDrawCursor(cursor_row, cursor_col, BLACK);

            if (old_cell_state != EMPTY) {
                GUI_voidDrawSymbol(cursor_row, cursor_col, old_cell_state);
            }

            cursor_col++;
            if (cursor_col > 2) {
                cursor_col = 0;
                cursor_row++;
                if (cursor_row > 2) {
                    cursor_row = 0;
                }
            }
            GUI_voidDrawCursor(cursor_row, cursor_col, YELLOW);
        }

        if (check_long_press(MOVE_BUTTON_PORT, MOVE_BUTTON_PIN, 1500)) {
            XO_voidResetWinCounters();
            HTFT_voidFillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GRAY);
            MSTK_voidDelayms(150);
            HTFT_voidFillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
            GUI_voidDrawBoard();
            GUI_voidUpdateWinCounters();
            GUI_voidDrawCursor(cursor_row, cursor_col, YELLOW);
        }

        u8 current_player = XO_u8GetCurrentPlayer();

        if (current_player == PLAYER_X && debounce(X_PLAYER_BUTTON_PORT, X_PLAYER_BUTTON_PIN)) {
            if (XO_u8MakeMove(cursor_row, cursor_col)) {
                Sound_voidPlayMove(); /* NEW */
                GUI_voidDrawSymbol(cursor_row, cursor_col, current_player);
                game_state = XO_u8CheckGameState();
                if(game_state == GAME_ONGOING) {
                    XO_voidSwitchPlayer();
                }
            } else {
                Sound_voidPlayError(); /* NEW */
            }
        }
        else if (current_player == PLAYER_O && debounce(O_PLAYER_BUTTON_PORT, O_PLAYER_BUTTON_PIN)) {
            if (XO_u8MakeMove(cursor_row, cursor_col)) {
                Sound_voidPlayMove(); /* NEW */
                GUI_voidDrawSymbol(cursor_row, cursor_col, current_player);
                game_state = XO_u8CheckGameState();
                if(game_state == GAME_ONGOING) {
                    XO_voidSwitchPlayer();
                }
            } else {
                Sound_voidPlayError(); /* NEW */
            }
        }
        MSTK_voidDelayms(50);
    }
}

/* دوال الواجهة الرسومية (GUI) كما هي بدون تغيير */
static u8 debounce(Port_t port, PIN_t pin) { if (MGPIO_u8GetPinValue(port, pin) == HIGH) { MSTK_voidDelayms(50); if (MGPIO_u8GetPinValue(port, pin) == HIGH) { while(MGPIO_u8GetPinValue(port, pin) == HIGH); return 1; } } return 0; }
static u8 check_long_press(Port_t port, PIN_t pin, u16 delay_ms) { if (MGPIO_u8GetPinValue(port, pin) == HIGH) { MSTK_voidDelayms(delay_ms); if (MGPIO_u8GetPinValue(port, pin) == HIGH) { while(MGPIO_u8GetPinValue(port, pin) == HIGH); return 1; } } return 0; }
static void GUI_voidDrawBoard(void) { u8 board_width = (CELL_SIZE * 3); HTFT_voidFillRect(BOARD_ORIGIN_X + CELL_SIZE - (GRID_THICKNESS/2), BOARD_ORIGIN_Y, GRID_THICKNESS, board_width, WHITE); HTFT_voidFillRect(BOARD_ORIGIN_X + CELL_SIZE * 2 - (GRID_THICKNESS/2), BOARD_ORIGIN_Y, GRID_THICKNESS, board_width, WHITE); HTFT_voidFillRect(BOARD_ORIGIN_X, BOARD_ORIGIN_Y + CELL_SIZE - (GRID_THICKNESS/2), board_width, GRID_THICKNESS, WHITE); HTFT_voidFillRect(BOARD_ORIGIN_X, BOARD_ORIGIN_Y + CELL_SIZE * 2 - (GRID_THICKNESS/2), board_width, GRID_THICKNESS, WHITE); }
static void GUI_voidDrawSymbol(u8 Copy_u8Row, u8 Copy_u8Col, u8 Copy_u8Player) { u8 x_start = BOARD_ORIGIN_X + (Copy_u8Col * CELL_SIZE) + SYMBOL_PADDING; u8 y_start = BOARD_ORIGIN_Y + (Copy_u8Row * CELL_SIZE) + SYMBOL_PADDING; u8 symbol_size = CELL_SIZE - (2 * SYMBOL_PADDING); u8 line_thick = 3; if (Copy_u8Player == PLAYER_X) { for(u8 i=0; i < symbol_size; i++) { HTFT_voidFillRect(x_start + i, y_start + i, line_thick, line_thick, RED); HTFT_voidFillRect(x_start + (symbol_size - i - 1), y_start + i, line_thick, line_thick, RED); } } else if (Copy_u8Player == PLAYER_O) { HTFT_voidFillRect(x_start, y_start, symbol_size, line_thick, BLUE); HTFT_voidFillRect(x_start, y_start + symbol_size - line_thick, symbol_size, line_thick, BLUE); HTFT_voidFillRect(x_start, y_start, line_thick, symbol_size, BLUE); HTFT_voidFillRect(x_start + symbol_size - line_thick, y_start, line_thick, symbol_size, BLUE); } }
static void GUI_voidDrawCursor(u8 Copy_u8Row, u8 Copy_u8Col, u16 Copy_u16Color) { u8 x = BOARD_ORIGIN_X + Copy_u8Col * CELL_SIZE; u8 y = BOARD_ORIGIN_Y + Copy_u8Row * CELL_SIZE; u8 thick = 2; u8 inset = 3; if (Copy_u16Color == BLACK) { HTFT_voidFillRect(x + thick, y + thick, CELL_SIZE - (2 * thick), CELL_SIZE - (2 * thick), BLACK); } else { HTFT_voidFillRect(x + inset, y + inset, CELL_SIZE - (2 * inset), thick, Copy_u16Color); HTFT_voidFillRect(x + inset, y + CELL_SIZE - thick - inset, CELL_SIZE - (2 * inset), thick, Copy_u16Color); HTFT_voidFillRect(x + inset, y + inset, thick, CELL_SIZE - (2 * inset), Copy_u16Color); HTFT_voidFillRect(x + CELL_SIZE - thick - inset, y + inset, thick, CELL_SIZE - (2 * inset), Copy_u16Color); } }
static void GUI_voidUpdateWinCounters() { char score_str[5]; HTFT_voidFillRect(0, 0, SCREEN_WIDTH, 18, BLACK); GUI_voidDrawString(5, 5, "X:", RED, 2); sprintf(score_str, "%d", XO_u8GetWinCount(PLAYER_X)); GUI_voidDrawString(30, 5, score_str, WHITE, 2); GUI_voidDrawString(SCREEN_WIDTH - 50, 5, "O:", BLUE, 2); sprintf(score_str, "%d", XO_u8GetWinCount(PLAYER_O)); GUI_voidDrawString(SCREEN_WIDTH - 25, 5, score_str, WHITE, 2); }
static void GUI_voidShowEndScreen(u8 game_state) { char final_score_str[20];	HTFT_voidFillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);	if (game_state == GAME_WIN_X) { GUI_voidDrawString(12, 50, "PLAYER X WINS!", RED, 2);	} else if (game_state == GAME_WIN_O) { GUI_voidDrawString(12, 50, "PLAYER O WINS!", BLUE, 2);	} else if (game_state == GAME_DRAW) { GUI_voidDrawString(35, 50, "DRAW!", WHITE, 2); } sprintf(final_score_str, "X:%d  O:%d", XO_u8GetWinCount(PLAYER_X), XO_u8GetWinCount(PLAYER_O)); GUI_voidDrawString(25, 80, final_score_str, YELLOW, 2); GUI_voidDrawString(15, 120, "RESTARTING...", GRAY, 1); }
static void GUI_voidDrawString(u8 x, u8 y, char* str, u16 color, u8 size) { while (*str) { if (*str < ' ' || *str > 'Z') { str++; continue; } const unsigned char* glyph = font5x7 + (*str - ' ') * 5; for (u8 col = 0; col < 5; col++) { u8 bits = glyph[col]; for (u8 row = 0; row < 7; row++) { if ((bits >> row) & 1) { HTFT_voidFillRect(x + col * size, y + row * size, size, size, color); } } } x += 6 * size; if (x > SCREEN_WIDTH - (6 * size)) { x = 0; y += 8 * size; } str++; } }


/*******************************************************************************
* NEW & CORRECTED SOUND FUNCTIONS                                              *
*******************************************************************************/

static void Sound_voidInit(void) {
    MGPIO_voidSetMode(BUZZER_PORT, BUZZER_PIN, OUTPUT);
    MGPIO_voidSetOutputConfig(BUZZER_PORT, BUZZER_PIN, Push_Pull, Low_Speed);
}

static void Sound_voidBeep(u16 frequency, u16 duration_ms) {
    if (frequency == 0) return;
    u32 half_period_us = 500000 / frequency;
    if (half_period_us == 0) return;
    u32 total_cycles = ((u32)duration_ms * 1000) / (half_period_us * 2);
    for (u32 i = 0; i < total_cycles; i++) {
        MGPIO_voidSetPinValue(BUZZER_PORT, BUZZER_PIN, HIGH);
        MSTK_voidDelayus(half_period_us);
        MGPIO_voidSetPinValue(BUZZER_PORT, BUZZER_PIN, LOW);
        MSTK_voidDelayus(half_period_us);
    }
    MGPIO_voidSetPinValue(BUZZER_PORT, BUZZER_PIN, LOW);
}

// صوت نقرة لتحريك المؤشر
static void Sound_voidPlayClick(void) {
    Sound_voidBeep(3000, 30);
}

// صوت لتأكيد وضع علامة X أو O
static void Sound_voidPlayMove(void) {
    Sound_voidBeep(1500, 60);
}

// صوت الفوز في الجولة
static void Sound_voidPlayWin(void) {
    Sound_voidBeep(1200, 100);
    MSTK_voidDelayms(50);
    Sound_voidBeep(1600, 100);
    MSTK_voidDelayms(50);
    Sound_voidBeep(2000, 150);
}

// صوت التعادل
static void Sound_voidPlayDraw(void) {
    Sound_voidBeep(1000, 150);
    MSTK_voidDelayms(80);
    Sound_voidBeep(800, 150);
}

// صوت عند محاولة اللعب في خانة ممتلئة
static void Sound_voidPlayError(void) {
    Sound_voidBeep(400, 200);
}
