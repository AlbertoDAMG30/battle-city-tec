#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // para manejar cadenas

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GAME_WIDTH 520      // Ancho del área de juego
#define GAME_HEIGHT 520     // Alto del área de juego
#define TILE_SIZE 20       // Tamaño de los cuadro de la matriz

#define MAP_WIDTH 26
#define MAP_HEIGHT 26

// Direcciones
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// Boleanos
#define TRUE 1
#define FALSE 0

//Enemigos
#define MAX_ENEMIGOS_SIMULTANEOS 4
#define TIEMPO_ENTRE_OLEADAS 2000 // 2 segundos en milisegundos

#define MAX_EXPLOSIONES 20

#define TECLA_REINICIO SDLK_RETURN

//Pantalla cambio nivel
#define CHAR_WIDTH 16
#define CHAR_HEIGHT 16
#define SPRITE_LETTERS_Y 992
#define SPRITE_NUMBERS_Y 1008

// Velocidad de las balas
float NormalBulletSpeed = 200.0f;
float FastBulletSpeed = 250.0f;
float SlowBulletSpeed = 150.0f;

// Velocidad de movimiento de los tanques
float slowSpeed = 80.0f;
float fastSpeed = 120.0f;
float normalSpeed = 100.0f;

// Variable global para controlar el tamaño del tanque
int TANK_SIZE = 31;

int mapOffsetX = 40; // Offset en X
int mapOffsetY = 40; // Offset en Y

// Variables globales
int enemigos_vivos = 20;
int puntosBasic = 0;
int puntosFast = 0;
int puntosArmor = 0;
int puntosPower = 0;
int puntosTotales = 0;

int puntosGlobales = 0;

int id_enemigo = 1;

// Para el power de tiempo
int enemigosCongelados = 0;
Uint32 tiempoDescongelar = 0;

// Tiempo de spawn inicial
Uint32 levelStartTime = 0;

// Definición de mapa 1
int mapa1[MAP_HEIGHT][MAP_WIDTH] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
	{2, 2, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 2, 2},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

