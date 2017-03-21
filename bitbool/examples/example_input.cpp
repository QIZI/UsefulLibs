#include "../bitbool.h"
#include <curses.h>
#include <stdio.h>

BOOL8_REDEF_PREFIX(KEY_PRESS_,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE,
    E,
    R,
    Q
    ) pressed;

void handleKeys(){

    pressed = 0x00;
    char keyPressed = getc(stdin);

    if(keyPressed == 65){
        pressed.KEY_PRESS_UP = true;
    }

    if(keyPressed == 67){
        pressed.KEY_PRESS_RIGHT = true;
    }
    
    if(keyPressed == 66){
        pressed.KEY_PRESS_DOWN = true;
    }

    if(keyPressed == 68){
        pressed.KEY_PRESS_LEFT = true;
    }

    if(keyPressed == 32){
        pressed.KEY_PRESS_SPACE = true;

    }
    if(keyPressed == 'e'){
        pressed.KEY_PRESS_E = true;
        
    }
    if(keyPressed == 'q'){
        pressed.KEY_PRESS_Q = true;
    }

}


int main(){

    initscr();
    cbreak();
    noecho(); 
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
   

    printf("Press 'Q' to quit.\n");
    while(true){
         
         handleKeys();

         if(pressed.KEY_PRESS_UP == true){
             printf("Pressed KEY_UP\n");         }

         if(pressed.KEY_PRESS_DOWN == true){
             printf("Pressed KEY_DOWN\n");
         }

         if(pressed.KEY_PRESS_LEFT == true){
             printf("Pressed KEY_LEFT\n");
         }

         if(pressed.KEY_PRESS_RIGHT == true){
             printf("Pressed KEY_RIGHT\n");
         }

         if(pressed.KEY_PRESS_E == true){
             printf("Pressed KEY_E\n");
         }

         if(pressed.KEY_PRESS_SPACE == true){
             printf("Pressed KEY_SPACE\n");
         }

         if(pressed.KEY_PRESS_Q){
            endwin(); 
            break;
         }


    }

    return 0;
}