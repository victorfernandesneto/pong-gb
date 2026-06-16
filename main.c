#include <gb/gb.h>
#include "MyBg.h"

void main(void)
{
    DISPLAY_ON;

    SHOW_BKG;

    set_bkg_data(0,22,MyBg_tiles);

    set_bkg_tiles(0,0,20,18,MyBg_map);

    // Loop forever
    while(1) {

		// Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }
}