#include "../bitbool.h"
#include <stdio.h>
#define MASK_VISIBLE_MOVABLE (0x0A)

class EXAMPLE_Window{
    public:
    typedef BOOL8_REDEF_PREFIX(is,//prefix
        Enabled,
        Visible,
        Resizable,
        Movable,
        Focused
    ) windowState; 
    

    EXAMPLE_Window(){
        wState = 0x00; // set all bits to 0
        wState.isVisible = true;
        wState.isEnabled = true;
        wState.isMovable = true;
    }

    windowState getWindowState(){
        
        return wState;
    }

    void handle(){

       //set negate state for each bit, this method has big performance cost
       for(auto i = 0; i < wState.size(); i++){
            wState(i, !wState[i]);
       }

       // this can be done by
       wState = ~wState;
       // with no overhead

       

       wState.isVisible = wState.isFocused;
        
    }
    uint8_t getRaw(){
        return wState; // auto conversion to regular 8 bit int/char
    }
    private:
    windowState wState;
};







int main(){
    EXAMPLE_Window win;

    BOOL8 b;

    b = win.getWindowState(); // convert BOOL8_PREF_REDEF to regular BOOL8
    b.b2 = false;
        
    if((win.getWindowState() & (uint8_t)MASK_VISIBLE_MOVABLE) != 0){// old-style C masks
        
    }

    printf("Is window visible ? %d\n", win.getWindowState().isVisible);// accesing the bit-member of the type/object
    win.handle();
    win.handle();
    win.handle();
    win.handle();
    printf("Is window visible ? %d\n", win.getWindowState().isVisible);
    
    //you can use bitbool types as any other languages
    unsigned char c = win.getRaw(); 
    b = c;
    c = b;
    
    return 0;
}