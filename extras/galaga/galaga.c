#include <xinu.h>
#include <microui.h>
#include "titlescreen.h"
#include "playerImage.h"
#include "enemy.h"
#include "boss.h"
#include "gameover.h"
#include "shoot.h"
#include "win.h"
#include "controls.h"

int n_galaga_window;

volatile unsigned char tecla_actual;
typedef unsigned short u16;

/*
ESTA LINEA DE ABAJO ESTA HORRIBLEMENTE MAL
NO LA CORREGI PORQUE YA HABIA ARMADO LAS IMAGENES
FORMA CORRECTA
#define RGB(r, g, b) ((r << 10) | (g << 5) | b )
*/
#define RGB(r, g, b) (r | (g << 5) | (b << 10))
// #define REG_DISPCNT *(u16 *)0x4000000
#define extern videoBuffer
#define MODE3 3
#define BG2_ENABLE (1 << 10)
#define WHITE RGB(255, 255, 255)
#define BLACK RGB(0, 0, 0)
#define YELLOW RGB(255, 255, 0)

/*
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)
#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
*/
//#define BUTTONS *(volatile unsigned int *)0x4000130

#define BUTTON_A		'j'	//N
#define BUTTON_B		'k' 	//M
#define BUTTON_SELECT	'l'	//RIGHT SHIFT
#define BUTTON_START	'1'  	//ENTER
#define BUTTON_RIGHT	'd'	//D
#define BUTTON_LEFT		'a' //A
#define BUTTON_UP		'w'	//W
#define BUTTON_DOWN 		's'	//S
#define BUTTON_R		'n' 	//K
#define BUTTON_L		'm'	//J
#define BUTTON_ESCAPE	'0'		//ESC
// #define KEY_DOWN_NOW(key)  (tecla_actual == key)

int teclasg[11] = {'j', 'k', 'l', '1', 'd', 'a', 'w', 's', 'n', 'm', '0'};
int pulsadas[11];
void reading_keys()
{
	 mu_event_t e;
	 while(1) {
                mu_get_event(n_galaga_window, &e);
		//if (e.c[0] != '\0')
		//	tecla_actual = e.c[0];
		if (e.c[0] != '\0') {
			for (int i=0; i<11; i++) {
				if (teclasg[i] == e.c[0]) {
					pulsadas[i] = 1;
				}
			}
		}
		sleepms(2);
	 }

}

void teclas_init() 
{
	for (int i=0; i<11; i++)
		pulsadas[i] = -1;
}

int KEY_DOWN_NOW(char key)
{
	for (int i=0; i<11; i++) {
		if ((pulsadas[i] != -1) && (teclasg[i] == key)) {
			pulsadas[i] = -1;
			return 1;
		}
	}
	return 0;

		 /*
	        if (tecla_actual == key) {
                tecla_actual = -1;
                return 1;
        }
        return 0;
	*/
}


//variable definitions
#define playerspeed 2
#define enemyXSpeed 3
#define enemyYSpeed 2
#define respawnPosition 65516

int enemyspeed = 1;

void setPixel(int x, int y, u16 color);
void drawRect(int x, int y, int width, int height, u16 color);
void drawHollowRect(int x, int y, int width, int height, u16 color);
void drawImage3(int x, int y, int width, int height, const u16* image);
void delay_galaga();
int everyone_die();

//helpers
void initialize();
void drawEnemies();
void game_over(int);
int collision(u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight,
			u16 playerX, u16 playerY, u16 playerWidth, u16 playerHeight);

void restart();

void game_control();
int game_galaga(int n);
void info_game();

//objects
struct Players {
	volatile u16 playerX;
	volatile u16 playerY;
	volatile int lives;
	volatile u16 score;
};
struct Enemy {
	volatile u16 enemyX;
	volatile u16 enemyY;
	volatile int lives;
};
struct Shoot{
	volatile u16 shootX;
	volatile u16 shootY;
	volatile u16 in_use;	
};

void updateEnemy(struct Enemy *);
void checkHitShootEnemy(struct Enemy *, struct Shoot *);
void checkEnemyDamagePlayer(struct Enemy *);
void shoot_bullet();


//Shoot information
struct Shoot shoots[10];
int curr_shot = 0;
#define N_SHOOTS 10

struct Enemy easyEnemies[9];
struct Enemy hardEnemies[9];
struct Players player;
struct Enemy fast;


//Show info
char str_lives[32], str_score[32];

//Control information
int pid_control, pid_info, pid_game;

int galaga(int n){
	n_galaga_window = n;
	printf("n2=%d\n", n);
	int pid_keys;
	pid_keys = create(reading_keys, 1024, 20, "keys", 0);
	resume(pid_keys);

	pid_control = getpid();

	int endgame_msg;
	while (1) {
		teclas_init();
		initialize();
		restart();
	// gui_buf_init();
		pid_info = create(info_game, 1024, 20, "info_proc", 0);
		pid_game = create(game_galaga, 1024, 20, "game_proc", 1, n);
	
		resume(pid_info);
		resume(pid_game);

		recvclr();
		endgame_msg = receive();
		kill(pid_game);
		kill(pid_info);
		if (endgame_msg == 1) {
			kill(pid_keys);
			return endgame_msg;
		} else if (endgame_msg == 1) {
		}
	}
	return endgame_msg;
}

