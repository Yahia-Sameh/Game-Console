#include "main.h"
#include "STD_TYPES.h"

#include "MRCC_Interface.h"
#include "MGPIO_Interface.h"
#include "MSTK_Interface.h"
#include "MSPI_Interface.h"
#include "HTFT_Interface.h"

#include "GUI_Main.h"

#define BUTTON1_PORT       PORTB
#define BUTTON1_PIN        PIN6
#define BUTTON2_PORT       PORTB
#define BUTTON2_PIN        PIN7
#define BUTTON3_PORT       PORTB
#define BUTTON3_PIN        PIN8

int main(void)
{
	/* 1. System Clocks Configuration */
	MRCC_voidInit();
	MRCC_voidEnablePeripheralClock(AHB1_BUS, AHB1_GPIOA);
	MRCC_voidEnablePeripheralClock(AHB1_BUS, AHB1_GPIOB);
	MRCC_voidEnablePeripheralClock(APB2_BUS, APB2_SPI1);

	/* 2. SysTick Initialization */
	MSTK_voidInit();

	/* 3. GPIO Pins Initialization */
	// SPI Pins for TFT
	MGPIO_voidSetMode(PORTA, PIN5, Alternative_Func); // SCK
	MGPIO_voidSetAlternativeConfig(PORTA, PIN5, AF5);
	MGPIO_voidSetMode(PORTA, PIN7, Alternative_Func); // MOSI
	MGPIO_voidSetAlternativeConfig(PORTA, PIN7, AF5);

    // Button Pins (PB6, PB7, PB8)
    MGPIO_voidSetMode(BUTTON1_PORT, BUTTON1_PIN, INPUT);
    MGPIO_voidSetInputConfig(BUTTON1_PORT, BUTTON1_PIN, PULLDOWN);

    MGPIO_voidSetMode(BUTTON2_PORT, BUTTON2_PIN, INPUT);
    MGPIO_voidSetInputConfig(BUTTON2_PORT, BUTTON2_PIN, PULLDOWN);

    MGPIO_voidSetMode(BUTTON3_PORT, BUTTON3_PIN, INPUT);
    MGPIO_voidSetInputConfig(BUTTON3_PORT, BUTTON3_PIN, PULLDOWN);

	/* 4. Peripherals Initialization (SPI & TFT) */
	MSPI_voidMasterInit();
	HTFT_voidInit();

    /* 5. Run the Main Game Menu */
	GUI_Main_voidRun();

    // The program should never reach here as GUI_Main_voidRun() is an infinite loop.
    while(1)
    {
    }
}
