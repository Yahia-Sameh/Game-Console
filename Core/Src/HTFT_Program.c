#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "MGPIO_Interface.h"
#include "MSTK_Interface.h"
#include "MSPI_Interface.h"
#include "HTFT_Interface.h"
#include "HTFT_Config.h"

// ... (كود HTFT_voidInit, HTFT_voidWriteData, HTFT_voidWriteCMD الحالي الخاص بك يظل كما هو) ...

void HTFT_voidInit()
{
	MGPIO_voidSetMode(TFT_PORT,RST_PIN,OUTPUT);
	MGPIO_voidSetOutputConfig(TFT_PORT,RST_PIN,Push_Pull,Low_Speed);
	MGPIO_voidSetMode(TFT_PORT,CONTROL_PIN,OUTPUT);
	MGPIO_voidSetOutputConfig(TFT_PORT,CONTROL_PIN,Push_Pull,Low_Speed);
	
	//Reset Sequence 
	MGPIO_voidSetPinValue(TFT_PORT,RST_PIN,HIGH);
	MSTK_voidDelayus(100);
	MGPIO_voidSetPinValue(TFT_PORT,RST_PIN,LOW);
	MSTK_voidDelayus(1);
	MGPIO_voidSetPinValue(TFT_PORT,RST_PIN,HIGH);
	MSTK_voidDelayus(100);
	MGPIO_voidSetPinValue(TFT_PORT,RST_PIN,LOW);
	MSTK_voidDelayus(100);
	MGPIO_voidSetPinValue(TFT_PORT,RST_PIN,HIGH);
	MSTK_voidDelayms(120);
	
	//Sleep Out
	HTFT_voidWriteCMD(SLEEP_OUT);
	MSTK_voidDelayms(10);
	
	//Select color mode
	HTFT_voidWriteCMD(COLOR_MODE);
	HTFT_voidWriteData(RGB565);
	
	//Diplay on
	HTFT_voidWriteCMD(DISPLAY_ON);
}

void HTFT_voidWriteData(u8 Copy_u8Data)
{
	MGPIO_voidSetPinValue(TFT_PORT,CONTROL_PIN,HIGH);
	(void)MSPI_u16Tranceive(Copy_u8Data);
}

void HTFT_voidWriteCMD(u8 Copy_u8CMD)
{
	MGPIO_voidSetPinValue(TFT_PORT,CONTROL_PIN,LOW);
	(void)MSPI_u16Tranceive(Copy_u8CMD);
}

void HTFT_voidDisplay(const u16*Copy_pu16PTR)
{
	u16 Local_u16Iterator;
	u8 Local_u8HighPart,Local_u8LowPart;
	
	HTFT_voidSetWindow(0, 127, 0, 159);

	for(Local_u16Iterator=0;Local_u16Iterator<IMAGE_SIZE;Local_u16Iterator++)
	{
		Local_u8LowPart=(u8)Copy_pu16PTR[Local_u16Iterator];
		Local_u8HighPart=(u8)(Copy_pu16PTR[Local_u16Iterator]>>8);
		HTFT_voidWriteData(Local_u8HighPart);
		HTFT_voidWriteData(Local_u8LowPart);
	}
}

/************************************************/
/* تنفيذ دوال الرسم الجديدة              */
/************************************************/

void HTFT_voidSetWindow(u8 Copy_u8X0, u8 Copy_u8X1, u8 Copy_u8Y0, u8 Copy_u8Y1)
{
	// Set X address
	HTFT_voidWriteCMD(X_Direction);
	HTFT_voidWriteData(0);
	HTFT_voidWriteData(Copy_u8X0);
	HTFT_voidWriteData(0);
	HTFT_voidWriteData(Copy_u8X1);
	
	// Set Y address
	HTFT_voidWriteCMD(Y_Direction);
	HTFT_voidWriteData(0);
	HTFT_voidWriteData(Copy_u8Y0);
	HTFT_voidWriteData(0);
	HTFT_voidWriteData(Copy_u8Y1);
	
	// Start memory write command
	HTFT_voidWriteCMD(SCREEN_WRITE);
}

void HTFT_voidFillColor(u16 Copy_u16Color, u32 Copy_u32Size)
{
	u8 Local_u8HighPart = (u8)(Copy_u16Color >> 8);
	u8 Local_u8LowPart  = (u8)(Copy_u16Color);

	for(u32 Local_u32Iterator = 0; Local_u32Iterator < Copy_u32Size; Local_u32Iterator++)
	{
		HTFT_voidWriteData(Local_u8HighPart);
		HTFT_voidWriteData(Local_u8LowPart);
	}
}

void HTFT_voidFillRect(u8 Copy_u8X, u8 Copy_u8Y, u8 Copy_u8Width, u8 Copy_u8Height, u16 Copy_u16Color)
{
    u32 Local_u32Area = (u32)Copy_u8Width * Copy_u8Height;
    HTFT_voidSetWindow(Copy_u8X, Copy_u8X + Copy_u8Width - 1, Copy_u8Y, Copy_u8Y + Copy_u8Height - 1);
    HTFT_voidFillColor(Copy_u16Color, Local_u32Area);
}
