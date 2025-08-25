#include "BIT_MATH.h"
#include "STD_TYPES.h"

#include "MSTK_Interface.h"
#include "MSTK_Private.h"
#include "MSTK_Config.h"


static volatile u32 G_u32TickCounter = 0;

void MSTK_voidInit()
{
	#if (STK_SYSTEM_CLK==STK_AHB_8)
		STK->CTRL&=~(1<<CLOCK_SOURCE);
	    STK->CTRL&=~(1<<STK_ENABLE);
	#elif (STK_SYSTEM_CLK==STK_AHB)
		STK->CTRL|=(1<<CLOCK_SOURCE);
	STK->CTRL&=~(1<<STK_ENABLE);
	#endif
}
void MSTK_voidStartTimer(u32 Copy_u32ReloadValue)
{
	STK->LOAD=Copy_u32ReloadValue;
	STK->VAL=0;
	STK->CTRL|=(1<<STK_ENABLE);
}
void MSTK_voidCtrlIntState(STK_IntState Copy_uddtIntState)
{
	STK->CTRL|=(Copy_uddtIntState<<TICKINT);
}
u8 MSTK_u8ReadFlag()
{
	return GET_BIT(STK->CTRL,COUNT_FLAG);
}
/*Function in Single shot*/
u32 MSTK_u32GetElapsedTickSingleShot()
{
	return ((STK->LOAD)-(STK->VAL));
}
u32 MSTK_u32GetRemainingTickSingleShot()
{
	return (STK->VAL);
}

void MSTK_voidDelayms(u32 Copy_u32Delayms)
{
	MSTK_voidCtrlIntState(Systick_IntDisable);
	MSTK_voidStartTimer(Copy_u32Delayms*2000);
	while(MSTK_u8ReadFlag()==0);
	STK->CTRL&=~(1<<STK_ENABLE);
}
void MSTK_voidDelayus(u32 Copy_u32Delayus)
{
	MSTK_voidCtrlIntState(Systick_IntDisable);
	MSTK_voidStartTimer(Copy_u32Delayus*2);
	while(MSTK_u8ReadFlag()==0);
	STK->CTRL&=~(1<<STK_ENABLE);
}

// في ملف MSTK_Program.c، أضف هذه الدالة في آخره
void MSTK_voidIncrementTicks(void)
{
    G_u32TickCounter++;
}

/**
 * @brief للحصول على القيمة الحالية لعداد التكات
 */
u32 MSTK_u32GetTicks(void)
{
    return G_u32TickCounter;
}

/**
 * @brief لحساب الوقت المنقضي بالـ "تكات" منذ نقطة زمنية معينة
 */
u32 MSTK_u32GetElapsedTime(u32 Copy_u32StartTime)
{
    u32 local_u32CurrentTime = G_u32TickCounter;

    if (local_u32CurrentTime >= Copy_u32StartTime)
    {
        return local_u32CurrentTime - Copy_u32StartTime;
    }
    else // في حالة حدوث overflow للعداد
    {
        return (0xFFFFFFFF - Copy_u32StartTime) + local_u32CurrentTime + 1;
    }
}
