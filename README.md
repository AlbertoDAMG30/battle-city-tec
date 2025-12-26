# Battle City - NES Classic Replica

![Battle City](assets/gameplay.png)

## 📋 Description

C implementation of the classic **Battle City** game from Nintendo Entertainment System (NES), developed as a project for the EL-3310 Digital Systems Design course at Costa Rica Institute of Technology (TEC).

The game replicates the first 5 levels of the original Battle City, including all enemy types, power-ups, and gameplay mechanics.

## 🎮 Implemented Features

### Player
- ✅ X and Y axis movement
- ✅ Shooting system with multiple levels
- ✅ Lives and respawn system
- ✅ Temporary protection shield
- ✅ Progressive tank upgrades (4 levels)

### Enemies
- ✅ **Basic Tank**: Basic tank with slow speed
- ✅ **Fast Tank**: Fast tank with increased speed
- ✅ **Power Tank**: Tank with powerful shots
- ✅ **Armor Tank**: Resistant tank (4 lives)
- ✅ Wave-based spawn system
- ✅ Red blinking variants (power-up drops)

### Power-ups
- 🎁 **Tank**: Extra life
- ⭐ **Star**: Shot upgrade
- 🛡️ **Helmet**: Temporary shield (5 seconds)
- 🔨 **Shovel**: Fortifies base (7 seconds)
- ⏰ **Timer**: Freezes enemies (6 seconds)
- 💣 **Grenade**: Eliminates all on-screen enemies

### Levels
- 🗺️ 5 unique levels with different layouts
- 🧱 Destructible blocks (bricks)
- 🔩 Indestructible blocks (steel)
- 🌳 Decorative blocks (trees)
- 🦅 Base to defend (eagle)

### Game System
- 🎯 Scoring system by enemy type
- 📊 Remaining enemies counter
- ❤️ Lives counter
- 🏆 Level transition screen with statistics
- 🎬 Start screen
- 💀 Game Over screen
- 🏅 Victory screen
- 🔄 Restart system

## 🛠️ Technologies Used

- **Language**: C (C11 standard)
- **Graphics library**: SDL2 (Simple DirectMedia Layer)
- **Operating System**: Linux
- **Compiler**: GCC
- **Build system**: Make
- **Version control**: Git

## 📦 Requirements

### System dependencies
```bash
# Ubuntu/Debian
sudo apt-get install build-essential libsdl2-dev

# Fedora/RHEL
sudo dnf install gcc make SDL2-devel

# Arch Linux
sudo pacman -S base-devel sdl2
```

## 🚀 Compilation and Execution

### Compile the project
```bash
make
```

### Run the game
```bash
./battle_city
```

### Clean compiled files
```bash
make clean
```

## 🎮 Controls

| Key | Action |
|-----|--------|
| `W` | Move up |
| `S` | Move down |
| `A` | Move left |
| `D` | Move right |
| `SPACE` | Shoot |
| `ENTER` | Restart game (on Game Over) |
| `ESC` | Quit game |

## 📂 Project Structure

```
battle-city-tec/
├── battle_city.c       # Main source code
├── Makefile           # Compilation script
├── README.md          # This file
├── Proyecto_1.pdf     # Project specifications (Spanish)
└── assets/            # Graphic resources
    ├── sprite1.bmp    # Main spritesheet
    ├── inicio.bmp     # Start screen
    ├── gameover.bmp   # Game Over screen
    └── youwin.bmp     # Victory screen
```

## 🎯 Scoring System

| Enemy | Points |
|-------|--------|
| Basic Tank | 100 pts |
| Fast Tank | 200 pts |
| Power Tank | 300 pts |
| Armor Tank | 400 pts |
| Power-up | 500 pts |

**Bonus**: Every 20,000 points = 1 extra life

## 🧩 Technical Features

### Data Structures
- Use of structures (`struct`) to represent game entities
- Dynamic memory management with `malloc` and `free`
- Linked lists for enemy management
- SDL_Rect-based collision system

### Code Architecture
- Modular system with specific functions
- Game state management
- Frame-based animation system
- Delta time for framerate-independent movement

### Memory Management
- Dynamic creation and destruction of enemies
- Correct resource deallocation
- Memory leak prevention
- No segmentation fault errors

## 🐛 Troubleshooting

### Game doesn't compile
- Verify SDL2 is correctly installed
- Make sure you're on Linux (not virtual machine)

### Sprites don't show
- Verify BMP files are in the same directory
- Confirm file names match the code

### Game runs too slow/fast
- Game is configured for 60 FPS
- Check your system has available resources

## 📖 Original Game Reference

- [Original Gameplay](https://www.youtube.com/watch?v=MPsA5PtfdL0)
- [Game Mechanics](https://strategywiki.org/wiki/Battle_City/Gameplay)

## 👨‍💻 Author

**David Alberto Mirandda Gonzalez**
- Student ID: 2020207762
- Course: EL-3310 Digital Systems Design
- Institution: Costa Rica Institute of Technology (TEC)
- Professor: Ernesto Rivera Alvarado

**Valeria Santamaria Vargas**
- Student ID: 2022138144
- Course: EL-3310 Digital Systems Design
- Institution: Costa Rica Institute of Technology (TEC)
- Professor: Ernesto Rivera Alvarado

## 📄 License

This project was developed for educational purposes for the Digital Systems Design course at Costa Rica Institute of Technology.

## 🙏 Acknowledgments

- Professor Ernesto Rivera Alvarado for project specifications and guidance
- Namco for the original Battle City game (1985)
- SDL community for excellent documentation

---

⭐ If you liked this project, give it a star on GitHub!