int mapa2[MAP_HEIGHT][MAP_WIDTH] = {
	{0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 1, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0},
	{3, 3, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 1, 1, 3, 3, 1, 1, 2, 2},
	{3, 3, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 1, 1, 3, 3, 1, 1, 2, 2},
	{3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 2, 2, 0, 0, 3, 3, 0, 0, 0, 0},
	{3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 2, 2, 0, 0, 3, 3, 0, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 2, 2, 0, 0, 0, 0, 3, 3, 1, 1, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 2, 2, 0, 0, 0, 0, 3, 3, 1, 1, 2, 2},
	{0, 0, 0, 0, 0, 0, 2, 2, 3, 3, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 2, 2, 3, 3, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{2, 2, 1, 1, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
	{2, 2, 1, 1, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 5, 5, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 5, 5, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
};

int mapa3[MAP_HEIGHT][MAP_WIDTH] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 3, 3, 3, 3, 3, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 3, 3, 3, 3, 3, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2},
	{1, 1, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
	{3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
	{3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0},
	{3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0},
	{3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
	{3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
	{0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 3, 0, 0},
	{0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 3, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3},
	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3},
	{1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3},
	{1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3},
	{1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 0, 0},
	{1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0},
	{1, 1, 1, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0},
	{1, 1, 1, 1, 0, 0, 2, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0},
	{2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
	{2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
};

int mapa4[MAP_HEIGHT][MAP_WIDTH] = {
	{0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0},
	{0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0},
	{3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3},
	{3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 3, 3},
	{3, 3, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 2, 2},
	{3, 3, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
	{2, 2, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0},
	{4, 4, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{4, 4, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 4, 4, 4, 4, 4},
	{0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 4, 4, 4, 4, 4},
	{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 3, 3},
	{0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 3, 3},
	{3, 3, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 3, 3, 3, 3},
	{3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3},
	{2, 2, 3, 3, 0, 0, 0, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0, 0, 0, 0, 3, 3, 3, 3, 2, 2},
	{2, 2, 3, 3, 0, 0, 0, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0, 0, 0, 0, 3, 3, 3, 3, 2, 2},
};

int mapa5[MAP_HEIGHT][MAP_WIDTH] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0},
	{2, 2, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0},
	{2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 4, 4, 4, 4, 0, 0, 4, 4},
	{1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 4, 4},
	{1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 1, 1, 1, 1},
	{0, 0, 0, 0, 1, 1, 0, 0, 4, 4, 4, 4, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 0, 0, 4, 4, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 1, 1, 0, 0, 0, 0, 4, 4, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0},
	{4, 4, 4, 4, 4, 4, 0, 0, 4, 4, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0},
	{4, 4, 4, 4, 4, 4, 0, 0, 4, 4, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
	{1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *tileset = NULL;

//Variables globales para el cambio de nivel
int currentLevel = 1;
int (*currentMap)[MAP_WIDTH] = mapa1;

// ====================
// Tipos de enemigos
// ====================
typedef enum {
    BASIC_TANK,  // 0
    FAST_TANK,    // 1
    ARMOR_TANK,   // 2
    POWER_TANK    // 3
} EnemyType;

// ====================
// Spawn Points
// ====================

typedef struct spawn_node {
    int fila;
    int columna;
    EnemyType tipo;  
    Uint32 tiempoAparicion;
    struct spawn_node *next;
} SpawnNode;

SpawnNode *spawn_list = NULL;

typedef struct {
    int count;
    EnemyType type;
} EnemyWave;

// ====================================================
// Estructura Jugador (único elemento, no dinámico)
// ====================================================
typedef struct {
    SDL_Rect rect;      // Posición y tamaño del tanque
    int vida;           // Cantidad de vidas
    int powerup;        // Código de powerup (si aplica)
    int direccion;      // 0: arriba, 1: abajo, 2: izquierda, 3: derecha
    int nivelDisparo; // 0 = normal, 1 = balas rápidas, 2 = doble bala, 3 = balas destructoras
    int escudoActivo;
    Uint32 tiempoInicioEscudo;    // Tiempo en el que se activó el escudo
    int escudoFrame;          // Frame actual de la animación
    Uint32 tiempoUltimoFrameEscudo; // Tiempo del último cambio de frame
    int respawnando;          // 1 si está en tiempo de respawn
    Uint32 tiempoRespawn;     // Tiempo en que inició el respawn
    int respawnFrame;                // <- ESTE DEBE ESTAR
    Uint32 tiempoUltimoFrameRespawn;// <- Y ESTE TAMBIÉN
} Jugador;

Jugador jugador = { {220, 520, 28, 28}, 3, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};

// ====================
// Estructura Bloque
// ====================
typedef struct {
    int type;      // 0: vacio, 1: descritible, 2: indestructible
    int row, col;  
	int dureza;    // Cantidad de balas necesarias para destruirlo
    int impacto;
    SDL_Rect rect; // Rectángulo en pantalla para este bloque
} Bloque;

Bloque **bloques = NULL;
int totalBloques = 0;

// ====================
// Estructura Bala
// ====================
typedef struct {
    SDL_Rect rect;      // Posición y tamaño de la bala
    float velX, velY;   // Velocidad (dirección y magnitud)
    int direccion;      // 0: arriba, 1: abajo, 2: izquierda, 3: derecha
} Bala;

Bala **balas = NULL;
int totalBalas = 0;

typedef struct {
    SDL_Rect rect;      // Posición y tamaño de la bala
    float velX, velY;   // Velocidad (dirección y magnitud)
    int direccion;      // 0: arriba, 1: abajo, 2: izquierda, 3: derecha
    float speed;
} Bala_enemy;

Bala_enemy **balas_enemigas = NULL;
int totalBalasEnemigas = 0;

// ====================
// Lista de enemigos
// ====================

typedef struct {
    SDL_Rect rect;
    EnemyType tipo;           // BASIC_TANK, FAST_TANK, etc.
    int vida;
    float velocidad;
    int direccion;
    float dx, dy;       // Dirección actual
    Uint32 ultimoCambio; // Tiempo del último cambio de dirección
    Uint32 ultimaColision;
    Uint32 ultimoDisparo;
    int id;
    int colisionesSpawn;
    int ignorarSpawn;
    int enSpawn;
} enemy_data;

typedef struct enemies {
	enemy_data enemy;
	struct enemies *next;
    Uint32 ultimoCambio;
} enemies;

enemies *lista_enemigos = NULL;


// ====================
// Estructura power up 
// ====================

typedef struct {
    SDL_Rect rect;      // Posición y tamaño 
    int tipo;      
    int activo; // 1: Activo, 0: Recogido  
} PowerUp;

PowerUp **powerUps = NULL;
int totalPowerUps = 0;

//Especial para hacer el power up pala
typedef struct {
    int fila;
    int columna;
} BloqueTransformado;

BloqueTransformado *bloquesTransformados = NULL;
int totalTransformados = 0;


Uint32 tiempoPowerUpPala = 0;
int palaActiva = 0;

// ====================
// Estructura para colision con la explosion  
// ====================
typedef struct {
    SDL_Rect posicion;
    Uint32 tiempoInicio;
    int frame;
    Uint32 ultimoCambioFrame;
} Explosion;

Explosion explosiones[MAX_EXPLOSIONES];
int totalExplosiones = 0;

// ====================
// Estructura para spawn 
// ====================
typedef struct SpawnAnimation {
    int fila, columna;     // Posición en el mapa
    EnemyType tipo;         // Tipo de enemigo
    int x, y;               // Posición en pantalla
    Uint32 startTime;       // Tiempo de inicio
    struct SpawnAnimation *next;
} SpawnAnimation;

SpawnAnimation *activeSpawnAnimations = NULL;

void dibujarAnimacionRespawn(SDL_Renderer *renderer, SDL_Texture *tileset, Jugador *jugador) {
    Uint32 tiempoActual = SDL_GetTicks();

    if (tiempoActual - jugador->tiempoUltimoFrameRespawn > 200) {
        jugador->respawnFrame = (jugador->respawnFrame + 1) % 6;
        jugador->tiempoUltimoFrameRespawn = tiempoActual;
    }

    SDL_Rect src = {
        255 + jugador->respawnFrame * 32,  // Frame horizontal
        32,                                // Línea vertical
        32, 32
    };

    SDL_Rect dest = {
        jugador->rect.x + jugador->rect.w / 2 - 15,  // Centrado horizontal
        jugador->rect.y + jugador->rect.h / 2 - 15,  // Centrado vertical
        32,
        32
    };

    SDL_RenderCopy(renderer, tileset, &src, &dest);
}

// ====================
// Variables para bloques destruidos
// ====================
SDL_Rect *destroyedBlockRects = NULL;
int totalDestroyedBlocks = 0;

void agregarBloqueDestruido(SDL_Rect rect) {
    totalDestroyedBlocks++;
    destroyedBlockRects = (SDL_Rect*) realloc(destroyedBlockRects, totalDestroyedBlocks * sizeof(SDL_Rect));
    if (!destroyedBlockRects) {
        fprintf(stderr, "Error al asignar memoria para bloques destruidos\n");
        exit(1);
    }
    destroyedBlockRects[totalDestroyedBlocks - 1] = rect;
}

// ====================
// Funciones para bloques
// ====================
Bloque* crearBloque(int row, int col, int type, int x, int y, int w, int h) {
    Bloque *bloque = (Bloque*) malloc(sizeof(Bloque));
    if (!bloque) {
        fprintf(stderr, "Error al crear bloque en (%d, %d)\n", row, col);
        exit(1);
    }
    bloque->type = type;
    bloque->row = row;
    bloque->col = col;
    bloque->rect.x = x;
    bloque->rect.y = y;
    bloque->rect.w = w;
    bloque->rect.h = h;
	if (type == 1) {
        bloque->dureza = 2;
    } else {
        bloque->dureza = 1; 
    }
    return bloque;
}

void initBloques(int (*map)[MAP_WIDTH]) {
    int contador = 0;
    
    // Primera pasada: contar bloques necesarios
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (map[i][j] == 0) {
                contador += 1;
            }
            else if (map[i][j] == 1 || map[i][j] == 2 || map[i][j] == 3 || map[i][j] == 4|| map[i][j] == 5) {
                contador++;
            }
        }
    }

    // Asignar memoria para los bloques
    totalBloques = contador;
    bloques = (Bloque**)malloc(totalBloques * sizeof(Bloque*));
    if (!bloques) {
        fprintf(stderr, "Error al asignar memoria para bloques\n");
        exit(1);
    }

    // Segunda pasada: crear los bloques
    int index = 0;
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            int x = mapOffsetX + j * TILE_SIZE;
            int y = mapOffsetY + i * TILE_SIZE;
            
            switch(map[i][j]) {
                case 0:
                    bloques[index++] = crearBloque(i, j, 0, x, y, TILE_SIZE, TILE_SIZE);
                    break;
                case 1:
                    bloques[index++] = crearBloque(i, j, 1, x, y, TILE_SIZE, TILE_SIZE);
                    break;
                case 2:
                    bloques[index++] = crearBloque(i, j, 2, x, y, TILE_SIZE, TILE_SIZE);
                    break;
                case 3:
                    bloques[index++] = crearBloque(i, j, 3, x, y, TILE_SIZE, TILE_SIZE);
                    break;
                case 4:
                    bloques[index++] = crearBloque(i, j, 4, x, y, TILE_SIZE, TILE_SIZE);
                    break;
                case 5:  // Nuevo caso para el bloque del águila
                    bloques[index++] = crearBloque(i, j, 5, x, y, TILE_SIZE, TILE_SIZE);
                    break;
            }
        }
    }
}

void removeBloque(int index, Bloque ***bloques, int *totalBloques) {
    if (index < 0 || index >= *totalBloques) return;
    free((*bloques)[index]);
    for (int i = index; i < *totalBloques - 1; i++) {
        (*bloques)[i] = (*bloques)[i + 1];
    }
    (*totalBloques)--;
    *bloques = (Bloque**) realloc(*bloques, (*totalBloques) * sizeof(Bloque*));
}

void destroyBloques() {
    for (int i = 0; i < totalBloques; i++) {
        free(bloques[i]);
    }
    free(bloques);
}

//Para transformar el bloque con un power up
void convertirBloquesEspecificos() {
    int posiciones[][2] = {
        {23, 11}, {24, 11}, {25, 11}, // Columna 11
        {23, 14}, {24, 14}, {25, 14}, // Columna 14
        {23, 12}, {23, 13}            // Fila 23
    };

    int cantidad = 8;
    totalTransformados = 0;

    // Reservar espacio para los bloques transformados
    bloquesTransformados = (BloqueTransformado*) malloc(cantidad * sizeof(BloqueTransformado));
    if (!bloquesTransformados) {
        fprintf(stderr, "Error al asignar memoria para bloques transformados\n");
        exit(1);
    }

    // Iterar sobre la lista de bloques y transformar los necesarios
    for (int j = 0; j < cantidad; j++) {
  // Iteramos sobre las 8 posiciones deseadas
        for (int i = 0; i < totalBloques; i++) {  // Iteramos sobre todos los bloques del mapa
            //Si son la columna y la fila es la misma del arreglo posiciones
            if (bloques[i]->row == posiciones[j][0] && 
                bloques[i]->col == posiciones[j][1]) {  

                // Convertimos el bloque en indestructible (2)
                bloques[i]->type = 2;

                // Guardar en la lista de transformados
                bloquesTransformados[totalTransformados].fila = posiciones[j][0];
                bloquesTransformados[totalTransformados].columna = posiciones[j][1];
                totalTransformados++;
            }
        }
    }
}

void restaurarBloques() {

    for (int i = 0; i < totalTransformados; i++) {
        int row = bloquesTransformados[i].fila;
        int col = bloquesTransformados[i].columna;

        for (int j = 0; j < totalBloques; j++) {
            if (bloques[j]->row == row && bloques[j]->col == col && bloques[j]->type == 2) {
                bloques[j]->type = 1; // Restaurar a su estado original
                bloques[j]->dureza = 2; //Si tenia daño lo restaura
            }
        }
    }
    
    //Libera los bloques transformados y limpia
    free(bloquesTransformados);
    bloquesTransformados = NULL;
    totalTransformados = 0;
    palaActiva = 0;
}

//====================
// Funciones para el dibujar la explosion
// ====================

void dibujarExplosiones(SDL_Renderer *renderer, SDL_Texture *tileset) {
    Uint32 tiempoActual = SDL_GetTicks();

    for (int i = 0; i < totalExplosiones;) {
        Uint32 tiempoTranscurrido = tiempoActual - explosiones[i].tiempoInicio;

        // Cambiar de frame cada 150ms
        if (tiempoActual - explosiones[i].ultimoCambioFrame > 150) {
            explosiones[i].frame = !explosiones[i].frame;
            explosiones[i].ultimoCambioFrame = tiempoActual;
        }

        // Eliminar explosión después de 1 segundo
        if (tiempoTranscurrido >= 200) {
            for (int j = i; j < totalExplosiones - 1; j++) {
                explosiones[j] = explosiones[j + 1];
            }
            totalExplosiones--;//Reduce el arreglo de explosiones
            continue; // No incrementes i si eliminaste
        }

        SDL_Rect srcExplosion;
        if (explosiones[i].frame == 0) {
            srcExplosion = (SDL_Rect){288, 64, 32, 32}; // Frame 1
        } else {
            srcExplosion = (SDL_Rect){320, 64, 32, 32}; // Frame 2
        }

        SDL_RenderCopy(renderer, tileset, &srcExplosion, &explosiones[i].posicion);
        i++;
    }
}

void agregarExplosion(SDL_Rect colisionRect) {
    if (totalExplosiones >= MAX_EXPLOSIONES) return;

    Explosion nueva;//El rand es para se vea mas "centrado"
    nueva.posicion.x = colisionRect.x + (rand() % 6 - 12);
    nueva.posicion.y = colisionRect.y + (rand() % 6 - 12);
    nueva.posicion.w = 32;
    nueva.posicion.h = 32;

    nueva.tiempoInicio = SDL_GetTicks();
    nueva.frame = 0;
    nueva.ultimoCambioFrame = SDL_GetTicks();

    explosiones[totalExplosiones++] = nueva;
}


// ==============================
// Funciones para balas enemigas
// ==============================

Bala_enemy* crearBalaEnemiga(int x, int y, float velX, float velY) {
    Bala_enemy *bala_enemiga = (Bala_enemy*) malloc(sizeof(Bala_enemy));
    if (!bala_enemiga) {
        fprintf(stderr, "Error al crear bala Enemiga\n");
        exit(1);
    }
    bala_enemiga->rect.x = x-7;
    bala_enemiga->rect.y = y-3;
    bala_enemiga->rect.w = 21;
    bala_enemiga->rect.h = 21;
    bala_enemiga->velX = velX;
    bala_enemiga->velY = velY;
    bala_enemiga->direccion = 0;
    bala_enemiga->speed = 200.0f;
    return bala_enemiga;
}

void agregarBalaEnemiga(Bala_enemy* bala_enemiga) {
    totalBalasEnemigas++;
    Bala_enemy **temp = (Bala_enemy**) realloc(balas_enemigas, totalBalasEnemigas * sizeof(Bala_enemy*));
    if (!temp) {
        fprintf(stderr, "Error al reasignar memoria para balas enemigas\n");
        free(bala_enemiga);  // Liberar la bala enemiga recién creada
        exit(EXIT_FAILURE);
    }
    balas_enemigas = temp;
    balas_enemigas[totalBalasEnemigas - 1] = bala_enemiga;
}

void removeBalaEnemiga(int index, Bala_enemy ***balas_enemigas, int *totalBalasEnemigas) {
    if (index < 0 || index >= *totalBalasEnemigas) return;
    
    free((*balas_enemigas)[index]);
    
    for (int i = index; i < *totalBalasEnemigas - 1; i++) {
        (*balas_enemigas)[i] = (*balas_enemigas)[i + 1];
    }

    (*totalBalasEnemigas)--;

    if (*totalBalasEnemigas == 0) {
        free(*balas_enemigas);
        *balas_enemigas = NULL;
    } else {
        Bala_enemy **temp = realloc(*balas_enemigas, (*totalBalasEnemigas) * sizeof(Bala_enemy*));
        if (temp) {
            *balas_enemigas = temp;
        }
    }
}

void destroyBalasEnemigas() {
    if (balas_enemigas) {
        for (int i = 0; i < totalBalasEnemigas; i++) {
            free(balas_enemigas[i]);
        }
        free(balas_enemigas);
        balas_enemigas = NULL;
        totalBalasEnemigas = 0; // Asegura que no haya acceso posterior
    }
}


int gestionarColisionBalaEnemigaBloque(Bala_enemy *bala_enemiga, Bloque ***bloques, int *totalBloques) {
    int golpe = 0;  // Variable para indicar si hubo una colisión
    int bloquesAEliminar[*totalBloques]; // Array temporal para marcar bloques a eliminar
    int totalAEliminar = 0; // Contador de bloques a eliminar
    
    for (int j = 0; j < *totalBloques; j++) {
        Bloque *bloque = (*bloques)[j];

        // Verificar colisión con cualquier bloque (destructible o indestructible)
        if (bloque &&
            bala_enemiga->rect.x < bloque->rect.x + bloque->rect.w &&
            bala_enemiga->rect.x + bala_enemiga->rect.w > bloque->rect.x &&
            bala_enemiga->rect.y < bloque->rect.y + bloque->rect.h &&
            bala_enemiga->rect.y + bala_enemiga->rect.h > bloque->rect.y) {
            
            if (bloque->type == 1) { // Si es destructible
                // Detectamos la dirección del impacto basada en la dirección de la bala
                int impacto = -1;

                switch (bala_enemiga->direccion) {
                    case 0: impacto = 0; break; // Disparo arriba
                    case 1: impacto = 1; break; // Disparo abajo
                    case 2: impacto = 2; break; // Disparo izquierda
                    case 3: impacto = 3; break; // Disparo derecha
                }

                bloque->impacto = impacto;  // Asignamos el tipo de impacto
                bloque->dureza--; 

                if (bloque->dureza <= 0) {
                    bloquesAEliminar[totalAEliminar++] = j; 
                }  
                golpe = 1; 
            }
            if (bloque->type == 2) { // Si es un bloque tipo 2 (indestructible)
                golpe = 1;  // Indicar colisión pero no destruir
            }

            if (bloque->type == 5) { // Si es un bloque tipo 5 (aguila)
                printf("¡El águila fue destruida! El jugador pierde todas las vidas.\n");
                jugador.vida = 0; // Eljugador se queda sin vidas
                return 1; // Indica que la bala debe desaparecer
            }
            if (golpe) {
                SDL_Rect
                explosionRect = {
                    bala_enemiga->rect.x, bala_enemiga->rect.y,
                    jugador.rect.w,
                    jugador.rect.h
               };
                agregarExplosion(explosionRect);
            }
        }
    }

    // Segundo ciclo: Eliminar los bloques que fueron marcados
    for (int k = totalAEliminar - 1; k >= 0; k--) {
        agregarBloqueDestruido((*bloques)[bloquesAEliminar[k]]->rect);
        removeBloque(bloquesAEliminar[k], bloques, totalBloques);
    }

    return golpe; // Devuelve si hubo colisión o no
}

void respawnJugador() {
    jugador.rect.x = 220;
    jugador.rect.y = 520;
    jugador.direccion = 0;
    if (jugador.escudoActivo) {
        jugador.escudoFrame = 0; // Reiniciar animación
        jugador.tiempoUltimoFrameEscudo = SDL_GetTicks();
    }
}

void procesarBalasEnemigas(Bala_enemy ***balas_enemigas, int *totalBalasEnemigas, Bloque ***bloques, int *totalBloques, float deltaTime) {
    for (int i = 0; i < *totalBalasEnemigas; ) {
        Bala_enemy *bala_enemiga = (*balas_enemigas)[i];
        if (!bala_enemiga) {
            i++;
            continue;
        }

        // Movimiento de la bala
        float vx = 0, vy = 0;
        switch (bala_enemiga->direccion) {
            case 0: vy = -1; break;
            case 1: vy = 1; break;
            case 2: vx = -1; break;
            case 3: vx = 1; break;
        }
        float norm = sqrt(vx * vx + vy * vy);
        if (norm == 0) norm = 1;
        vx = (vx / norm) * bala_enemiga->speed;
        vy = (vy / norm) * bala_enemiga->speed;

        bala_enemiga->rect.x += vx * deltaTime;
        bala_enemiga->rect.y += vy * deltaTime;

        int colisiono = 0;

        // Colisión con bloques
        if (gestionarColisionBalaEnemigaBloque(bala_enemiga, bloques, totalBloques)) {
            removeBalaEnemiga(i, balas_enemigas, totalBalasEnemigas);
            colisiono = 1;
        }

        if (!colisiono) {
            // Colisión con bordes del mapa
            if (bala_enemiga->rect.x < mapOffsetX || 
                bala_enemiga->rect.x + bala_enemiga->rect.w > mapOffsetX + GAME_WIDTH ||
                bala_enemiga->rect.y < mapOffsetY || 
                bala_enemiga->rect.y + bala_enemiga->rect.h > mapOffsetY + GAME_HEIGHT) {
                
                SDL_Rect explosionRect = { bala_enemiga->rect.x, bala_enemiga->rect.y, jugador.rect.w, jugador.rect.h };
                agregarExplosion(explosionRect);
                removeBalaEnemiga(i, balas_enemigas, totalBalasEnemigas);
                colisiono = 1;
            }
        }

        if (!colisiono) {
            // Colisión con el jugador
            if (SDL_HasIntersection(&bala_enemiga->rect, &jugador.rect)) {
                if (!jugador.escudoActivo) {
                    jugador.vida--;
                    jugador.respawnando = 1;
                    jugador.tiempoRespawn = SDL_GetTicks();
                    respawnJugador();
                }
                SDL_Rect explosionRect = { bala_enemiga->rect.x, bala_enemiga->rect.y, jugador.rect.w, jugador.rect.h };
                agregarExplosion(explosionRect);
                removeBalaEnemiga(i, balas_enemigas, totalBalasEnemigas);
                colisiono = 1;
            }
        }

        if (colisiono) {
            // Ya se eliminó la bala, no incrementar i
        } else {
            i++;
        }
    }
}

void dibujarBalasEnemigas(SDL_Renderer *renderer, Bala_enemy **balas_enemigas, int totalBalasEnemigas) {
    SDL_Rect balaSrcRect = {350, 433, 12, 12}; 
    for (int i = 0; i < totalBalasEnemigas; i++) {
        SDL_Rect destino = {
            balas_enemigas[i]->rect.x,
            balas_enemigas[i]->rect.y,
            balas_enemigas[i]->rect.w,
            balas_enemigas[i]->rect.h
        };

        SDL_RenderCopy(renderer, tileset, &balaSrcRect, &destino);
    }
}

// ====================
// Funciones para power-ups
// ====================

PowerUp* crearPowerUp(int (*map)[MAP_WIDTH]) {
    int x, y;
    int intentos = 0;
    
    // Buscar una posición libre en el mapa actual
    do {
        x = rand() % MAP_WIDTH;
        y = rand() % MAP_HEIGHT;
        intentos++;
    } while (map[y][x] != 0 && intentos < 100); // Solo en espacios vacíos

    if (intentos >= 100) return NULL; // No se encontró espacio libre

    PowerUp *p = (PowerUp*) malloc(sizeof(PowerUp));// Reservar memoria para el power up
    if (!p) {
        fprintf(stderr, "Error al crear power up\n");
        return NULL;
    }

    p->rect.x = mapOffsetX + x * TILE_SIZE;
    p->rect.y = mapOffsetY + y * TILE_SIZE;
    p->rect.w = TANK_SIZE;
    p->rect.h = TANK_SIZE;
    p->tipo = rand() % 6;// 6 tipos de power up
    p->activo = 1;

    return p;
}

int gestionarColisionBalaEnemigo(Bala *bala, enemies **lista) {
    enemies *actual = *lista;
    enemies *prev = NULL;
    int colision = 0;

    while (actual && !colision) {
        if (SDL_HasIntersection(&bala->rect, &actual->enemy.rect)) {
            actual->enemy.vida--;
            
            if (actual->enemy.vida <= 0) { // Eliminar enemigo                
                // Sumar puntos según tipo
                switch (actual->enemy.tipo) {
                    case BASIC_TANK:
                        puntosBasic += 100;
                        puntosGlobales +=100;
                        break;
                    case FAST_TANK:
                        puntosFast += 200;
                        puntosGlobales += 200;
                        break;
                    case ARMOR_TANK:
                        puntosArmor += 400;
                        puntosGlobales += 400;
                        break;
                    case POWER_TANK:
                        puntosPower += 300;
                        puntosGlobales += 300;
                        break;
                    default:
                        break;
                }
                puntosTotales = puntosBasic + puntosFast + puntosArmor + puntosPower;

                if(puntosGlobales >= 20000){
                    puntosGlobales = 0;
                    jugador.vida++;
                }

                // Agregar power-up si el ID es el indicado
                if (actual->enemy.id == 4 || actual->enemy.id == 11 || actual->enemy.id == 18) {
                    PowerUp *nuevoPU = crearPowerUp(currentMap);
                    if (nuevoPU != NULL) {
                        totalPowerUps++;
                        powerUps = realloc(powerUps, totalPowerUps * sizeof(PowerUp*));
                        powerUps[totalPowerUps - 1] = nuevoPU;
                    }
                }

                if (prev) prev->next = actual->next;
                else *lista = actual->next;
                
                enemies *to_remove = actual;
                actual = actual->next;
                free(to_remove);
            } else { // Solo daño
                prev = actual;
                actual = actual->next;
            }
            // Crear explosión en 
            SDL_Rect explosionRect = {
                bala->rect.x,
                bala->rect.y,
                jugador.rect.w,
                jugador.rect.h
            };
            agregarExplosion(explosionRect);
            colision = 1;
        } else {
            prev = actual;
            actual = actual->next;
        }
    }
    return colision;
}

//====================
// Funciones para el dibujar el escudo
// ====================

void dibujarEscudo(SDL_Renderer *renderer, SDL_Texture *tileset, Jugador *jugador) {
    if (jugador->escudoActivo && SDL_GetTicks() >= levelStartTime) {
        // Cambiar de frame cada 100 ms
        if (SDL_GetTicks() - jugador->tiempoUltimoFrameEscudo > 100) {
            jugador->escudoFrame = !jugador->escudoFrame; // Alternar entre 0 y 1
            jugador->tiempoUltimoFrameEscudo = SDL_GetTicks();
        }
        
        SDL_Rect srcEscudo;
        if (jugador->escudoFrame) {
            srcEscudo = (SDL_Rect){289, 0, 32, 32}; // Segundo frame
        } else {
            srcEscudo = (SDL_Rect){256, 0, 32, 32}; // Primer frame
        }
        
        SDL_Rect dstEscudo = {
            jugador->rect.x - 3,
            jugador->rect.y - 3,
            jugador->rect.w + 10,
            jugador->rect.h + 10 //para que se vea al rededor del tanque
        };
        
        SDL_RenderCopy(renderer, tileset, &srcEscudo, &dstEscudo);
    }
}

//====================
// Funciones para el dibujar letras y numeros
// ====================

void dibujarCaracter(SDL_Renderer *renderer, SDL_Texture *sprite1, char c, int x, int y) {
    SDL_Rect src, dst;
    dst.x = x;
    dst.y = y;
    dst.w = CHAR_WIDTH;
    dst.h = CHAR_HEIGHT;

    //Nos da la posición del caracter en el sprite
    if (c >= 'A' && c <= 'Z') {
        src.x = (c - 'A') * CHAR_WIDTH;
        src.y = SPRITE_LETTERS_Y;
    } else if (c >= '0' && c <= '9') {
        src.x = (c - '0') * CHAR_WIDTH;
        src.y = SPRITE_NUMBERS_Y;
    } else {
        return; // no dibujar caracteres no soportados
    }

    src.w = CHAR_WIDTH;
    src.h = CHAR_HEIGHT;
    SDL_RenderCopy(renderer, sprite1, &src, &dst);
}

void dibujarTexto(SDL_Renderer *renderer, SDL_Texture *sprite1, const char* texto, int x, int y) {
    if (!texto) return; 
    //Dibuja cada carácter uno al lado del otro.
    for (int i = 0; texto[i] != '\0'; i++) {
        dibujarCaracter(renderer, sprite1, texto[i], x + i * CHAR_WIDTH, y);
    }
}

void mostrarTransicionNivel(SDL_Renderer *renderer, SDL_Texture *sprite1, int nivel) {
    //Borra la pantalla a negro
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Dibujar texto "NIVEL" y el número
    dibujarTexto(renderer, sprite1, "NIVEL", 200, 200);
    dibujarTexto(renderer, sprite1, "PTS", 400, 270);
    dibujarTexto(renderer, sprite1, "PTS", 400, 320);
    dibujarTexto(renderer, sprite1, "PTS", 400, 370);
    dibujarTexto(renderer, sprite1, "PTS", 400, 420);

     //Para el numero de nivel, de 1 a 4
    char numeroStr[4];
    snprintf(numeroStr, sizeof(numeroStr), "%d", nivel);
    dibujarTexto(renderer, sprite1, numeroStr, 300, 200); 

    char buffer[32];//Para escribir hasta 32 caracteres
    sprintf(buffer, "BASIC: %d", puntosBasic);
    dibujarTexto(renderer, sprite1, buffer, 200, 270);

    sprintf(buffer, "FAST: %d", puntosFast);
    dibujarTexto(renderer, sprite1, buffer, 200, 320);

    sprintf(buffer, "POWER: %d", puntosPower);
    dibujarTexto(renderer, sprite1, buffer, 200, 370);

    sprintf(buffer, "ARMOR: %d", puntosArmor);
    dibujarTexto(renderer, sprite1, buffer, 200, 420);
    // Mostrar total
    sprintf(buffer, "TOTAL: %d", puntosTotales);
    dibujarTexto(renderer, sprite1, buffer, 300, 500);

    sprintf(buffer, "TOTAL JUGADOR: %d", puntosGlobales);
    dibujarTexto(renderer, sprite1, buffer, 200, 100);


    // Dibujar decoración
    //posicion y tamaño en el sprite
    SDL_Rect decoracionSrc = {0, 425, 32, 32};
     //posicion y tamaño en la pantalla
    SDL_Rect decoracionDst = {460, 250, 42, 42};
    SDL_RenderCopy(renderer, sprite1, &decoracionSrc, &decoracionDst);

    SDL_Rect decoracion2Src = {0, 554, 32, 32};
    SDL_Rect decoracion2Dst = {460, 310, 42, 42};
    SDL_RenderCopy(renderer, sprite1, &decoracion2Src, &decoracion2Dst);

    SDL_Rect decoracion3Src = {0, 682, 32, 32};
    SDL_Rect decoracion3Dst = {460, 360, 42, 42};
    SDL_RenderCopy(renderer, sprite1, &decoracion3Src, &decoracion3Dst);

    SDL_Rect decoracion4Src = {0, 810, 32, 32};
    SDL_Rect decoracion4Dst = {460, 410, 42, 42};
    SDL_RenderCopy(renderer, sprite1, &decoracion4Src, &decoracion4Dst);

    SDL_RenderPresent(renderer);
    SDL_Delay(3000); // esperar 3 segundos
}


// ====================
// Funciones para balas jugador
// ====================
int balaActiva = 0;

Bala* crearBala(int x, int y, float velX, float velY) {
    Bala *bala = (Bala*) malloc(sizeof(Bala));
    if (!bala) {
        fprintf(stderr, "Error al crear bala\n");
        exit(1);
    }
    bala->rect.x = x-7;
    bala->rect.y = y-3;
    bala->rect.w = 21;
    bala->rect.h = 21;
    bala->velX = velX;
    bala->velY = velY;
    bala->direccion = 0;
    return bala;
}

void agregarBala(Bala* bala) {
    totalBalas++;
    Bala **temp = (Bala**) realloc(balas, totalBalas * sizeof(Bala*));
    if (!temp) {
        fprintf(stderr, "Error al reasignar memoria para balas\n");
        free(bala);  // Liberar la bala recién creada
        exit(EXIT_FAILURE);
    }
    balas = temp;
    balas[totalBalas - 1] = bala;
}

void removeBala(int index, Bala ***balas, int *totalBalas) {
    if (index < 0 || index >= *totalBalas) return;
    free((*balas)[index]);
    for (int i = index; i < *totalBalas - 1; i++) {
        (*balas)[i] = (*balas)[i + 1];
    }
    (*totalBalas)--;
    *balas = (Bala**) realloc(*balas, (*totalBalas) * sizeof(Bala*));
}

void destroyBalas() {
    for (int i = 0; i < totalBalas; i++) {
        free(balas[i]);
    }
    free(balas);
}

// ====================
// Función para gestionar la colisión de la bala con el bloque
// ====================
int gestionarColisionBalaBloque(Bala *bala, Bloque ***bloques, int *totalBloques) {
    int golpe = 0;  // Variable para indicar si hubo una colisión
    int bloquesAEliminar[*totalBloques]; // Array temporal para marcar bloques a eliminar
    int totalAEliminar = 0; // Contador de bloques a eliminar

    // Primer ciclo: Detectar colisiones con todos los bloques
    for (int j = 0; j < *totalBloques; j++) {
        Bloque *bloque = (*bloques)[j];

        // Verificar colisión con cualquier bloque (destructible o indestructible)
        if (bloque &&
            bala->rect.x < bloque->rect.x + bloque->rect.w &&
            bala->rect.x + bala->rect.w > bloque->rect.x &&
            bala->rect.y < bloque->rect.y + bloque->rect.h &&
            bala->rect.y + bala->rect.h > bloque->rect.y) {

            if (bloque->type == 1) { // Pared destructible
                // Detectamos la dirección del impacto basada en la dirección de la bala
                int impacto = -1;

                switch (bala->direccion) {
                    case 0: impacto = 0; break; // Disparo arriba
                    case 1: impacto = 1; break; // Disparo abajo
                    case 2: impacto = 2; break; // Disparo izquierda
                    case 3: impacto = 3; break; // Disparo derecha
                }

                bloque->impacto = impacto;  // Asignamos el tipo de impacto

                // Aplicar la dureza del bloque
                if (jugador.nivelDisparo == 3) { 
                    bloque->dureza -= 2; // Con Nivel 3, 2 disparos destruyen la pared
                    
                } else {
                    bloque->dureza--; // Normalmente, 4 disparos la destruyen
                }

                // Si la dureza es 0 o menos, marcar el bloque para eliminación
                if (bloque->dureza <= 0) {
                    bloquesAEliminar[totalAEliminar++] = j; 
                }
                golpe = 1;  // Indicar que hubo colisión, pero seguir verificando más bloques
            }

            if (bloque->type == 2) { // Si es un bloque tipo 2 (indestructible)
                if (jugador.nivelDisparo == 3) { 
                    bloque->dureza -= 1; // Con Nivel 3, 2 disparos destruyen la pared  
                } 
                
                if (bloque->dureza <= 0) {
                    bloquesAEliminar[totalAEliminar++] = j; 
                }
                golpe = 1;
            }

            if (bloque->type == 5) { // Si es un bloque tipo 5 (aguila)
                jugador.vida = 0; // El jugador se queda sin vidas
                return 1; // Indica que la bala debe desaparecer
            }

            if (golpe) {
                SDL_Rect explosionRect = {
                    bala->rect.x, bala->rect.y,
                    jugador.rect.w, jugador.rect.h
                };
                agregarExplosion(explosionRect);
            }
            
        }
    }
    // Segundo ciclo: Eliminar los bloques que fueron marcados
    for (int k = totalAEliminar - 1; k >= 0; k--) {
        agregarBloqueDestruido((*bloques)[bloquesAEliminar[k]]->rect);
        removeBloque(bloquesAEliminar[k], bloques, totalBloques);
    }

    return golpe; // Devuelve si hubo colisión o no
}

// ====================
// Función para gestionar la colisión de la bala del jugador con la bala del enemigo
// ====================
void gestionarColisionBalasJugadorVsEnemigas(Bala ***balasJugador, int *totalBalasJugador, Bala_enemy ***balasEnemigas, int *totalBalasEnemigas) {
    for (int i = 0; i < *totalBalasJugador; ) {
        int colisionDetectada = 0;
        for (int j = 0; j < *totalBalasEnemigas; ) {
            if (SDL_HasIntersection(&(*balasJugador)[i]->rect, &(*balasEnemigas)[j]->rect)) {
                // Crear explosión
                SDL_Rect explosionRect = {
                    ((*balasJugador)[i]->rect.x + (*balasEnemigas)[j]->rect.x) / 2,
                    ((*balasJugador)[i]->rect.y + (*balasEnemigas)[j]->rect.y) / 2,
                    jugador.rect.w,
                    jugador.rect.h
                };
                agregarExplosion(explosionRect);

                // Eliminar ambas balas
                removeBala(i, balasJugador, totalBalasJugador);
                removeBalaEnemiga(j, balasEnemigas, totalBalasEnemigas);
                colisionDetectada = 1;
                break; // Salir del bucle interno
            } else {
                j++; // Solo incrementar si no hubo eliminación
            }
        }
        if (!colisionDetectada) {
            i++; // Solo incrementar si no se eliminó la bala del jugador
        }
    }
}


// ====================
// Función fusionada: calcula dirección de la bala y la mueve
// ====================
void procesarBalas(Bala ***balas, int *totalBalas, Bloque ***bloques, int *totalBloques, float deltaTime) {
    for (int i = 0; i < *totalBalas; ) {
        Bala *bala = (*balas)[i];
        float vx = 0, vy = 0;
        switch (bala->direccion) {
            case 0: vy = -1; break;
            case 1: vy = 1; break;
            case 2: vx = -1; break;
            case 3: vx = 1; break;
        }
        float norm = sqrt(vx * vx + vy * vy);
        if (norm == 0) norm = 1;
        vx = (vx / norm) * NormalBulletSpeed;
        vy = (vy / norm) * NormalBulletSpeed;
        bala->velX = vx;
        bala->velY = vy;
        bala->rect.x += vx * deltaTime;
        bala->rect.y += vy * deltaTime;
     

        // Verificar colisión con los bordes del mapa
        if (bala->rect.x < mapOffsetX || bala->rect.x > GAME_WIDTH+mapOffsetX||
            bala->rect.y < mapOffsetY || bala->rect.y > GAME_HEIGHT+mapOffsetY) {

            
            // Crear explosión en el punto donde la bala salió del mapa
            SDL_Rect explosionRect = {
                bala->rect.x,
                bala->rect.y,
                jugador.rect.w,
                jugador.rect.h
            };
            agregarExplosion(explosionRect);
            
            removeBala(i, balas, totalBalas);
			balaActiva = 0;
            continue;
        }

        int colisiono = 0;
        if (gestionarColisionBalaBloque(bala, bloques, totalBloques) || gestionarColisionBalaEnemigo(bala,&lista_enemigos)){
            removeBala(i, balas, totalBalas);
            balaActiva = 0; 
			colisiono = 1;
        }



        if (!colisiono) {
            i++;
        }
    }
}

// ======================
// Funcion para enemigos
// ======================

void actualizarDireccionSprite(enemy_data *enemigo) {
    // Movimiento exclusivamente horizontal o vertical
    if (enemigo->dx != 0) {
        enemigo->direccion = (enemigo->dx > 0) ? RIGHT : LEFT;
    } else {
        enemigo->direccion = (enemigo->dy > 0) ? DOWN : UP;
    }
}

void setup_spawn_list_for_level(int level) {
    // Limpiar lista existente
    id_enemigo = 1;

    while (spawn_list != NULL) {
        SpawnNode *temp = spawn_list;
        spawn_list = spawn_list->next;
        free(temp);
    }

    EnemyWave waves[10];
    int num_waves = 0;

    // Definir oleadas para cada nivel
    switch (level) {
        case 1:
            waves[0] = (EnemyWave){18, BASIC_TANK};
            waves[1] = (EnemyWave){2, FAST_TANK};
            num_waves = 2;
            break;
        case 2:
            waves[0] = (EnemyWave){2, ARMOR_TANK};
            waves[1] = (EnemyWave){4, FAST_TANK};
            waves[2] = (EnemyWave){14, BASIC_TANK};
            num_waves = 3;
            break;
        case 3:
            waves[0] = (EnemyWave){14, BASIC_TANK};
            waves[1] = (EnemyWave){4, FAST_TANK};
            waves[2] = (EnemyWave){2, ARMOR_TANK};
            num_waves = 3;
            break;
        case 4:
            waves[0] = (EnemyWave){10, POWER_TANK};
            waves[1] = (EnemyWave){5, FAST_TANK};
            waves[2] = (EnemyWave){2, BASIC_TANK};
            waves[3] = (EnemyWave){3, ARMOR_TANK};
            num_waves = 4;
            break;
        case 5:
            waves[0] = (EnemyWave){5, POWER_TANK};
            waves[1] = (EnemyWave){2, ARMOR_TANK};
            waves[2] = (EnemyWave){8, BASIC_TANK};
            waves[3] = (EnemyWave){5, FAST_TANK};
            num_waves = 4;
            break;
        default:
            return;
    }

    // Calcular total de enemigos para el nivel y actualizar enemigos_vivos
    int total_enemigos = 0;
    for (int i = 0; i < num_waves; i++) {
        total_enemigos += waves[i].count;
    }
    enemigos_vivos = total_enemigos;

    // Resto del código para crear nodos de spawn.
    int spawn_pattern[3][2] = {{0,12}, {0,25}, {0,0}};
    int spawn_index = 0;
    SpawnNode **current = &spawn_list;

    for (int i = 0; i < num_waves; i++) {
        for (int j = 0; j < waves[i].count; j++) {
            SpawnNode *new_node = malloc(sizeof(SpawnNode));
            new_node->fila = spawn_pattern[spawn_index % 3][0];
            new_node->columna = spawn_pattern[spawn_index % 3][1];
            new_node->tipo = waves[i].type;
            new_node->tiempoAparicion = 0;
            new_node->next = NULL;

            *current = new_node;
            current = &(*current)->next;
            spawn_index++;
        }
    }
}

void add_enemy(enemies **lista, int x, int y, EnemyType tipo) {
    if (lista == NULL) {
        fprintf(stderr, "Error: Puntero a la lista de enemigos no inicializado.\n");
        return;
    }

    enemies *nuevo = malloc(sizeof(enemies));
    if (!nuevo) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el enemigo.\n");
        return;
    }

    if (tipo < BASIC_TANK || tipo > POWER_TANK) {
        fprintf(stderr, "Error: Tipo de enemigo inválido (%d)\n", tipo);
        return;
    }
    
    // Inicializar el enemigo
    nuevo->enemy.id =id_enemigo;
    nuevo->enemy.rect = (SDL_Rect){x, y, TANK_SIZE, TANK_SIZE};
    nuevo->enemy.ultimoCambio = SDL_GetTicks();
    nuevo->enemy.ultimaColision = 0;
    nuevo->enemy.ultimoDisparo = SDL_GetTicks(); // Inicializar timer de disparo
    nuevo->enemy.tipo = tipo;
    nuevo->enemy.dx = (rand() % 2 == 0) ? 1 : -1;
    nuevo->enemy.dy = (rand() % 2 == 0) ? 1 : -1;
    nuevo->enemy.colisionesSpawn = 0;
    nuevo->enemy.ignorarSpawn = 0;
    nuevo->enemy.enSpawn = 0;
    actualizarDireccionSprite(&nuevo->enemy);

        // Configurar propiedades según tipo
        switch(tipo) {
            case BASIC_TANK:
                nuevo->enemy.velocidad = slowSpeed;
                nuevo->enemy.vida = 1;
                break;
            case FAST_TANK:
                nuevo->enemy.velocidad = fastSpeed;
                nuevo->enemy.vida = 1;
                break;
            case ARMOR_TANK:
                nuevo->enemy.velocidad = normalSpeed;
                nuevo->enemy.vida = 4;
                break;
            case POWER_TANK:
                nuevo->enemy.velocidad = normalSpeed;
                nuevo->enemy.vida = 1;
                break;
            default:
                fprintf(stderr, "Error: Tipo de enemigo desconocido.\n");
                free(nuevo);
                return;
    }
    id_enemigo++;
    nuevo->next = *lista;
    *lista = nuevo;
}

void free_enemies(enemies *lista) {
    while (lista) {
        enemies *temp = lista;
        lista = lista->next;
        free(temp);
    }
}

int frame_count = 0;
void dibujarEnemigo(SDL_Renderer *renderer, SDL_Texture *tileset, enemy_data enemy) {

    // Sprites para cada tipo de enemigo y dirección
    SDL_Rect spriteEnemigo[8][4] = {
        // BASIC_TANK 
        {
            {0,  424, TANK_SIZE, TANK_SIZE},   // Arriba
            {0, 488, TANK_SIZE, TANK_SIZE},    // Abajo
            {0, 520, TANK_SIZE, TANK_SIZE},    // Izquierda
            {0, 456, TANK_SIZE, TANK_SIZE}     // Derecha
        },
        // FAST_TANK 
        {
            {0, 552, TANK_SIZE, TANK_SIZE},   // Arriba
            {0, 616, TANK_SIZE, TANK_SIZE},   // Abajo
            {0, 648, TANK_SIZE, TANK_SIZE},   // Izquierda
            {0, 584, TANK_SIZE, TANK_SIZE}    // Derecha
        },
        // ARMOR_TANK 
        {
            {322, 810, TANK_SIZE, TANK_SIZE},  // Arriba
            {322, 874, TANK_SIZE, TANK_SIZE},  // Abajo
            {322, 906, TANK_SIZE, TANK_SIZE},  // Izquierda
            {322, 842, TANK_SIZE, TANK_SIZE}   // Derecha
        },
        // POWER_TANK
        {
            {0, 680, TANK_SIZE, TANK_SIZE},  // Arriba
            {0, 744, TANK_SIZE, TANK_SIZE},  // Abajo
            {0, 776, TANK_SIZE, TANK_SIZE},  // Izquierda
            {0, 712, TANK_SIZE, TANK_SIZE}   // Derecha
        },

        // RED-BASIC_TANK 
        {
            {96,  424, TANK_SIZE, TANK_SIZE},   // Arriba
            {96, 488, TANK_SIZE, TANK_SIZE},    // Abajo
            {96, 520, TANK_SIZE, TANK_SIZE},    // Izquierda
            {96, 456, TANK_SIZE, TANK_SIZE}     // Derecha
        },
        // RED-FAST_TANK 
        {
            {96, 552, TANK_SIZE, TANK_SIZE},   // Arriba
            {96, 616, TANK_SIZE, TANK_SIZE},   // Abajo
            {96, 648, TANK_SIZE, TANK_SIZE},   // Izquierda
            {96, 584, TANK_SIZE, TANK_SIZE}    // Derecha
        },
        // RED-ARMOR_TANK 
        {
            {96, 810, TANK_SIZE, TANK_SIZE},  // Arriba
            {96, 874, TANK_SIZE, TANK_SIZE},  // Abajo
            {96, 906, TANK_SIZE, TANK_SIZE},  // Izquierda
            {96, 842, TANK_SIZE, TANK_SIZE}   // Derecha
        },
        // RED-POWER_TANK 
        {
            {96, 680, TANK_SIZE, TANK_SIZE},  // Arriba
            {96, 744, TANK_SIZE, TANK_SIZE},  // Abajo
            {96, 776, TANK_SIZE, TANK_SIZE},  // Izquierda
            {96, 712, TANK_SIZE, TANK_SIZE}   // Derecha
        }
    };

    // Validar tipo de enemigo
    if (enemy.tipo < BASIC_TANK || enemy.tipo > POWER_TANK) {
        enemy.tipo = BASIC_TANK; // Default a BASIC_TANK si hay error
    }

    // Alternar entre dos sprites cada 4 frames para los enemigos con id 4, 11, 18
    if (enemy.id == 4 || enemy.id == 11 || enemy.id == 18) {
        // Alternar el sprite cada 4 frames
        if (frame_count / 30 % 2 == 0) {
            // Usamos el primer sprite del tipo normal
            SDL_Rect src = spriteEnemigo[enemy.tipo][enemy.direccion];
            SDL_RenderCopy(renderer, tileset, &src, &enemy.rect);
        } else {
            // Usamos el segundo sprite alternativo (rojo)
            SDL_Rect src = spriteEnemigo[4 + enemy.tipo][enemy.direccion]; // Usamos el sprite "rojo" basado en el tipo
            SDL_RenderCopy(renderer, tileset, &src, &enemy.rect);
        }
    } else {
        // Para los enemigos normales, usamos el sprite estándar
        SDL_Rect src = spriteEnemigo[enemy.tipo][enemy.direccion];
        SDL_RenderCopy(renderer, tileset, &src, &enemy.rect);
    }

    // Incrementar el contador de frames
    frame_count++;
}

void place_enemy(int i, int j, EnemyType tipo) {
    int x = mapOffsetX + j * TILE_SIZE;
    int y = mapOffsetY + i * TILE_SIZE;
    
    // Nueva posición inicial aleatoria dentro de un rango
    x += rand() % 16 - 8;
    y += rand() % 16 - 8;
    
    // Nueva verificación de posición válida
    int intentos = 0;
    SDL_Rect testRect = {x, y, TANK_SIZE, TANK_SIZE};
    int colision = TRUE;
    
    while (colision && intentos < 10) {
        colision = FALSE;
        
        // Verificar con bloques
        for (int i = 0; i < totalBloques; i++) {
            if (bloques[i]->type != 0 && SDL_HasIntersection(&testRect, &bloques[i]->rect)) {
                colision = TRUE;
                break;
            }
        }
        
        // Verificar con otros enemigos
        if (!colision) {
            enemies *temp = lista_enemigos;
            while (temp != NULL) {
                if (SDL_HasIntersection(&testRect, &temp->enemy.rect)) {
                    colision = TRUE;
                    break;
                }
                temp = temp->next;
            }
        }
        
        if (colision) {
            x = mapOffsetX + j * TILE_SIZE + rand() % 32 - 16;
            y = mapOffsetY + i * TILE_SIZE + rand() % 32 - 16;
            testRect.x = x;
            testRect.y = y;
            intentos++;
        }
    }
    
    add_enemy(&lista_enemigos, x, y, tipo);
    enemigos_vivos--;
}

void dibujarAnimacionSpawn(SDL_Renderer *renderer, SDL_Texture *tileset) {
    SpawnAnimation **current = &activeSpawnAnimations;
    Uint32 currentTime = SDL_GetTicks();

    while (*current != NULL) {
        Uint32 elapsed = currentTime - (*current)->startTime;

        // Dibujar animación
        int frame = (elapsed / 100) % 6;
        SDL_Rect src = {255 + frame * 32, 32, 32, 32};
        SDL_Rect dest = {(*current)->x - 6, (*current)->y - 6, 32, 32};
        SDL_RenderCopy(renderer, tileset, &src, &dest);

        // Spawnear el enemigo después de 2 segundos (2000ms)
        if (elapsed >= 2000) {
            // Verificar si hay un enemigo en la zona de spawn
            SDL_Rect areaSpawn = {(*current)->x, (*current)->y, TANK_SIZE+30, TANK_SIZE+30};
            int hayColision = 0;

            enemies *temp = lista_enemigos;
            while (temp != NULL) {
                if (SDL_HasIntersection(&areaSpawn, &temp->enemy.rect)) {
                    hayColision = 1;
                    break;
                }
                temp = temp->next;
            }

            if (!hayColision) {
                // Crear el enemigo solo si no hay colisión
                place_enemy((*current)->fila, (*current)->columna, (*current)->tipo);

                // Eliminar la animación
                SpawnAnimation *tempAnim = *current;
                *current = (*current)->next;
                free(tempAnim);
                continue;  // Ya avanzamos el puntero, no más current = ...
            }
        }

        current = &(*current)->next;
    }
}

int colisionEnemigo(SDL_Rect rect, enemy_data *enemigoActual) {

    // Verificar colisión con los bordes del mapa
    SDL_Rect bordes = {
        mapOffsetX, 
        mapOffsetY, 
        MAP_WIDTH * TILE_SIZE, 
        MAP_HEIGHT * TILE_SIZE
    };
    
    if (rect.x < bordes.x ||
        rect.y < bordes.y ||
        rect.x + rect.w > bordes.x + bordes.w ||
        rect.y + rect.h > bordes.y + bordes.h) {
        return TRUE;
    }

    // Revisa colisión con bloques
    for (int i = 0; i < totalBloques; i++) {
        if (bloques[i]->type != 0 && SDL_HasIntersection(&rect, &bloques[i]->rect) && bloques[i]->type != 3) 
            return TRUE;
    }

    // Revisa colisión con otros enemigos
    enemies *temp = lista_enemigos;
    while (temp != NULL) {
        if (&temp->enemy != enemigoActual) { 
            if (SDL_HasIntersection(&rect, &temp->enemy.rect))
                return TRUE;
        }
        temp = temp->next;
    }

    if (SDL_HasIntersection(&rect, &jugador.rect)) {
        if(!jugador.escudoActivo){
            jugador.vida --;
            jugador.respawnando = 1;
            jugador.tiempoRespawn = SDL_GetTicks();
            jugador.rect.x = 220;
            jugador.rect.y = 520;
            SDL_Rect explosionRect = {
                enemigoActual->rect.x,
                enemigoActual->rect.y,
                jugador.rect.w,
                jugador.rect.h
            };
            agregarExplosion(explosionRect);
            respawnJugador();
            return TRUE;
        }
    }

    return FALSE;
}

// Nuevo sistema de direcciones prioritarias
void cambiarDireccionSistematica(enemy_data *enemigo) {
    // Orden de prioridad: mantener dirección actual, luego alternativas
    static int direcciones[4][3] = {
        {UP, RIGHT, LEFT},    // Si venía de UP
        {DOWN, RIGHT, LEFT},  // Si venía de DOWN
        {LEFT, DOWN, UP},     // Si venía de LEFT
        {RIGHT, UP, DOWN}     // Si venía de RIGHT
    };
    
    // Intentar direcciones en orden de prioridad
    for (int i = 0; i < 3; i++) {
        int nuevaDir = direcciones[enemigo->direccion][i];
        
        // Calcular delta para la nueva dirección
        float testDx = 0, testDy = 0;
        switch(nuevaDir) {
            case UP: testDy = -1; break;
            case DOWN: testDy = 1; break;
            case LEFT: testDx = -1; break;
            case RIGHT: testDx = 1; break;
        }
        
        // Verificar si la nueva dirección es viable
        SDL_Rect testRect = enemigo->rect;
        testRect.x += testDx * enemigo->velocidad * 0.1; // Pequeño movimiento
        testRect.y += testDy * enemigo->velocidad * 0.1;
        
        if (!colisionEnemigo(testRect, enemigo)) {
            enemigo->dx = testDx;
            enemigo->dy = testDy;
            enemigo->direccion = nuevaDir;
            return;
        }
    }
    
    // Si todas fallan, dirección opuesta
    enemigo->dx *= -1;
    enemigo->dy *= -1;
    actualizarDireccionSprite(enemigo);
}

void disparoEnemigo(enemy_data *enemigo) {
    if (enemigosCongelados) return;// No disparar si están congelados
     // Calcular posición central del enemigo
    int centerX = enemigo->rect.x + enemigo->rect.w/2 - 2;
    int centerY = enemigo->rect.y + enemigo->rect.h/2 - 2;
    
    // Crear bala con dirección actual del enemigo
    Bala_enemy *nuevaBalaEnemiga = crearBalaEnemiga(centerX, centerY, 0, 0);
    nuevaBalaEnemiga->direccion = enemigo->direccion;
    
    // Asignar velocidad según tipo de enemigo
    switch(enemigo->tipo) {
        case BASIC_TANK:    nuevaBalaEnemiga->speed = SlowBulletSpeed; break; // Lenta
        case FAST_TANK:     nuevaBalaEnemiga->speed = NormalBulletSpeed; break; // Normal
        case POWER_TANK:    nuevaBalaEnemiga->speed = FastBulletSpeed; break; // Rápida
        case ARMOR_TANK:    nuevaBalaEnemiga->speed = NormalBulletSpeed; break; // Normal
        default:            nuevaBalaEnemiga->speed = NormalBulletSpeed; break;
    }
    
    agregarBalaEnemiga(nuevaBalaEnemiga);
    enemigo->ultimoDisparo = SDL_GetTicks();// Reiniciar time
}

void moverEnemigoIA(enemy_data *enemigo, float deltaTime) {
    if (!enemigo) return;
    if (enemigosCongelados) return; // No mover si están congelados

    SDL_Rect bordes = {
        mapOffsetX, 
        mapOffsetY, 
        MAP_WIDTH * TILE_SIZE, 
        MAP_HEIGHT * TILE_SIZE
    };

    // Lógica de movimiento periódico
    if ((SDL_GetTicks() - enemigo->ultimoCambio) > (Uint32)(2000 + rand() % 3000)) {
        // Cambiar a dirección ortogonal aleatoria
        if (enemigo->dx != 0) { // Si se movía horizontal
            enemigo->dx = 0;
            enemigo->dy = (rand() % 2 == 0) ? 1 : -1;
        } else { // Si se movía vertical
            enemigo->dy = 0;
            enemigo->dx = (rand() % 2 == 0) ? 1 : -1;
        }
        enemigo->ultimoCambio = SDL_GetTicks();
        cambiarDireccionSistematica(enemigo);
    }

    // Calcular nueva posición
    SDL_Rect nuevaPos = enemigo->rect;
    nuevaPos.x += enemigo->dx * enemigo->velocidad * deltaTime;
    nuevaPos.y += enemigo->dy * enemigo->velocidad * deltaTime;

    // Manejar colisiones
    if (!colisionEnemigo(nuevaPos, enemigo)) {
        enemigo->rect = nuevaPos;
    } else {
        // Retroceder y cambiar dirección ortogonal
        enemigo->rect.x -= enemigo->dx * enemigo->velocidad * deltaTime;
        enemigo->rect.y -= enemigo->dy * enemigo->velocidad * deltaTime;
        
        // Cambiar dirección perpendicularmente
        if (enemigo->dx != 0) {
            enemigo->dx = 0;
            enemigo->dy = (rand() % 2 == 0) ? 1 : -1;
        } else {
            enemigo->dy = 0;
            enemigo->dx = (rand() % 2 == 0) ? 1 : -1;
        }
        
        // Forzar nuevo cambio de dirección pronto
        enemigo->ultimoCambio = SDL_GetTicks() - 1;
        cambiarDireccionSistematica(enemigo);
    }

    // Mantener dentro de los bordes del mapa
    if (enemigo->rect.x < bordes.x) {
        enemigo->rect.x = bordes.x;
        enemigo->dx = 1;
        enemigo->dy = 0;
    }
    else if (enemigo->rect.x + enemigo->rect.w > bordes.x + bordes.w) {
        enemigo->rect.x = bordes.x + bordes.w - enemigo->rect.w;
        enemigo->dx = -1;
        enemigo->dy = 0;
    }

    if (enemigo->rect.y < bordes.y) {
        enemigo->rect.y = bordes.y;
        enemigo->dy = 1;
        enemigo->dx = 0;
    }
    else if (enemigo->rect.y + enemigo->rect.h > bordes.y + bordes.h) {
        enemigo->rect.y = bordes.y + bordes.h - enemigo->rect.h;
        enemigo->dy = -1;
        enemigo->dx = 0;
    }

    // Disparar con menor frecuencia
    if ((SDL_GetTicks() - enemigo->ultimoDisparo) > 4000) {
        disparoEnemigo(enemigo);
    }

    // Actualizar sprite según dirección
    cambiarDireccionSistematica(enemigo);
}

// ====================
// Funciones SDL
// ====================
int initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error al inicializar SDL: %s\n", SDL_GetError());
        return 0;
    }
    return 1;
}

SDL_Window* createWindow() {
    SDL_Window *window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Error al crear la ventana: %s\n", SDL_GetError());
        return NULL;
    }
    return window;
}  

SDL_Renderer* createRenderer(SDL_Window *window) {
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Error al crear el renderer: %s\n", SDL_GetError());
        return NULL;
    }
    return renderer;
}

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (!surface) {
        printf("Error al cargar la imagen %s: %s\n", path, SDL_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Error al crear textura desde %s: %s\n", path, SDL_GetError());
        SDL_FreeSurface(surface);
        return NULL;
    }

    SDL_FreeSurface(surface);
    return texture;
}


