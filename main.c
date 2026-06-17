#include <gb/gb.h>
#include <stdlib.h>
#include "MyBg.h"
#include "BolaSprite.h"
int spriteX,spriteY;
int8_t velocityX,velocityY;

void main(void)
{

    DISPLAY_ON;

    SHOW_BKG;
    set_bkg_data(0,22,MyBg_tiles);
    set_bkg_tiles(0,0,20,18,MyBg_map);
    delay(3000);
    HIDE_BKG;

    SHOW_SPRITES;
    set_sprite_data(0,1,BolaSprite);
    set_sprite_tile(0,0);
    spriteX=80;
    spriteY=72;

    /* 
    Setando a velocidade do sprite randomizada no início da partida
    Ainda vou precisar randomizar após cada reinício de partida 
    */
    velocityX = -1;
    velocityY = 1;


    // Loop forever
    while(1) {
        spriteX += velocityX;
        spriteY += velocityY;

        if(spriteY < 4) {
            spriteY = 4;
            velocityY = -velocityY; // Inverte a direção vertical
        }
        if(spriteY > 140) {
            spriteY = 140;
            velocityY = -velocityY; // Inverte a direção vertical
        }
        if (spriteX < -4 || spriteX > 164) {
            if (spriteX < -4) {
                // Lógica do gol pro time da direita
            } else {
                // Lógica do gol pro time da esquerda
            }
            spriteX = 80;
            spriteY = 72;
            delay(1000); // Pausa pra mostrar o gol
            move_sprite(0,spriteX+4,spriteY+12); // Reposiciona o sprite antes de esperar para evitar que ele fique invisível
            delay(1000); // Pausa pra se preparar
        }

        move_sprite(0,spriteX+4,spriteY+12);
		// Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }
}