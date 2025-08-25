# STM32 Game Console 

![STM32F401](https://img.shields.io/badge/STM32F401-Cortex--M4-03234B.svg)
![Bare Metal](https://img.shields.io/badge/Bare--Metal-No%20HAL-green.svg)
![Games](https://img.shields.io/badge/Games-Tic--Tac--Toe%20%26%20Snake-blue.svg)

A complete game console implementation running on STM32F401 microcontroller featuring two classic games: Tic-Tac-Toe and Snake. Developed using bare-metal drivers written from scratch during embedded systems training.

## 🎮 Features

- **Dual Game System**: Play both Tic-Tac-Toe and Snake on the same hardware
- **TFT Display**: 128x160 pixel RGB565 color display via SPI interface
- **User Input**: Push button controls for game navigation and gameplay
- **Sound Feedback**: Buzzer for audio feedback during gameplay
- **Bare Metal Implementation**: No HAL libraries - pure register-level programming
- **Modular Architecture**: Clean separation between hardware drivers and game logic

## 🛠️ Hardware Components

- **Microcontroller**: STM32F401CCU6 (ARM Cortex-M4 @ 84MHz)
- **Display**: TFT LCD (128x160 pixels, SPI interface)
- **Input**: Push buttons for user control
- **Audio**: Buzzer for sound effects
- **Power**: 3.3V regulated power supply

## 📁 Project Structure
<details open>
<summary><b>STM32_Game_Console/</b></summary>

<br>

```text
STM32_Game_Console/
├── Drivers/               # Bare-metal peripheral drivers
│   ├── MGPIO/             # GPIO control for buttons and signals
│   ├── MSPI/              # SPI communication for TFT display
│   ├── MRCC/              # Clock configuration and management
│   ├── MSTK/              # SysTick timer for game timing
│   └── HTFT/              # TFT display controller
│
├── Games/                 # Game implementations
│   ├── XO_Logic/          # Tic-Tac-Toe game logic
│   └── Snake_Logic/       # Snake game logic
│
├── System/                # System configuration
│   ├── Clock/             # System clock configuration
│   └── Memory/            # Memory management
│
└── Documentation/         # Project documentation
```
</details> 

## 🎯 Games Overview

### Tic-Tac-Toe (XO)
- 3x3 grid gameplay with two players (X and O)
- Win detection for rows, columns, and diagonals
- Draw detection when board is full
- Score tracking for multiple games

### Snake
- 12x14 grid gameplay (derived from 128x160 display)
- Direction control via push buttons
- Food generation and collision detection
- Score system based on food consumption
- Game over detection on wall or self collision

## 🔧 Technical Implementation

### System Architecture
- **Clock Configuration**: HSI oscillator at 16MHz
- **SPI Configuration**: Master mode with CPOL=0, CPHA=0
- **GPIO Setup**: Pull-up resistors for buttons, output for display control
- **Timing**: SysTick timer with AHB/8 clock source for precise delays

### Display System
- RGB565 color format support
- Rectangle drawing functions for game elements
- Dynamic window setting for efficient rendering
- Command/data writing via SPI interface

### Game Logic
- **Tic-Tac-Toe**: Board state management, move validation, win condition checking
- **Snake**: Movement algorithms, collision detection, food generation
- Both games feature state machines for smooth gameplay transitions

## 🚀 Getting Started

### Prerequisites
- STM32CubeIDE
- STM32F4xx DFP package
- ARM GCC toolchain
- STM32F401-based development board
- 128x160 TFT display with SPI interface
- Push buttons and buzzer

### Hardware Connections
| STM32F401 | Peripheral | Connection |
|-----------|------------|------------|
| PA5       | SPI1 SCK   | TFT SCK    |
| PA6       | SPI1 MISO  | TFT MISO   |
| PA7       | SPI1 MOSI  | TFT MOSI   |
| PA4       | TFT CS     | TFT CS     |
| PA3       | TFT DC     | TFT DC     |
| PA2       | TFT RST    | TFT RESET  |
| PB0-PB3   | Buttons    | Input      |
| PC13      | Buzzer     | Audio      |

### Building and Flashing
1. Clone the repository
2. Import into STM32CubeIDE as Makefile project
3. Set build variables for STM32F401 target
4. Build project
5. Flash to STM32F401 microcontroller

## 🎮 How to Play

1. **System Boot**: Console initializes and shows game selection menu
2. **Game Selection**: Use buttons to choose between Tic-Tac-Toe and Snake
3. **Tic-Tac-Toe**:
   - Navigate with direction buttons
   - Press select to place X/O
   - Game detects wins and draws automatically
4. **Snake**:
   - Use buttons to change direction (clockwise/counterclockwise)
   - Eat food to grow and increase score
   - Avoid walls and self-collision

## 📊 Performance Characteristics

- **Display Refresh**: Optimized SPI communication for smooth graphics
- **Input Response**: Immediate button response with GPIO interrupts
- **Game Timing**: Consistent frame timing using SysTick interrupts
- **Memory Usage**: Efficient heap management with custom _sbrk implementation

## 🔮 Future Enhancements

**Game Boy-style Cartridge System**  
Planned future development includes implementing a cartridge-based system similar to classic game consoles. This enhancement would allow users to:

- **ROM Cartridge Interface**: Physical cartridge slot for loading different games
- **File System Support**: FAT32 or custom filesystem for reading game ROMs from SD card
- **Bootloader System**: Custom bootloader to detect and load games from external media
- **Game Library Expansion**: Ability to add new games without reflashing the microcontroller
- **Save System**: Battery-backed SRAM or EEPROM for game save data persistence

This architecture would transform the console into a versatile gaming platform capable of running multiple games from external storage media, greatly expanding its capabilities beyond the current built-in games.

## 📝 Documentation

For detailed technical information, refer to:[
- [STM32F401 Reference Manual](https://www.st.com/content/ccc/resource/technical/document/reference_manual/5d/b1/ef/b2/a1/66/40/80/DM00096844.pdf/files/DM00096844.pdf/jcr:content/translations/en.DM00096844.pdf)
- [Cortex-M4 Technical Reference Manual](https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf)
- [TFT Display Datasheet](https://www.kingtechdisplay.com/uploads/file/20241218/pv104007r0240h.pdf)

## 🔗 Standalone Drivers Repository

The bare-metal drivers used in this project are available as a separate standalone library for use in other STM32 projects:

**[STM32 BareMetal Drivers Library](https://github.com/Yahia-Sameh/stm32f4xx-firmware-and-drivers-ARM-Cortex-M)**  

## 📜 License

This project is open source and available under the [MIT License](LICENSE).

---

**Note**: This is an educational project demonstrating embedded systems development. Some components may require optimization for production use.
