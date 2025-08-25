#include <stdio.h>
#include <string.h>
#include "GUI_Main.h"
#include "STD_TYPES.h"

// Drivers and Game files
#include "MGPIO_Interface.h"
#include "MSTK_Interface.h"
#include "HTFT_Interface.h"
#include "XO_Game.h"
#include "Snake_Game.h"

/************************************************************************************************/
/* UI & DESIGN DEFINITIONS                                                                      */
/************************************************************************************************/
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 160

#define CLASSIC_BEIGE      0xFFDC
#define DARK_WOOD          0x5A06
#define VIBRANT_ORANGE     0xCA80
#define LIGHT_TAN          0xD5AA
#define OFF_WHITE          0xFFFB

#define NAV_NEXT_PORT PORTB
#define NAV_NEXT_PIN  PIN8
#define SELECT_PORT   PORTB
#define SELECT_PIN    PIN7
#define NAV_PREV_PORT PORTB
#define NAV_PREV_PIN  PIN6

#define GAME_XO     0
#define GAME_SNAKE  1
#define TOTAL_GAMES 2

/* NEW - Buzzer Pin Definition */
#define BUZZER_PORT     PORTB
#define BUZZER_PIN      PIN10

/************************************************************************************************/
/* خط 8x8 بكسل (Font data remains the same)                                                       */
/************************************************************************************************/
const unsigned char font8x8[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // U+0020 (space)
	0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00,  // U+0021 (!)
	0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // U+0022 (")
	0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00,  // U+0023 (#)
	0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00,  // U+0024 ($)
	0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00,  // U+0025 (%)
	0x1C, 0x36, 0x1C, 0x3B, 0x36, 0x37, 0x1D, 0x00,  // U+0026 (&)
	0x06, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,  // U+0027 (')
	0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00,  // U+0028 (()
	0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00,  // U+0029 ())
	0x00, 0x18, 0x3C, 0x7E, 0x3C, 0x18, 0x00, 0x00,  // U+002A (*)
	0x00, 0x0C, 0x0C, 0x3E, 0x0C, 0x0C, 0x00, 0x00,  // U+002B (+)
	0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, 0x00,  // U+002C (,)
	0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00,  // U+002D (-)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00,  // U+002E (.)
	0x00, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x00,  // U+002F (/)
	0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00,  // U+0030 (0)
	0x0C, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00,  // U+0031 (1)
	0x3E, 0x63, 0x03, 0x0E, 0x1C, 0x30, 0x7F, 0x00,  // U+0032 (2)
	0x3E, 0x63, 0x03, 0x1E, 0x03, 0x63, 0x3E, 0x00,  // U+0033 (3)
	0x06, 0x0F, 0x18, 0x30, 0x66, 0x66, 0x3F, 0x00,  // U+0034 (4)
	0x7F, 0x60, 0x7E, 0x03, 0x03, 0x63, 0x3E, 0x00,  // U+0035 (5)
	0x3E, 0x63, 0x60, 0x7C, 0x63, 0x63, 0x3E, 0x00,  // U+0036 (6)
	0x7F, 0x03, 0x06, 0x0C, 0x18, 0x18, 0x18, 0x00,  // U+0037 (7)
	0x3E, 0x63, 0x63, 0x3E, 0x63, 0x63, 0x3E, 0x00,  // U+0038 (8)
	0x3E, 0x63, 0x63, 0x3F, 0x03, 0x63, 0x3E, 0x00,  // U+0039 (9)
	0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00,  // U+003A (:)
	0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x30, 0x00,  // U+003B (;)
	0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00,  // U+003C (<)
	0x00, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x00, 0x00,  // U+003D (=)
	0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00,  // U+003E (>)
	0x3E, 0x63, 0x03, 0x0E, 0x0C, 0x00, 0x0C, 0x00,  // U+003F (?)
	0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x63, 0x3E, 0x00,  // U+0040 (@)
	0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00,  // U+0041 (A)
	0x7E, 0x63, 0x63, 0x7E, 0x63, 0x63, 0x7E, 0x00,  // U+0042 (B)
	0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00,  // U+0043 (C)
	0x7C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00,  // U+0044 (D)
	0x7F, 0x03, 0x03, 0x3E, 0x03, 0x03, 0x7F, 0x00,  // U+0045 (E)
	0x7F, 0x06, 0x06, 0x3E, 0x06, 0x06, 0x06, 0x00,  // U+0046 (F)
	0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x3E, 0x00,  // U+0047 (G)
	0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00,  // U+0048 (H)
	0x3E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3E, 0x00,  // U+0049 (I)
	0x0F, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C, 0x00,  // U+004A (J)
	0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00,  // U+004B (K)
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x7F, 0x00,  // U+004C (L)
	0x63, 0x77, 0x7F, 0x6B, 0x6B, 0x6B, 0x6B, 0x00,  // U+004D (M)
	0x63, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x63, 0x00,  // U+004E (N)
	0x3E, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00,  // U+004F (O)
	0x7E, 0x63, 0x63, 0x7E, 0x03, 0x03, 0x03, 0x00,  // U+0050 (P)
	0x3E, 0x63, 0x63, 0x63, 0x7B, 0x33, 0x3E, 0x60,  // U+0051 (Q)
	0x7E, 0x63, 0x63, 0x7E, 0x36, 0x66, 0x67, 0x00,  // U+0052 (R)
	0x3E, 0x63, 0x03, 0x1E, 0x60, 0x63, 0x3E, 0x00,  // U+0053 (S)
	0x7F, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00,  // U+0054 (T)
	0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00,  // U+0055 (U)
	0x63, 0x63, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00,  // U+0056 (V)
	0x63, 0x6B, 0x6B, 0x6B, 0x7F, 0x77, 0x63, 0x00,  // U+0057 (W)
	0x63, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x63, 0x00,  // U+0058 (X)
	0x63, 0x63, 0x36, 0x1C, 0x0C, 0x0C, 0x0C, 0x00,  // U+0059 (Y)
	0x7F, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x00,  // U+005A (Z)
};