SDL_Texture* cargarPantallaInicio();
void mostrarPantallaInicio(SDL_Texture* pantallaInicio);

SDL_Texture* cargarPantallaInicio() {
    SDL_Surface* screenSurface = SDL_LoadBMP("inicio.bmp"); // Imagen de la pantalla de inicio
    if (!screenSurface) {
        printf("Error cargando imagen de inicio: %s\n", SDL_GetError());
        return NULL;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screenSurface);
    SDL_FreeSurface(screenSurface);
    return texture;
}

void mostrarPantallaInicio(SDL_Texture* pantallaInicio) {
    SDL_Event e;
    int enPantallaInicio = 1;
    
    while(enPantallaInicio) {
        // Dibujar pantalla de inicio
        SDL_RenderCopy(renderer, pantallaInicio, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        // Esperar eventos
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                exit(0);
            }
            if(e.type == SDL_KEYDOWN) { // Cualquier tecla
                enPantallaInicio = 0;
                break;
            }
        }
        
        SDL_Delay(10); // Reducir uso de CPU
    }
    
    // Limpiar textura
    SDL_DestroyTexture(pantallaInicio);
}


SDL_Texture* cargarPantallaGameOver();
void mostrarPantallaGameOver(SDL_Texture* pantallaGameOver);
void reinicializarJuego();

