#ifndef XO_LOGIC_H
#define XO_LOGIC_H

#include "STD_TYPES.h"

// تعريف حالات خانات اللوحة
#define PLAYER_X    1
#define PLAYER_O    2
#define EMPTY       0

// تعريف حالات اللعبة
#define GAME_ONGOING   0
#define GAME_WIN_X     1
#define GAME_WIN_O     2
#define GAME_DRAW      3

/**
 * @brief تهيئة لوحة اللعبة، مسح كل الخانات وتحديد اللاعب البادئ.
 */
void XO_voidInitBoard(void);

/**
 * @brief محاولة وضع علامة لاعب في مكان محدد على اللوحة.
 * @param Copy_u8Row الصف لوضع العلامة فيه (0-2).
 * @param Copy_u8Col العمود لوضع العلامة فيه (0-2).
 * @return 1 إذا كانت الحركة ناجحة، 0 إذا كانت الخانة مأخوذة بالفعل أو خارج الحدود.
 */
u8   XO_u8MakeMove(u8 Copy_u8Row, u8 Copy_u8Col);

/**
 * @brief التحقق من الحالة الحالية للعبة (فوز، تعادل، أو مستمرة).
 * @return حالة اللعبة الحالية (مثل GAME_WIN_X, GAME_DRAW).
 */
u8   XO_u8CheckGameState(void);

/**
 * @brief الحصول على اللاعب الحالي الذي حان دوره.
 * @return اللاعب الحالي (PLAYER_X أو PLAYER_O).
 */
u8   XO_u8GetCurrentPlayer(void);

/**
 * @brief تبديل الدور إلى اللاعب الآخر.
 */
void XO_voidSwitchPlayer(void);

/************************************************/
/* دوال عداد الفوز الجديدة                */
/************************************************/

/**
 * @brief زيادة عداد الفوز للاعب المحدد.
 * @param Copy_u8Player اللاعب الفائز (PLAYER_X أو PLAYER_O).
 */
void XO_voidIncrementWinCount(u8 Copy_u8Player);

/**
 * @brief الحصول على عدد مرات فوز لاعب معين.
 * @param Copy_u8Player اللاعب (PLAYER_X أو PLAYER_O).
 * @return عدد مرات الفوز.
 */
u8   XO_u8GetWinCount(u8 Copy_u8Player);

/**
 * @brief إعادة تعيين عدادات الفوز لكلا اللاعبين إلى الصفر.
 */
void XO_voidResetWinCounters(void);

/**
 * @brief الحصول على حالة خانة معينة على اللوحة.
 * @param Copy_u8Row الصف (0-2).
 * @param Copy_u8Col العمود (0-2).
 * @return حالة الخانة (PLAYER_X, PLAYER_O, أو EMPTY).
 */
u8 XO_u8GetCellState(u8 Copy_u8Row, u8 Copy_u8Col);

#endif