int game_galaga(int n) {
	printf("ENTRAMOS n=%d\n", n);
	//start black screen for drawing
	while(1) {
		//go back to title screen if select button is pressed
		// if (KEY_DOWN_NOW(BUTTON_SELECT)) {
		// 	initialize();
		// 	restart();
		//	game_galaga();
		//}

		if (KEY_DOWN_NOW(BUTTON_ESCAPE)){
			send(pid_control, 1);
		}

		//player shots 
		if (KEY_DOWN_NOW(BUTTON_A)) {
			shoot_bullet();
		}

		//player movement input
		if (KEY_DOWN_NOW(BUTTON_LEFT) && (player.playerX > 0)) {
			player.playerX -= playerspeed;
		}
		if (KEY_DOWN_NOW(BUTTON_RIGHT) && (player.playerX <= 216)) {
			player.playerX += playerspeed;
		}
		if (KEY_DOWN_NOW(BUTTON_UP) && (player.playerY > 25)) {
			player.playerY -= playerspeed;
		}
		if (KEY_DOWN_NOW(BUTTON_DOWN) && (player.playerY <= 136)) {
			player.playerY += playerspeed;
		}
		sleepms(40);
		gui_signal_redraw(n);  // Mark window dirty after game update


		//draw player
		drawImage3(player.playerX, player.playerY, 24, 24, playerImage);
		drawHollowRect(player.playerX - 1, player.playerY - 1, 26, 26, BLACK);
		drawHollowRect(player.playerX - 2, player.playerY - 2, 28, 28, BLACK);
		

		//draw enemies with downward movement
		for (int a = 0; a < 9; a++) {
			updateEnemy(&easyEnemies[a]);
			checkEnemyDamagePlayer(&easyEnemies[a]);
			updateEnemy(&hardEnemies[a]);
			checkEnemyDamagePlayer(&hardEnemies[a]);
		}
		//Update boss
		/*
		El jefe es el cerebro de la colmena, si el muere
		los enemigos no aceleran
		*/
		if(fast.lives > 0){
			drawRect(fast.enemyX, fast.enemyY, 15, 15, BLACK);
			fast.enemyY += enemyspeed;
			if((fast.enemyY > 180 && fast.enemyY < respawnPosition) &&
				!(fast.enemyY <= respawnPosition && fast.enemyY >= respawnPosition-80))
			{
				fast.enemyY = -20;
				enemyspeed = (enemyspeed%5)+1;
			}
			drawImage3(fast.enemyX, fast.enemyY, 15, 15, boss);
			checkEnemyDamagePlayer(&fast);
		}

		//draw shots
		for (int i = 0; i < N_SHOOTS; i++) {
			if (shoots[i].in_use > 0) {
				shoots[i].shootY = shoots[i].shootY-4;
				drawRect((shoots[i].shootX), (shoots[i].shootY)+4, 5, 5, BLACK);
				drawImage3((shoots[i].shootX), (shoots[i].shootY), 5, 5, shoot);
				if (shoots[i].shootY >= 160){ //Se verifica mayor a 160 porque nunca va a negativo
					drawRect((shoots[i].shootX), 0, 10, 10, BLACK);
					shoots[i].in_use = 0;
				}
			}

			// check hits of shoots
			for (int j = 0; j < 9; j++) {
				checkHitShootEnemy(&easyEnemies[j], &shoots[i]);
				checkHitShootEnemy(&hardEnemies[j], &shoots[i]);
			}
			checkHitShootEnemy(&fast, &shoots[i]);
		}
	}	
	return 0;
}

int collision(u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight,
	u16 playerX, u16 playerY, u16 playerWidth, u16 playerHeight) {
	//Case of not collision
	if((enemyX > playerX + playerWidth) ||
		(enemyX + enemyWidth < playerX) ||
		(enemyY > playerY + playerHeight) ||
		(enemyY + enemyHeight < playerY)
	){
		return 0;
	}

	return 1;
}

void checkHitShootEnemy(struct Enemy *enemy, struct Shoot *shoot){
	if (collision(enemy->enemyX, enemy->enemyY, 20, 20, shoot->shootX, shoot->shootY, 5, 5)) {
		drawRect(shoot->shootX, shoot->shootY, 5, 5, BLACK);
		shoot->in_use = 0;
		shoot->shootX = 5000;
		shoot->shootY = 5000;
		enemy->lives--;
		player.score += 1;
		send(pid_info, 1);
		if(enemy->lives == 0){
			drawRect(enemy->enemyX, enemy->enemyY, 20, 20, BLACK);
			enemy->enemyY = 0;
			enemy->enemyX = 5000;
			int some_alive = everyone_die();
			if(some_alive == 0){
				send(pid_info, 1);
				game_over(0);
			}
		}
	}
}

