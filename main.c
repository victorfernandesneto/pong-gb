#include <gb/gb.h>

#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <gbdk/console.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "MyBg.h"
#include "BolaSprite.h"
#include "RaqueteSprite.h"

int spriteBallX,spriteBallY,playerX,playerY,cpuX,cpuY,diffBallPlayer,diffBallCPU;
int8_t velocityBallX,velocityBallY,velocitycpuX=0,velocitycpuY=0,velocityplayerX=0,velocityplayerY=0,cpuTargetOffset=0;
uint8_t joypadPrevious=0,joypadCurrent=0,cpuReactionTimer=0,playerPoints=0,cpuPoints=0;

void atualizar_hud(void) {
    gotoxy(0, 17);
    printf(" P1: %d      CPU: %d ", playerPoints, cpuPoints);
}

void move_all_sprites(void) {
    move_sprite(0,spriteBallX,spriteBallY-16);
    move_metasprite_ex(RaqueteSprite_metasprites[0], 1, 0, 1, playerX, playerY-16);
    move_metasprite_ex(RaqueteSprite_metasprites[0], 1, 0, 5, cpuX, cpuY-16);
}

void reset_positions(void){
    spriteBallX=88;
    spriteBallY=92;
    playerX = 8;
    playerY = 96;
    cpuX = 164;
    cpuY = 96;
    move_all_sprites();
    velocityBallX = -1;
    velocityBallY = 1;
    delay(1000);
}

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

    set_sprite_data(1,2,RaqueteSprite_tiles);

    atualizar_hud();
    reset_positions();

    // Loop forever
    while(1) {
        // Lógica da movimentação do player
        joypadPrevious=joypadCurrent;
        joypadCurrent=joypad();
        
        // Lógica da movimentação do player
        if(joypadCurrent & J_UP) {
            velocityplayerY=-2;
        } else if(joypadCurrent & J_DOWN) {
            velocityplayerY=2;
        } else {
            velocityplayerY=0;
        }
        playerY+=velocityplayerY;
        if(playerY < 48) {
            playerY = 48;
        }
        if(playerY > 144) {
            playerY = 144;
        }

        // Lógica da movimentação da CPU
        if (velocityBallX > 0) {
            if (cpuReactionTimer > 0) {
                cpuReactionTimer--;
            }
            else{
            if (cpuY < spriteBallY + cpuTargetOffset - 3) {
                cpuY += 2;
            } 
            else if (cpuY > spriteBallY + cpuTargetOffset + 3) {
                cpuY -= 2;
        }
            if(cpuY < 48) {
                cpuY = 48;
            }
            if(cpuY > 144) {
                cpuY = 144;
            }
    }}

        // Lógica da movimentação da bola
        spriteBallX += velocityBallX;
        spriteBallY += velocityBallY;

        // Lógica da colisão da bola com as paredes superior e inferior
        if(spriteBallY < 32) {
            spriteBallY = 32;
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
                cpuPoints++;
                atualizar_hud();
            } else {
                // Lógica do gol pro time da esquerda
                playerPoints++;
                atualizar_hud();
            }
            delay(1000);
            reset_positions();
        }


        // Lógica da colisão da bola com a raquete do player
        if (velocityBallX < 0 && spriteBallX <= playerX + 2 && spriteBallX >= playerX - 2) { 
            if (spriteBallY + 8 >= (playerY - 16) && spriteBallY <= (playerY + 16)) {
                velocityBallX = -velocityBallX;
                velocityBallX++;
                spriteBallX = playerX + 2;
                cpuReactionTimer = 10 + (spriteBallY % 11);
                cpuTargetOffset = (spriteBallY % 21) - 10;
                diffBallPlayer=spriteBallY-playerY+4;
                if (diffBallPlayer < -6){
                    velocityBallY = -2;
                }
                else if (diffBallPlayer > 6){
                    velocityBallY = 2;
                }
                else{
                    velocityBallY = (velocityBallY > 0) ? 1:-1;
                }
            }
        }

        // Lógica da colisão da bola com a raquete da CPU
        if (velocityBallX > 0 && spriteBallX <= cpuX && spriteBallX >= cpuX - 4) { 
            if (spriteBallY + 8 >= (cpuY - 16) && spriteBallY <= (cpuY + 16)) {
                velocityBallX = -velocityBallX;
                velocityBallX--;
                spriteBallX = cpuX - 4;
                diffBallCPU=spriteBallY-cpuY+4;
                if (diffBallCPU < -6){
                    velocityBallY = -2;
                }
                else if (diffBallCPU > 6){
                    velocityBallY = 2;
                }
                else{
                    velocityBallY = (velocityBallY > 0) ? 1:-1;
                }
            }
        }

        move_all_sprites();
		// Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }
}