#include <nes.h>
#include <string.h>

#define AIR   0x00
#define DIRT  0x01
#define GRASS 0x02
#define STONE 0x03
#define WOOD  0x04
#define CURSOR 0x0F

unsigned char world[32 * 16];
unsigned char cx = 16, cy = 8;
unsigned char sel = GRASS;
unsigned char old_pad = 0, delay = 0;

/* cc65 expects exactly 16 bytes for bg palette */
const unsigned char bg_pal[] = {
    0x0F, 0x00, 0x16, 0x20,  /* Pal 0: Sky */
    0x0F, 0x16, 0x0F, 0x0F,  /* Pal 1: Dirt */
    0x0F, 0x1A, 0x0F, 0x0F,  /* Pal 2: Grass */
    0x0F, 0x00, 0x10, 0x0F   /* Pal 3: Stone/Wood */
};

void update_tile(unsigned char x, unsigned char y, unsigned char t) {
    world[y * 32 + x] = t;
    ppu_off();
    vram_adr(0x2000 + (y * 32) + x);
    vram_put(t);
    ppu_on_all();
}

int main(void) {
    unsigned char x;
    unsigned char pad, new;

    pal_bg(bg_pal);
    memset(world, AIR, sizeof(world));

    /* Clear name table */
    ppu_off();
    vram_adr(0x2000);
    for (x = 0; x < 32 * 16; ++x) {
        vram_put(AIR);
    }
    ppu_on_all();

    /* Generate flat terrain */
    for (x = 0; x < 32; ++x) {
        update_tile(x, 14, GRASS);
        update_tile(x, 15, DIRT);
    }

    /* Draw initial cursor */
    update_tile(cx, cy, CURSOR);

    while (1) {
        pad = pad_poll(0);
        new = pad & ~old_pad;
        
        if (delay > 0) {
            delay--;
        } else {
            delay = 4;
        }

        if (delay == 0) {
            if (new & PAD_UP && cy > 0) {
                update_tile(cx, cy, world[cy * 32 + cx]); /* Restore old tile */
                cy--;
                update_tile(cx, cy, CURSOR);
            }
            if (new & PAD_DOWN && cy < 15) {
                update_tile(cx, cy, world[cy * 32 + cx]);
                cy++;
                update_tile(cx, cy, CURSOR);
            }
            if (new & PAD_LEFT && cx > 0) {
                update_tile(cx, cy, world[cy * 32 + cx]);
                cx--;
                update_tile(cx, cy, CURSOR);
            }
            if (new & PAD_RIGHT && cx < 31) {
                update_tile(cx, cy, world[cy * 32 + cx]);
                cx++;
                update_tile(cx, cy, CURSOR);
            }
        }

        if (new & PAD_A) update_tile(cx, cy, sel);
        if (new & PAD_B) update_tile(cx, cy, AIR);
        if (new & PAD_SELECT) {
            sel++;
            if (sel > WOOD) sel = DIRT;
        }

        old_pad = pad;
        ppu_wait_nmi();
    }
    return 0;
}