void checkEnemyDamagePlayer(struct Enemy *anEnemy){
	if (collision(anEnemy->enemyX, anEnemy->enemyY, 20, 20, player.playerX, player.playerY, 24, 24)) {
		drawRect(anEnemy->enemyX, anEnemy->enemyY,  20, 20, BLACK);
		anEnemy->enemyY = 0;
		anEnemy->enemyX = 5000;
		anEnemy->lives = 0;
		player.lives--;
		send(pid_info, 1);
		if(player.lives <= 0){
			game_over(1);
		}
	}
}

void game_over(int state) {
	//start Game Over State
	if(state == 1){
		drawImage3(0, 0, 240, 160, gameover);
		drawHollowRect(0, 0, 240, 160, WHITE);
	}
	else{
		drawImage3(0, 0, 240, 160, win);
		drawHollowRect(0, 0, 240, 160, BLACK);
	}

	memset(str_lives, 0, sizeof(str_lives));
	memset(str_score, 0, sizeof(str_score));

	sleep(4);
	send(pid_control, 2);
	while(1){
	}
}

void info_game(){
	while(1){
		//int change_msg = receive();
		receive();
		sprintf(str_lives, "Lives: %d", player.lives);
		sprintf(str_score, "Score: %d", player.score);
		// print_text_on_vga(280, 20, str_lives);
		// print_text_on_vga(280, 40, str_score);
	}
}

void initialize(){
	//easy enemy wave set setup
	for (int a = 0; a < 9; a++) {
		easyEnemies[a].enemyX = (25*a)+10;
		easyEnemies[a].enemyY = -20;
		easyEnemies[a].lives = 1;

		hardEnemies[a].enemyX = (25*a)+10;
		hardEnemies[a].enemyY = -60;
		hardEnemies[a].lives = 10;
	}

	//player setup
	player.playerX = 120;
	player.playerY = 136;
	player.lives = 3;
	player.score = 0;
	
	//fast enemy "boss" setup
	fast.enemyX = 120;
	fast.enemyY = -100;
	fast.lives = 25;

	// REG_DISPCNT = MODE3 | BG2_ENABLE;
	//initalize title screen
	drawImage3(0, 0, 240, 160, titlescreen);

	send(pid_info, 2);

	while(1) {
		if (KEY_DOWN_NOW(BUTTON_START))
			break;
		if (KEY_DOWN_NOW(BUTTON_ESCAPE))
			send(pid_control, 1);
		sleepms(2);
	}

	drawImage3(0, 0, 240, 160, controls);
	//ad ignoramtian a la siguiente linea
	tecla_actual = 0x2;
	while(1){
		if(KEY_DOWN_NOW(BUTTON_START)){
			break;
		}
		sleepms(2);
	}
}

/*
Recorre las balas desde la actual que se supone libre (curr_shot)
hasta encontrar una libre
*/
void shoot_bullet(){
	int foundBullet = 0;
	int i = 0;
	while(foundBullet == 0 && i < N_SHOOTS){
		int nextBullet = (curr_shot+i)%N_SHOOTS;
		if (shoots[nextBullet].in_use == 0) {
			shoots[nextBullet].shootX = player.playerX+9; /* 24 widht player */
			shoots[nextBullet].shootY = player.playerY-9;
			shoots[nextBullet].in_use = 1;
			curr_shot = nextBullet+1;
			if (curr_shot >= N_SHOOTS)
				curr_shot = 0;
			foundBullet = 1;
		}
		i++;
	}
}

/*
Actualiza la ubicion del enemigo
Si:
	Paso 180 y no esta en la zona de respawn
	O
	No esta en la zona de spawn. Notese rango [respawn-80; respawn+20]
*/
void updateEnemy(struct Enemy *anEnemy){
	if(anEnemy->lives > 0){
		drawRect(anEnemy->enemyX, anEnemy->enemyY, 20, 20, BLACK);
		anEnemy->enemyY = ((anEnemy->enemyY > 180 && anEnemy->enemyY < respawnPosition) &&
							!(anEnemy->enemyY <= respawnPosition && anEnemy->enemyY >= respawnPosition-80)) ?
						respawnPosition : anEnemy->enemyY + enemyspeed;
		drawImage3(anEnemy->enemyX, anEnemy->enemyY, 20, 20, enemy);
	}
}

int everyone_die(){
	int a = 0;
	int is_alive = 0;
	while (a < 9 && is_alive == 0) {
		is_alive += easyEnemies[a].lives;
		if(is_alive == 0)
			is_alive += hardEnemies[a].lives;

		a++;
	}

	if(is_alive == 0)
		is_alive += fast.lives;
	
	return is_alive;
}

void restart(){
	for (int i = 0; i < 240; i++) {
		for (int j = 0; j < 160; j++) {
			setPixel(i, j, BLACK);
		}
	}

	for (int a = 0; a < 9; a++) {
		shoots[a].in_use = 0;
		shoots[a].shootX = 1000;
		shoots[a].shootY = 1000;
	}
}