// Función para cargar la textura de Game Over
SDL_Texture* cargarPantallaGameOver() {
    SDL_Surface* screenSurface = SDL_LoadBMP("gameover.bmp"); 
    if (!screenSurface) {
        printf("Error cargando imagen de game over: %s\n", SDL_GetError());
        return NULL;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screenSurface);
    SDL_FreeSurface(screenSurface);
    return texture;
}

// Función para mostrar pantalla de Game Over
void mostrarPantallaGameOver(SDL_Texture* pantallaGameOver) {
    SDL_Event e;
    int enPantallaGameOver = 1;
    
    while(enPantallaGameOver) {
        SDL_RenderCopy(renderer, pantallaGameOver, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                exit(0);
            }
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == TECLA_REINICIO) {
                enPantallaGameOver = 0;
            }
        }
        SDL_Delay(10);
    }
    SDL_DestroyTexture(pantallaGameOver);
    pantallaGameOver = NULL; 
}

// Función para mostrar pantalla de victoria
SDL_Texture* cargarPantallaYouWin();
void mostrarPantallaYouWin(SDL_Texture* pantallaYouWin);

SDL_Texture* cargarPantallaYouWin() {
    SDL_Surface* screenSurface = SDL_LoadBMP("youwin.bmp");
    if (!screenSurface) {
        printf("Error cargando imagen de you win: %s\n", SDL_GetError());
        return NULL;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screenSurface);
    SDL_FreeSurface(screenSurface);
    return texture;
}

