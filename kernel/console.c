#include <n7OS/console.h>
#include <n7OS/cpu.h>

uint16_t *scr_tab;
uint16_t curs;

void console_putcurs(uint16_t curs) {
    // positionnement du curseur en machine
    outb(CMD_LOW,PORT_CMD);
    outb(curs&0xf,PORT_DATA);
    outb(CMD_HIGH,PORT_CMD);
    outb(curs>>8,PORT_DATA);
    scr_tab[curs] = CHAR_COLOR<<8|0;
}

void init_console() {
    scr_tab= (uint16_t *) SCREEN_ADDR;
    curs = 0;
    console_putcurs(curs);
}


void console_putchar(const char c) {
    if (c > 31 && c < 127) {
        // affichage d'un caractère et mise à jour de la position du curseur
        scr_tab[curs]= CHAR_COLOR<<8|c;
        curs++;
    } else if (c == 8) {
        // backspace
        curs--;
        scr_tab[curs] = CHAR_COLOR<<8|0;
    } else if (c == 9) {
        // tabulation
        curs = curs+8;
    } else if (c == 10) {
        // retour à la ligne \n
        curs = ((curs/80) + 1) * 80;
    } else if (c == 12) {
        // remise à zéro
        for (int i = 0 ; i < 2000 ; i++) {
            scr_tab[i] = CHAR_COLOR<<8|0;
        }
        curs = 0;
    } else if (c == 13) {
        // retour au début de ligne
        curs = curs - curs%80;
    }

    console_putcurs(curs);
}

void console_putbytes(const char *s, int len) {
    for (int i= 0; i<len; i++) {
        console_putchar(s[i]);
    }
}