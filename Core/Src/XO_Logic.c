#include "XO_Logic.h"

// متغيرات static global لتخزين حالة اللعبة، يمكن الوصول إليها فقط داخل هذا الملف.
static u8 G_u8GameBoard[3][3];
static u8 G_u8CurrentPlayer;
static u8 G_u8PlayerX_Wins = 0;
static u8 G_u8PlayerO_Wins = 0;

void XO_voidInitBoard(void) {
    for (u8 r = 0; r < 3; r++) {
        for (u8 c = 0; c < 3; c++) {
            G_u8GameBoard[r][c] = EMPTY;
        }
    }
    // اللاعب X يبدأ اللعبة دائمًا.
    G_u8CurrentPlayer = PLAYER_X;
}

u8 XO_u8GetCurrentPlayer(void) {
    return G_u8CurrentPlayer;
}

void XO_voidSwitchPlayer(void) {
    if (G_u8CurrentPlayer == PLAYER_X) {
        G_u8CurrentPlayer = PLAYER_O;
    } else {
        G_u8CurrentPlayer = PLAYER_X;
    }
}

u8 XO_u8MakeMove(u8 Copy_u8Row, u8 Copy_u8Col) {
    // التحقق من الحركات غير الصالحة (خارج الحدود أو الخانة غير فارغة).
    if (Copy_u8Row >= 3 || Copy_u8Col >= 3 || G_u8GameBoard[Copy_u8Row][Copy_u8Col] != EMPTY) {
        return 0; // حركة غير صالحة.
    }
    // وضع علامة اللاعب الحالي على اللوحة.
    G_u8GameBoard[Copy_u8Row][Copy_u8Col] = G_u8CurrentPlayer;
    return 1; // حركة ناجحة.
}

u8 XO_u8CheckGameState(void) {
    u8 winner = EMPTY;

    // التحقق من الصفوف بحثًا عن فوز
    for (u8 i = 0; i < 3; i++) {
        if (G_u8GameBoard[i][0] != EMPTY && G_u8GameBoard[i][0] == G_u8GameBoard[i][1] && G_u8GameBoard[i][1] == G_u8GameBoard[i][2]) {
            winner = G_u8GameBoard[i][0];
            break;
        }
    }

    // التحقق من الأعمدة بحثًا عن فوز، إذا لم يكن هناك فائز بعد
    if (winner == EMPTY) {
        for (u8 i = 0; i < 3; i++) {
            if (G_u8GameBoard[0][i] != EMPTY && G_u8GameBoard[0][i] == G_u8GameBoard[1][i] && G_u8GameBoard[1][i] == G_u8GameBoard[2][i]) {
                winner = G_u8GameBoard[0][i];
                break;
            }
        }
    }

    // التحقق من الأقطار بحثًا عن فوز، إذا لم يكن هناك فائز بعد
    if (winner == EMPTY) {
        if (G_u8GameBoard[0][0] != EMPTY && G_u8GameBoard[0][0] == G_u8GameBoard[1][1] && G_u8GameBoard[1][1] == G_u8GameBoard[2][2]) {
            winner = G_u8GameBoard[1][1];
        } else if (G_u8GameBoard[0][2] != EMPTY && G_u8GameBoard[0][2] == G_u8GameBoard[1][1] && G_u8GameBoard[1][1] == G_u8GameBoard[2][0]) {
            winner = G_u8GameBoard[1][1];
        }
    }

    if (winner == PLAYER_X) return GAME_WIN_X;
    if (winner == PLAYER_O) return GAME_WIN_O;

    // التحقق من التعادل (إذا لم يكن هناك فائز واللوحة ممتلئة)
    for (u8 r = 0; r < 3; r++) {
        for (u8 c = 0; c < 3; c++) {
            if (G_u8GameBoard[r][c] == EMPTY) {
                return GAME_ONGOING; // اللعبة لم تنته بعد.
            }
        }
    }

    return GAME_DRAW; // إنها حالة تعادل.
}

/************************************************/
/* تنفيذ دوال عداد الفوز                  */
/************************************************/

void XO_voidIncrementWinCount(u8 Copy_u8Player) {
    if (Copy_u8Player == PLAYER_X) {
        G_u8PlayerX_Wins++;
    } else if (Copy_u8Player == PLAYER_O) {
        G_u8PlayerO_Wins++;
    }
}

u8 XO_u8GetWinCount(u8 Copy_u8Player) {
    if (Copy_u8Player == PLAYER_X) {
        return G_u8PlayerX_Wins;
    } else if (Copy_u8Player == PLAYER_O) {
        return G_u8PlayerO_Wins;
    }
    return 0;
}

void XO_voidResetWinCounters(void) {
    G_u8PlayerX_Wins = 0;
    G_u8PlayerO_Wins = 0;
}


u8 XO_u8GetCellState(u8 Copy_u8Row, u8 Copy_u8Col) {
    if (Copy_u8Row < 3 && Copy_u8Col < 3) {
        return G_u8GameBoard[Copy_u8Row][Copy_u8Col];
    }
    return EMPTY; // كإجراء وقائي
}