void mostrarPantallaYouWin(SDL_Texture* pantallaYouWin) {
    SDL_Event e;
    int enPantallaYouWin = 1;
    
    while(enPantallaYouWin) {
        // Limpiar pantalla
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Dibujar la textura
        SDL_RenderCopy(renderer, pantallaYouWin, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        // Procesar eventos
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                exit(0);
            }
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                enPantallaYouWin = 0; // Salir del bucle solo con SPACE
            }
        }
        SDL_Delay(10);
    }
    SDL_DestroyTexture(pantallaYouWin);
}

// Función para reiniciar todo el juego
void reinicializarJuego() {
    currentLevel = 1;
    currentMap = mapa1;
    id_enemigo = 1;
    puntosGlobales=0;

    // Limpiar lista de enemigos
    while (lista_enemigos) {
        enemies *temp = lista_enemigos;
        lista_enemigos = lista_enemigos->next;
        free(temp);
    }

    // Limpiar lista de spawn
    while (spawn_list) {
        SpawnNode *temp = spawn_list;
        spawn_list = spawn_list->next;
        free(temp);
    }
    spawn_list = NULL;

    while (activeSpawnAnimations) {
        SpawnAnimation *temp = activeSpawnAnimations;
        activeSpawnAnimations = activeSpawnAnimations->next;
        free(temp);
    }
    activeSpawnAnimations = NULL;

    // Restaurar bloques si estaban transformados por pala
    if (palaActiva) {
        restaurarBloques();
    }

    // Destruir estructuras del juego
    destroyBloques();
    destroyBalas();
    destroyBalasEnemigas();

    // Destruir power-ups
    for (int i = 0; i < totalPowerUps; i++) {
        free(powerUps[i]);
    }
    free(powerUps);
    powerUps = NULL;
    totalPowerUps = 0;

    // Limpiar bloques destruidos y transformados
    free(destroyedBlockRects);
    destroyedBlockRects = NULL;
    totalDestroyedBlocks = 0;

    bloquesTransformados = NULL;
    totalTransformados = 0;
    palaActiva = 0;

    // Destruir texturas y renderer si existen
    if (tileset) {
        SDL_DestroyTexture(tileset);
        tileset = NULL;
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    // Recrear renderer y tileset
    renderer = createRenderer(window);
    if (!renderer) {
        printf("Error al crear renderer al reiniciar\n");
        exit(1);
    }

    tileset = loadTexture(renderer, "sprite1.bmp");
    if (!tileset) {
        printf("Error al recargar tileset al reiniciar\n");
        exit(1);
    }

    // Reiniciar jugador
    jugador = (Jugador){
        {220, 520, TANK_SIZE, TANK_SIZE},
        3, 0, 0, 0, 0, 0, 0, 0,
        1,                      // respawnando
        SDL_GetTicks(),        // tiempoRespawn
        0,                     // respawnFrame
        SDL_GetTicks()         // tiempoUltimoFrameRespawn
    };
    jugador.escudoActivo = 0;
    jugador.tiempoInicioEscudo = 0;
    jugador.escudoFrame = 0;
    jugador.tiempoUltimoFrameEscudo = 0;

    // Reiniciar sistema de niveles y spawns
    setup_spawn_list_for_level(currentLevel);
    initBloques(currentMap);
    levelStartTime = SDL_GetTicks() + 2000;

    // Reiniciar puntuación
    puntosBasic = puntosFast = puntosArmor = puntosPower = 0;
    puntosTotales = 0;
}


// ====================
// Dibujado del tanque 
// ====================
void dibujarTanque(SDL_Renderer *renderer, SDL_Texture *tileset, Jugador jugador) {
    if(jugador.nivelDisparo == 0){
        SDL_Rect spriteTanque[4] = {
            {3,  5, 26, 26},    // Arriba
            {3, 65, 26, 26},   // Abajo
            {5, 99, 26, 26},   // Izquierda
            {1, 35, 26, 26}     // Derecha
        };
    SDL_RenderCopy(renderer, tileset, &spriteTanque[jugador.direccion], &jugador.rect);
    }
    if(jugador.nivelDisparo == 1){
        SDL_Rect spriteTanque[4] = {
            {67,  0, 25, 31},    // Arriba
            {67, 64, 25, 31},   // Abajo
            {64, 99, 31, 25},   // Izquierda
            {64, 35, 31, 25}     // Derecha
        };
    SDL_RenderCopy(renderer, tileset, &spriteTanque[jugador.direccion], &jugador.rect);
    }
    if(jugador.nivelDisparo == 2){
        SDL_Rect spriteTanque[4] = {
            {131,  2, 25, 29},    // Arriba
            {131, 64, 25, 29},   // Abajo
            {130, 99, 29, 25},   // Izquierda
            {128, 35, 29, 25}     // Derecha
        };
    SDL_RenderCopy(renderer, tileset, &spriteTanque[jugador.direccion], &jugador.rect);
    }
    if(jugador.nivelDisparo == 3){
        SDL_Rect spriteTanque[4] = {
            {194,  2, 27, 29},    // Arriba
            {194, 64, 27, 29},   // Abajo
            {194, 98, 29, 27},   // Izquierda
            {192, 34, 29, 27}     // Derecha
        };
    SDL_RenderCopy(renderer, tileset, &spriteTanque[jugador.direccion], &jugador.rect);
    }
}


void dibujarBalas(SDL_Renderer *renderer, Bala **balas, int totalBalas) {
    SDL_Rect balaSrcRect = {350, 433, 12, 12}; 

    for (int i = 0; i < totalBalas; i++) {
        SDL_Rect destino = {
            balas[i]->rect.x,
            balas[i]->rect.y,
            balas[i]->rect.w,
            balas[i]->rect.h
        };

        SDL_RenderCopy(renderer, tileset, &balaSrcRect, &destino);
    }
}

// ====================
// Dibujado de power-ups 
// ====================
void dibujarPowerUps(SDL_Renderer *renderer, SDL_Texture *tileset) {
    SDL_Rect spritePowerUp[6] = {
        {31, 392, 32, 32},  // Tanque
        {0, 392, 32, 32},  // Estrella
        {63, 392, 32, 32},   // Casco
        {95, 392, 32, 32}, //pala
        {64, 360, 32, 32}, //reloj
        {94, 360, 32, 32} //pala

    };

    for (int i = 0; i < totalPowerUps; i++) {
        if (powerUps[i]->activo) {
            SDL_RenderCopy(renderer, tileset, &spritePowerUp[powerUps[i]->tipo], &powerUps[i]->rect);
        }
    }
}

int rectsOverlap(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}

int colisionJugador(SDL_Rect jugadorRect) {
    SDL_Rect bordes = {mapOffsetX, mapOffsetY, MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE};
    if (jugadorRect.x < bordes.x ||
        jugadorRect.y < bordes.y ||
        jugadorRect.x + jugadorRect.w > bordes.x + bordes.w ||
        jugadorRect.y + jugadorRect.h > bordes.y + bordes.h) {
        return 1;
    }
    for (int i = 0; i < totalBloques; i++) {
        Bloque *bloque = bloques[i];
        if (bloque && (bloque->type == 1 || bloque->type == 2 || bloque->type == 4)&& rectsOverlap(jugadorRect, bloque->rect)) {
            return 1;
        }
    }
    return 0;
}

// Mueve el jugador en solo una dirección a la vez (sin diagonales)
void moverJugador(float dx, float dy) {
    // Actualiza la dirección según la tecla presionada
    if (dx > 0) {
        jugador.direccion = 3; // Derecha
    } else if (dx < 0) {
        jugador.direccion = 2; // Izquierda
    } else if (dy > 0) {
        jugador.direccion = 1; // Abajo
    } else if (dy < 0) {
        jugador.direccion = 0; // Arriba
    }

    SDL_Rect newRect = jugador.rect;
    newRect.x += dx;
    newRect.y += dy;
    if (!colisionJugador(newRect)) {
        jugador.rect = newRect;
    }
}

void recogerPowerUp() {
    for (int i = 0; i < totalPowerUps; i++) {
        if (powerUps[i]->activo && rectsOverlap(jugador.rect, powerUps[i]->rect)) {
            // Cada que se recoja en power up se suman 500 puntos al puntaje global, de a cuerdo con el gameplay original
            puntosGlobales += 500;
            switch (powerUps[i]->tipo) {
                case 0: //Tanque
                    jugador.vida++; // Aumenta vida
                    break;
                case 1://Estrella
                if (jugador.nivelDisparo < 3) {
                    jugador.nivelDisparo++; // Sube de nivel
                    if (jugador.nivelDisparo == 1) {
                        NormalBulletSpeed = FastBulletSpeed; // Balas más rápidas
                    }
                }
                    break;
                case 2:
                    jugador.escudoActivo = 1;
                    jugador.tiempoInicioEscudo = SDL_GetTicks();
                    jugador.escudoFrame = 0; // Reiniciar animación
                    jugador.tiempoUltimoFrameEscudo = SDL_GetTicks();
                    break;
                case 3: 
                    convertirBloquesEspecificos();
                    palaActiva = 1;
                    tiempoPowerUpPala = SDL_GetTicks();
                    break;
                case 4: 
                    enemigosCongelados = 1;
                    tiempoDescongelar = SDL_GetTicks() + 6000;  // 6 segundos de congelación
                    break;
                case 5: 
                    enemies *temp = lista_enemigos;
                    while (temp != NULL) {
                        SDL_Rect expl = temp->enemy.rect;
                        agregarExplosion(expl);  // Muestra explosión en la posición del enemigo
                        temp = temp->next;
                    }
                    free_enemies(lista_enemigos);
                    lista_enemigos = NULL;
                    break;
                }
            powerUps[i]->activo = 0; // Lo elimina del mapa
        }
    }
}

void dibujarVidas(SDL_Renderer *renderer, SDL_Texture *tileset, Jugador jugador) {
    // El tamaño de cada icono de vida 
    const int iconoWidth = 30;
    const int iconoHeight = 30;
    const int iconoPosX = 630;
    const int iconoPosY = 400;
    // El rectángulo de destino para cada icono de vida
    SDL_Rect rectDestinoIcono;
    rectDestinoIcono.x = iconoPosX;
    rectDestinoIcono.y = iconoPosY;
    // Itera a través de las vidas disponibles y dibuja un icono por cada vida
    for (int i = 0; i < jugador.vida; i++) {
        rectDestinoIcono.x = iconoPosX + i * (iconoWidth + 5); // Espacio entre los iconos
        rectDestinoIcono.w = iconoWidth;
        rectDestinoIcono.h = iconoHeight;

        // Dibuja el icono del tileset (ajustar la fuente de icono si es necesario)
        SDL_Rect rectFuente = {32, 272, 16, 16};  
        SDL_RenderCopy(renderer, tileset, &rectFuente, &rectDestinoIcono);
    }
    
}

void dibujarVidaEnemigo(SDL_Renderer *renderer, SDL_Texture *tileset,int enemigos_vivos) {
    const int iconoWidth = 25;
    const int iconoHeight = 25;
    const int offsetX = 3;  // Espacio horizontal entre iconos
    const int offsetY = 3; // Espacio vertical entre filas
    const int iconoPosX = 654;
    const int iconoPosY = 40;

    SDL_Rect rectDestinoIcono;
    SDL_Rect rectFuente = {48, 273, 15, 15}; 

    for (int i = 0; i < enemigos_vivos; i++) {
        int fila = i / 2;  // Cada fila contiene 2 iconos
        int columna = i % 2;

        rectDestinoIcono.x = iconoPosX + columna * (iconoWidth + offsetX);
        rectDestinoIcono.y = iconoPosY + fila * (iconoHeight + offsetY);
        rectDestinoIcono.w = iconoWidth;
        rectDestinoIcono.h = iconoHeight;

        SDL_RenderCopy(renderer, tileset, &rectFuente, &rectDestinoIcono);
    }
}

void dibujarBandera(SDL_Renderer* renderer, SDL_Texture* tileset) {
    
    SDL_Rect srcBandera = {65, 272, 30, 30};
    
    
    SDL_Rect destBandera = {630, 505, 30, 30}; 
    
    SDL_RenderCopy(renderer, tileset, &srcBandera, &destBandera);
}

void dibujarSpriteMapaActual(SDL_Renderer* renderer, SDL_Texture* tileset) {
    SDL_Rect srcSprite;
    
    // Seleccionar sprite según nivel actual
    switch(currentLevel) {
        case 1: srcSprite = (SDL_Rect){16, 1010, 16, 16}; break;
        case 2: srcSprite = (SDL_Rect){32, 1010, 16, 16}; break;
        case 3: srcSprite = (SDL_Rect){48, 1010, 16, 16}; break;
        case 4: srcSprite = (SDL_Rect){64, 1010, 16, 16}; break;
        case 5: srcSprite = (SDL_Rect){80, 1010, 16, 16}; break;
        default: srcSprite = (SDL_Rect){16, 1010, 16, 16};
    }
    
    SDL_Rect destSprite = {637, 535, 16, 16};
    SDL_RenderCopy(renderer, tileset, &srcSprite, &destSprite);
}

// ====================
// Renderizado del mapa (bloques) y balas
// ====================

void renderizarMapa() {
    SDL_Rect src1,src3,src4;
    src1.w = 16;
    src1.h = 16;
    int w = 20, h = 20; // Variables temporales
    
    int xBase = -1, yBase = -1; // Posiciones iniciales del bloque 5

    for (int i = 0; i < totalBloques; i++) {
        Bloque *bloque = bloques[i];
        if (bloque) {
            if (bloque->type == 0) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &bloque->rect);
            }

            if (bloque->type == 1) { // *Bloque destructible con impacto*
                if (bloque->dureza == 1) { 
                    switch (bloque->impacto) {
                        case 0: // Impacto desde arriba
                            src1.x = 184; src1.y = 264;
                            src4.x = bloque->rect.x;
                            src4.y = bloque->rect.y;
                            src4.w = w;
                            src4.h = h;
                            
                            src3 = src4;
                            src3.y = src3.y + 10;
                            SDL_RenderCopy(renderer, tileset, &src1, &src4);
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            SDL_RenderFillRect(renderer, &src3);
                        break;
                        
                        case 1: // Impacto desde abajo
                            src1.x = 96; src1.y = 256;
                            src4.x = bloque->rect.x;
                            src4.y = bloque->rect.y;
                            src4.w = w;
                            src4.h = h;
    
                            src3 = src4;
                            src3.y = src3.y -10;
                            SDL_RenderCopy(renderer, tileset, &src1, &src4);
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            SDL_RenderFillRect(renderer, &src3);
                        break;
    
                        case 2: // Impacto desde la izquierda
                            src1.x =80; src1.y = 256;
                            src4.x = bloque->rect.x;
                            src4.y = bloque->rect.y;
                            src4.w = w;
                            src4.h = h;

                            src3 = src4;
                            src3.x = src3.x + 10;
                            SDL_RenderCopy(renderer, tileset, &src1, &src4);
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            SDL_RenderFillRect(renderer, &src3);
                        break;
    
                        case 3: // Impacto desde la derecha
                            src1.x = 112; src1.y = 256;
                            src4.x = bloque->rect.x;
                            src4.y = bloque->rect.y;
                            src4.w = w;
                            src4.h = h;

                            src3 = src4;
                            src3.x = src3.x - 10;
                            SDL_RenderCopy(renderer, tileset, &src1, &src4);
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            SDL_RenderFillRect(renderer, &src3);
                        break;
    
                        default: // Estado inicial
                            src1.x = 0; src1.y = 256;
                            SDL_RenderCopy(renderer, tileset, &src1, &bloque->rect);
                            break;
                    }
                } else {  // Si dureza != 1
                    src1.x = 0; src1.y = 256;
                    SDL_RenderCopy(renderer, tileset, &src1, &bloque->rect);
                }
            }

            if (bloque->type == 2) { // Bloque indestructible
                src1.x = 0;
                src1.y = 272;
                SDL_RenderCopy(renderer, tileset, &src1, &bloque->rect);
            }

            
			if (bloque->type == 4) { // Bloque indestructible
                src1.x = 0;
                src1.y = 320;
                SDL_RenderCopy(renderer, tileset, &src1, &bloque->rect);
            }
        }
    }
    
    // Dibuja las áreas de bloques destruidos con fondo negro
    for (int i = 0; i < totalDestroyedBlocks; i++) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &destroyedBlockRects[i]);
    }

    for (int i = 0; i < totalBloques; i++) {
        Bloque *bloque = bloques[i];

        if (bloque->type == 5) { // Bloque indestructible
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &bloque->rect);

        // Guardar la posición del primer bloque tipo 5
        if (xBase == -1 && yBase == -1) {
            xBase = bloque->rect.x;
            yBase = bloque->rect.y;
            }
            
        }
    }
    // Dibujar la imagen encima de los bloques tipo 5 (si existen)
    if (xBase != -1 && yBase != -1) {
        SDL_Rect srcSprite = {0, 361, 32, 32}; 
        SDL_Rect destSprite = {xBase, yBase, TILE_SIZE * 2, TILE_SIZE * 2}; // Tamaño sobre los 4 bloques

        SDL_RenderCopy(renderer, tileset, &srcSprite, &destSprite);
    }
}

