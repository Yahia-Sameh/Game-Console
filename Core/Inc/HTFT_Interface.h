#ifndef HTFT_INTERFACE_H
#define HTFT_INTERFACE_H

#include "STD_TYPES.h"

void HTFT_voidInit(void);
void HTFT_voidWriteData(u8 Copy_u8Data);
void HTFT_voidWriteCMD(u8 Copy_u8CMD);
void HTFT_voidDisplay(const u16* Copy_pu16PTR);

/************************************************/
/* دوال الرسم الجديدة                */
/************************************************/

/**
 * @brief تحديد نافذة رسم مستطيلة على شاشة TFT.
 * @param Copy_u8X0 إحداثي X للبداية.
 * @param Copy_u8X1 إحداثي X للنهاية.
 * @param Copy_u8Y0 إحداثي Y للبداية.
 * @param Copy_u8Y1 إحداثي Y للنهاية.
 */
void HTFT_voidSetWindow(u8 Copy_u8X0, u8 Copy_u8X1, u8 Copy_u8Y0, u8 Copy_u8Y1);

/**
 * @brief ملء نافذة محددة مسبقًا بلون خالص.
 * @param Copy_u16Color قيمة اللون 16 بت (RGB565).
 * @param Copy_u32Size العدد الإجمالي للبكسلات المراد ملؤها.
 */
void HTFT_voidFillColor(u16 Copy_u16Color, u32 Copy_u32Size);

/**
 * @brief رسم مستطيل بلون خالص على الشاشة.
 * @param Copy_u8X إحداثي X للزاوية العلوية اليسرى للمستطيل.
 * @param Copy_u8Y إحداثي Y للزاوية العلوية اليسرى للمستطيل.
 * @param Copy_u8Width عرض المستطيل.
 * @param Copy_u8Height ارتفاع المستطيل.
 * @param Copy_u16Color قيمة اللون 16 بت (RGB565).
 */
void HTFT_voidFillRect(u8 Copy_u8X, u8 Copy_u8Y, u8 Copy_u8Width, u8 Copy_u8Height, u16 Copy_u16Color);

#endif