const char *game_names[TOTAL_GAMES] = {"XO Game", "Snake Game"};

// Prototypes
static void GUI_voidDrawString(u16 x, u16 y, const char *str, u16 color, u16 bg, u8 transparent);
static void HILI_voidDrawRect(u16 x, u16 y, u16 w, u16 h, u16 color);
static void GUI_voidDrawGameCard(u8 index, u8 is_selected);
static void GUI_voidDrawMenu(s8 selected_game);
static void GUI_voidUpdateSelection(s8 old_selection, s8 new_selection);
static u8   debounce(Port_t port, PIN_t pin);

/* NEW - Sound Function Prototypes */
static void Sound_voidInit(void);
static void Sound_voidBeep(u16 frequency, u16 duration_ms);
static void Sound_voidPlayNavigate(void);
static void Sound_voidPlaySelect(void);


void GUI_Main_voidRun(void) {
    s8 current_selection = GAME_XO;

    Sound_voidInit(); // NEW: Initialize sound system
    GUI_voidDrawMenu(current_selection);

    while (1) {
        u8 selection_changed = 0;
        s8 previous_selection = current_selection;

        if (debounce(NAV_NEXT_PORT, NAV_NEXT_PIN)) {
            Sound_voidPlayNavigate(); // NEW: Play navigation sound
            current_selection = (current_selection + 1) % TOTAL_GAMES;
            selection_changed = 1;
        } else if (debounce(NAV_PREV_PORT, NAV_PREV_PIN)) {
            Sound_voidPlayNavigate(); // NEW: Play navigation sound
            current_selection = (current_selection == 0) ? (TOTAL_GAMES - 1) : (current_selection - 1);
            selection_changed = 1;
        }

        if (selection_changed) {
            GUI_voidUpdateSelection(previous_selection, current_selection);
        }

        if (debounce(SELECT_PORT, SELECT_PIN)) {
            Sound_voidPlaySelect(); // NEW: Play selection sound
            MSTK_voidDelayms(100); // Small delay to let the sound play

            if (current_selection == GAME_XO) {
                XO_Game_voidRun();
            } else if (current_selection == GAME_SNAKE) {
                Snake_Game_voidRun();
            }
            // After returning from a game, redraw the menu
            GUI_voidDrawMenu(current_selection);
        }
        MSTK_voidDelayms(50);
    }
}