void renderizarPastoEncima() {
    SDL_Rect src;
    src.w = 16;
    src.h = 16;
    src.x = 0;
    src.y = 304;  // Coordenada del pasto en tu tileset (ajustala si es otra)

    for (int i = 0; i < totalBloques; i++) {
        Bloque *bloque = bloques[i];
        if (bloque && bloque->type == 3) {
            SDL_RenderCopy(renderer, tileset, &src, &bloque->rect);
        }
    }
}

int main() {

    if (!initSDL()) {
        return 1;
    }
    window = createWindow();
    if (!window) {
        return 1;
    }
    renderer = createRenderer(window);
    if (!renderer) {
        SDL_DestroyWindow(window);
        return 1;
    }
    tileset = loadTexture(renderer, "sprite1.bmp");
    if (!tileset) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 1;
    }

    SDL_Texture* pantallaInicio = cargarPantallaInicio();
    if(pantallaInicio) {
        mostrarPantallaInicio(pantallaInicio);
    }

    currentLevel = 1;  // <-- Inicializar siempre desde nivel 1
    currentMap = mapa1; // <-- Inicializar siempre con el mapa 1
    setup_spawn_list_for_level(currentLevel);
    levelStartTime = SDL_GetTicks() + 2000;  // Inicializar timer para primer nivel
    
    // Inicializa el tamaño del tanque usando TANK_SIZE
    jugador.rect.w = TANK_SIZE;
    jugador.rect.h = TANK_SIZE;

    initBloques(currentMap);

    balas_enemigas = NULL;
    totalBalasEnemigas= 0;

    balas = NULL;
    totalBalas = 0;
    
    int corriendo = 1;
    SDL_Event e;
    float playerSpeed = 200.0f;
    float playerVelX = 0.0f, playerVelY = 0.0f;
    Uint32 lastTime = SDL_GetTicks();
    Uint32 frameStart;
    float deltaTime;
    int frameDelay = 1000 / 60; // 60 FPS
    Uint32 nextSpawnTime = 0;
    int prevEnemyCount = 0;

    while (corriendo) {
        frameStart = SDL_GetTicks();
        Uint32 currentTime = SDL_GetTicks();
        int levelStarted = currentTime >= levelStartTime;

        if (enemigosCongelados && SDL_GetTicks() > tiempoDescongelar) {
            enemigosCongelados = 0;
        }        

        // Verificar si el escudo debe desactivarse después de 5 segundos
        if (jugador.escudoActivo && (SDL_GetTicks() - jugador.tiempoInicioEscudo >= 5000)) {
            jugador.escudoActivo = 0;
        }

        if (palaActiva && SDL_GetTicks() - tiempoPowerUpPala >= 7000) {
            restaurarBloques();
        }

        // Procesa eventos para cosas puntuales (disparo SPACE)
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT||e.key.keysym.sym == SDLK_ESCAPE) {
                corriendo = 0;
            }
            // Procesa el disparo cuando se presiona SPACE
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                if (levelStarted && !jugador.respawnando) {
                if (jugador.nivelDisparo >= 2) { // Permite 2 balas en pantalla
                    if (totalBalas < 2) { 
                        int centerX = jugador.rect.x + jugador.rect.w / 2 - 2;
                        int centerY = jugador.rect.y + jugador.rect.h / 2 - 2;
                        Bala *nuevaBala = crearBala(centerX, centerY, 0, 0);
                        nuevaBala->direccion = jugador.direccion;
                        agregarBala(nuevaBala);
                    }
            
                } else if (totalBalas == 0) { // Nivel 0 y 1: Solo 1 bala en pantalla
                    int centerX = jugador.rect.x + jugador.rect.w / 2 - 2;
                    int centerY = jugador.rect.y + jugador.rect.h / 2 - 2;
                    Bala *nuevaBala = crearBala(centerX, centerY, 0, 0);
                    nuevaBala->direccion = jugador.direccion;
                    agregarBala(nuevaBala);
                }
            }
        }
    }

        if (jugador.escudoActivo) {
            // Desactivar después de 5 segundos (5000 ms)
            if (SDL_GetTicks() - jugador.tiempoInicioEscudo >= 5000) {
                jugador.escudoActivo = 0;
                printf("Escudo desactivado\n");
            }
        }

        // Consulta continua del estado del teclado para actualizar el movimiento
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        playerVelX = 0;
        playerVelY = 0;

        // Se permite solo un eje (sin diagonales); se revisa en un orden de prioridad
        if (levelStarted && !jugador.respawnando) {
            if (keys[SDL_SCANCODE_W]) {
                playerVelY = -playerSpeed; // Arriba
                jugador.direccion = 0;
            } else if (keys[SDL_SCANCODE_S]) {
                playerVelY = playerSpeed; // Abajo
                jugador.direccion = 1;
            } else if (keys[SDL_SCANCODE_A]) {
                playerVelX = -playerSpeed; // Izquierda
                jugador.direccion = 2;
            } else if (keys[SDL_SCANCODE_D]) {
                playerVelX = playerSpeed; // Derecha
                jugador.direccion = 3;
            }
        }
        
        deltaTime = (SDL_GetTicks() - lastTime) / 1000.0f;
        lastTime = SDL_GetTicks();
        
        moverJugador(playerVelX * deltaTime, playerVelY * deltaTime);
        procesarBalas(&balas, &totalBalas, &bloques, &totalBloques, deltaTime);
        procesarBalasEnemigas(&balas_enemigas, &totalBalasEnemigas,&bloques, &totalBloques, deltaTime);
        if (jugador.respawnando) {
            if (SDL_GetTicks() - jugador.tiempoRespawn >= 2000) {
                jugador.respawnando = 0;
                jugador.escudoActivo = 1;
                jugador.tiempoInicioEscudo = SDL_GetTicks();
            }
        }
        gestionarColisionBalasJugadorVsEnemigas(&balas, &totalBalas, &balas_enemigas, &totalBalasEnemigas);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);
    
        renderizarMapa();
        if (jugador.respawnando || SDL_GetTicks() < levelStartTime) {
            dibujarAnimacionRespawn(renderer, tileset, &jugador);
        } else {
            dibujarTanque(renderer, tileset, jugador);
        }
        if (jugador.escudoActivo) {
            dibujarEscudo(renderer, tileset, &jugador);
        }
        dibujarBalas(renderer, balas, totalBalas);
        dibujarBalasEnemigas(renderer, balas_enemigas, totalBalasEnemigas);
        dibujarVidas(renderer, tileset, jugador);
        dibujarVidaEnemigo(renderer,tileset,enemigos_vivos);
        recogerPowerUp();       // Verifica si el jugador recoge un power-up
        dibujarAnimacionSpawn(renderer, tileset);
        dibujarBandera(renderer, tileset);
        dibujarSpriteMapaActual(renderer, tileset);

    
        // Procesar enemigos solo si el nivel ha comenzado
        if (levelStarted) {
            int currentEnemyCount = 0;
            enemies *tempCount = lista_enemigos;
            while (tempCount) {
                currentEnemyCount++;
                tempCount = tempCount->next;
            }

            if (currentEnemyCount < MAX_ENEMIGOS_SIMULTANEOS && spawn_list != NULL) {
                if (prevEnemyCount >= MAX_ENEMIGOS_SIMULTANEOS) {
                    nextSpawnTime = currentTime + TIEMPO_ENTRE_OLEADAS;
                }
                
                if (currentTime >= nextSpawnTime) {
                    SpawnNode *nextSpawn = spawn_list;
                    SpawnAnimation *newAnim = (SpawnAnimation*)malloc(sizeof(SpawnAnimation));
                    newAnim->fila = nextSpawn->fila;
                    newAnim->columna = nextSpawn->columna;
                    newAnim->tipo = nextSpawn->tipo;
                    newAnim->x = mapOffsetX + nextSpawn->columna * TILE_SIZE;
                    newAnim->y = mapOffsetY + nextSpawn->fila * TILE_SIZE;
                    newAnim->startTime = currentTime;
                    newAnim->next = activeSpawnAnimations;
                    activeSpawnAnimations = newAnim;
                    spawn_list = nextSpawn->next;
                    free(nextSpawn);
                    nextSpawnTime = currentTime + TIEMPO_ENTRE_OLEADAS;
                }
            }
            prevEnemyCount = currentEnemyCount;

            enemies *actual = lista_enemigos;
            enemies *prev = NULL;
            while (actual != NULL) {
                moverEnemigoIA(&actual->enemy, deltaTime);
                enemies *next = actual->next;
                if (actual->enemy.vida <= 0) {
                    if (prev) prev->next = next;
                    else lista_enemigos = next;
                    free(actual);
                } else {
                    prev = actual;
                }
                actual = next;
            }
        }

         // Dibujar enemigos
         enemies *actual = lista_enemigos;
         while (actual != NULL) {
             dibujarEnemigo(renderer, tileset, actual->enemy);
             actual = actual->next;
         }
 

        renderizarPastoEncima();
        dibujarPowerUps(renderer, tileset);  // Dibuja los power-ups en pantalla
        dibujarExplosiones(renderer, tileset);

        // Logica de cambio de nivel
        if (spawn_list == NULL && lista_enemigos == NULL && activeSpawnAnimations == NULL) {
            if (currentLevel >= 5) {
                // PANTALLA DE VICTORIA
                SDL_Texture* pantallaYouWin = cargarPantallaYouWin();
                if (pantallaYouWin) {
                    mostrarPantallaYouWin(pantallaYouWin); // Espera tecla SPACE
                    SDL_DestroyTexture(pantallaYouWin);
                }
        
                reinicializarJuego();
                SDL_Texture* nuevaPantallaInicio = cargarPantallaInicio();
                if (nuevaPantallaInicio) {
                    mostrarPantallaInicio(nuevaPantallaInicio);
                    SDL_DestroyTexture(nuevaPantallaInicio);
                }
                continue;
            }
        
            // TRANSICIÓN NORMAL A SIGUIENTE NIVEL
            mostrarTransicionNivel(renderer, tileset, currentLevel);
            currentLevel++;
            levelStartTime = SDL_GetTicks() + 2000;
        
            jugador.respawnando = 1;
            jugador.tiempoRespawn = SDL_GetTicks();
            jugador.respawnFrame = 0;
            jugador.tiempoUltimoFrameRespawn = SDL_GetTicks();
        
            switch(currentLevel) {
                case 2: currentMap = mapa2; break;
                case 3: currentMap = mapa3; break;
                case 4: currentMap = mapa4; break;
                case 5: currentMap = mapa5; break;
            }
        
            setup_spawn_list_for_level(currentLevel);
            destroyBloques();
            initBloques(currentMap);
            jugador.rect.x = 220;
            jugador.rect.y = 520;
            jugador.direccion = 0;
            jugador.nivelDisparo = 0;
            jugador.escudoActivo = 0;
            NormalBulletSpeed = 200.0f;
            free_enemies(lista_enemigos);
            lista_enemigos = NULL;
        
            for (int i = 0; i < totalPowerUps; i++) {
                free(powerUps[i]);
            }
            free(powerUps);
            powerUps = NULL;
            totalPowerUps = 0;
        
            puntosBasic = puntosFast = puntosArmor = puntosPower = puntosTotales = 0;
        
            if (destroyedBlockRects) {
                free(destroyedBlockRects);
                destroyedBlockRects = NULL;
                totalDestroyedBlocks = 0;
            }
        }
        
        // Verificar colisiones entre jugador y enemigos
        enemies *temp = lista_enemigos;
        while (temp != NULL) {
            if (SDL_HasIntersection(&jugador.rect, &temp->enemy.rect) && !jugador.respawnando) {
                if (!jugador.escudoActivo) {
                    jugador.vida--;
                    jugador.respawnando = 1;
                    jugador.tiempoRespawn = SDL_GetTicks();
                    jugador.rect.x = 220;
                    jugador.rect.y = 520;
                    
                    float dx = jugador.rect.x - temp->enemy.rect.x;
                    float dy = jugador.rect.y - temp->enemy.rect.y;
                    float distancia = sqrt(dx*dx + dy*dy);
                    if (distancia > 0) {
                        temp->enemy.rect.x -= (dx / distancia) * 20;
                        temp->enemy.rect.y -= (dy / distancia) * 20;
                    }
                }
                else {
                    temp->enemy.vida--;
                }
            }
            temp = temp->next;
        }

        if (jugador.vida <= 0) {
            
            SDL_Texture* pantallaGameOver = cargarPantallaGameOver();
            if(pantallaGameOver) {
                mostrarPantallaGameOver(pantallaGameOver);
            }
            
            // Reiniciar el juego
            reinicializarJuego();
            
            // Volver a mostrar pantalla de inicio
            SDL_Texture* nuevaPantallaInicio = cargarPantallaInicio();
            if(nuevaPantallaInicio) {
                mostrarPantallaInicio(nuevaPantallaInicio);
            }
        }
        
        SDL_RenderPresent(renderer);
    
        frameDelay = 1000 / 60 - (SDL_GetTicks() - frameStart);
        if (frameDelay > 0) {
            SDL_Delay(frameDelay);
        }
    }

    while (spawn_list) {
        SpawnNode *temp = spawn_list;
        spawn_list = spawn_list->next;
        free(temp);
    }

    while (activeSpawnAnimations != NULL) {
        SpawnAnimation *temp = activeSpawnAnimations;
        activeSpawnAnimations = activeSpawnAnimations->next;
        free(temp);
    }
    
    destroyBloques();
    destroyBalas();
    destroyBalasEnemigas();
    if (destroyedBlockRects)
        free(destroyedBlockRects);
    SDL_DestroyTexture(tileset);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    free_enemies(lista_enemigos);
    SDL_Quit();
    return 0;
}