#include <gb/gb.h>
#include <stdlib.h>
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include "MyBg.h"
#include "BolaSprite.h"
#include "RaqueteSprite.h"
uint8_t joypadPrevious=0, joypadCurrent=0;
int spriteBallX,spriteBallY;
int playerX,playerY;
int cpuX, cpuY;
int8_t velocityBallX,velocityBallY;
int8_t velocityplayerX=0,velocityplayerY=0;
int8_t velocitycpuX=0,velocitycpuY=0;


void main(void)
{
    OBP0_REG = 0xFC;
    BGP_REG  = 0xFC;
    DISPLAY_ON;

    SHOW_BKG;
    set_bkg_data(0,22,MyBg_tiles);
    set_bkg_tiles(0,0,20,18,MyBg_map);
    delay(3000);
    HIDE_BKG;

    SPRITES_8x8;
    SHOW_SPRITES;

    set_sprite_data(0,1,BolaSprite);
    set_sprite_tile(0,0);
    spriteBallX=80;
    spriteBallY=72;

    /* 
    Setando a velocidade do sprite randomizada no início da partida
    Ainda vou precisar randomizar após cada reinício de partida 
    */
    velocityBallX = -1;
    velocityBallY = 1;

    set_sprite_data(1,2,RaqueteSprite_tiles);
    playerX = 8;
    playerY = 86;
    cpuX = 164;
    cpuY = 86;


    // Loop forever
    while(1) {
        // Lógica da movimentação do player
        joypadPrevious=joypadCurrent;
        joypadCurrent=joypad();
        
        // Lógica da movimentação do player
        if(joypadCurrent & J_UP) {
            velocityplayerY=-1;
        } else if(joypadCurrent & J_DOWN) {
            velocityplayerY=1;
        } else {
            velocityplayerY=0;
        }
        playerY+=velocityplayerY;
        if(playerY < 32) {
            playerY = 32;
        }
        if(playerY > 144) {
            playerY = 144;
        }

        // Lógica da movimentação da CPU
        if (velocityBallX > 0) {
            if (cpuY < spriteBallY - 3) {
                cpuY += 1;
            } 
            else if (cpuY > spriteBallY + 3) {
                cpuY -= 1;
        }
            if(cpuY < 32) {
                cpuY = 32;
            }
            if(cpuY > 144) {
                cpuY = 144;
    }}

        // Lógica da movimentação da bola
        spriteBallX += velocityBallX;
        spriteBallY += velocityBallY;

        // Lógica da colisão da bola com as paredes superior e inferior
        if(spriteBallY < 16) {
            spriteBallY = 16;
            velocityBallY = -velocityBallY; // Inverte a direção vertical
        }
        if(spriteBallY > 152) {
            spriteBallY = 152;
            velocityBallY = -velocityBallY; // Inverte a direção vertical
        }

        // Lógica da saída da bola da tela (gol)
        if (spriteBallX < 0 || spriteBallX > 168) {
            if (spriteBallX < 0) {
                // Lógica do gol pro time da direita
            } else {
                // Lógica do gol pro time da esquerda
            }
            spriteBallX = 84;
            spriteBallY = 84;
            playerY = 86;
            cpuY = 86;
            delay(1000); // Pausa pra mostrar o gol
            move_metasprite_ex(RaqueteSprite_metasprites[0], 1, 0, 1, playerX, playerY);
            move_metasprite_ex(RaqueteSprite_metasprites[0], 1, 0, 5, cpuX, cpuY);
            move_sprite(0,spriteBallX,spriteBallY); // Reposiciona o sprite antes de esperar para evitar que ele fique invisível
            velocityBallX = -1;
            delay(1000); // Pausa pra se preparar
        }

        // Lógica da colisão da bola com a raquete do player
        if (velocityBallX < 0 && spriteBallX <= playerX + 2 && spriteBallX >= playerX - 2) { 
            if (spriteBallY >= (playerY - 20) && spriteBallY <= (playerY + 20)) {
                velocityBallX = -velocityBallX;
                spriteBallX = playerX + 2;
                }}

        // Lógica da colisão da bola com a raquete da CPU
        if (velocityBallX > 0 && spriteBallX <= cpuX && spriteBallX >= cpuX - 4) { 
            if (spriteBallY >= (cpuY - 20) && spriteBallY <= (cpuY + 20)) {
                velocityBallX = -velocityBallX;
                spriteBallX = cpuX - 4;
                }}

        move_sprite(0,spriteBallX,spriteBallY);
        move_metasprite_ex(RaqueteSprite_metasprites[0], 1, 0, 1, playerX, playerY);
        move_metasprite_ex(RaqueteSprite_metasprites[0], 1, 0, 5, cpuX, cpuY);
		// Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }
}