void GUI_voidDrawGameCard(u8 index, u8 is_selected) {
    const u16 card_w = 110; const u16 card_h = 32; const u16 gap_between_cards = 15; const u16 title_h = 8; const u16 gap_after_title = 18;
    u16 total_content_height = title_h + gap_after_title + (TOTAL_GAMES * card_h) + ((TOTAL_GAMES - 1) * gap_between_cards);
    u16 start_y_block = (SCREEN_HEIGHT - total_content_height) / 2;
    u16 card_y = start_y_block + title_h + gap_after_title + (index * (card_h + gap_between_cards));
    u16 card_x = (SCREEN_WIDTH - card_w) / 2;
    u16 card_bg, border_color, text_color;
    if (is_selected) { card_bg = VIBRANT_ORANGE; border_color = OFF_WHITE; text_color = OFF_WHITE;
    } else { card_bg = LIGHT_TAN; border_color = DARK_WOOD; text_color = DARK_WOOD; }
    HTFT_voidFillRect(card_x, card_y, card_w, card_h, card_bg);
    HILI_voidDrawRect(card_x, card_y, card_w, card_h, border_color);
    if (is_selected) { HILI_voidDrawRect(card_x + 1, card_y + 1, card_w - 2, card_h - 2, DARK_WOOD); }
    u16 text_x = card_x + 10; u16 text_y = card_y + (card_h - 8) / 2;
    GUI_voidDrawString(text_x, text_y, game_names[index], text_color, card_bg, 1);
}

void GUI_voidDrawMenu(s8 selected_game) {
    HTFT_voidFillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, CLASSIC_BEIGE);
    HTFT_voidFillRect(0, 0, SCREEN_WIDTH, 3, DARK_WOOD);
    HTFT_voidFillRect(0, SCREEN_HEIGHT - 3, SCREEN_WIDTH, 3, DARK_WOOD);
    const u16 card_h = 32; const u16 title_h = 8; const u16 gap_after_title = 18; const u16 gap_between_cards = 15;
    u16 total_content_height = title_h + gap_after_title + (TOTAL_GAMES * card_h) + ((TOTAL_GAMES - 1) * gap_between_cards);
    u16 start_y_block = (SCREEN_HEIGHT - total_content_height) / 2;
    const char* title = "SELECT GAME";
    u16 title_width = strlen(title) * 8; u16 title_x = (SCREEN_WIDTH - title_width) / 2; u16 title_y = start_y_block;
    GUI_voidDrawString(title_x, title_y, title, VIBRANT_ORANGE, CLASSIC_BEIGE, 1);
    for (int i = 0; i < TOTAL_GAMES; i++) { GUI_voidDrawGameCard(i, (i == selected_game)); }
}

void GUI_voidUpdateSelection(s8 old_selection, s8 new_selection) {
    GUI_voidDrawGameCard(old_selection, 0);
    GUI_voidDrawGameCard(new_selection, 1);
}

void HILI_voidDrawRect(u16 x, u16 y, u16 w, u16 h, u16 color) {
    HTFT_voidFillRect(x, y, w, 1, color);
    HTFT_voidFillRect(x, y + h - 1, w, 1, color);
    HTFT_voidFillRect(x, y, 1, h, color);
    HTFT_voidFillRect(x + w - 1, y, 1, h, color);
}

void GUI_voidDrawString(u16 x, u16 y, const char *str, u16 color, u16 bg, u8 transparent) {
    while (*str) {
        if (*str >= 32 && *str <= 122) {
            const u8* glyph = font8x8 + (*str - 32) * 8;
            for (u8 row = 0; row < 8; row++) {
                u8 row_data = glyph[row];
                for (u8 col = 0; col < 8; col++) {
                    if ((row_data >> col) & 1) {
                        HTFT_voidFillRect(x + col, y + row, 1, 1, color);
                    } else if (!transparent) {
                        HTFT_voidFillRect(x + col, y + row, 1, 1, bg);
                    }
                }
            }
        }
        x += 8;
        str++;
    }
}

static u8 debounce(Port_t port, PIN_t pin) {
    if (MGPIO_u8GetPinValue(port, pin) == HIGH) {
        MSTK_voidDelayms(50);
        if (MGPIO_u8GetPinValue(port, pin) == HIGH) {
            while (MGPIO_u8GetPinValue(port, pin) == HIGH);
            return 1;
        }
    }
    return 0;
}


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

// صوت للتنقل بين الخيارات
static void Sound_voidPlayNavigate(void) {
    Sound_voidBeep(2800, 35); // نغمة قصيرة وحادة
}

// صوت لتأكيد الاختيار
static void Sound_voidPlaySelect(void) {
    Sound_voidBeep(1800, 80); // نغمة أطول وأكثر صلابة
}
