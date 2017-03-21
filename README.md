# UsefulLibs
Libraries and Header files that I created during other projects


## Bitbool
Header only library which provides types with more abstract way of accessing the bits [BOOL8, BOOL16, BOOL32].

Features:
- Abstract access to bits as members of the struct/class
- Possibility to rename the bit-members and their prefixes.
- Bit-wise, aritmethic and conditional operators are overloaded to feel like you are woriking with basic types.
- Compatibility with basic types like char, short and int. 

### Example:

```c++
#include "bitbool.h"

int main(){
    // usual BOOLX type
    BOOL8 b = 0x00; //it's possible to assign default value.
    b.b0 = true;
    b.b4 = false;
    b.b7 = true;
    b.b4 = (b.b3 || b.b0 ) // value of b.b3 is in this prat of code 0, because we assigned it in declaration.



    //now what if we wanted to change the name of bits instead of b0...b7, to better memorize what each bit state is used for.
    //solution to that is using macro BOOLX type which is included in library, and only tested with GCC/g++ and CLANG/clang++.

    BOOL8_REDEFINE( //BOOL8_REDEF will do the same
        Stared,
        Finished,
        Opened,
        Busy
        //macro is also overloaded so we do not need to rename/redefine all bit names
    )br = 0x00;
    

    //default prefix ('b') will stay unchanged

    br.bStarted = true; 
    br.bOpened = false;

    if(br.bBusy){
        br.bOpened = true;
    }

    //we now can go even further and aslo use rename the default prefix

    BOOL8_REDEFINE_PREFIX(//BOOL8_REDEF_PREF, BOOL8_REDEFINE_PREF, BOOL8_REDEF_PREFIX will do the same
        is,//first parameter is name of prefix
        Enabled,
        Visible,
        Movable,
        Alocated,
        Error
    )brp = 0x00;

    brp.isEnabled = true;
    brp.isVisible = true;

    // everything is the same as with previous types
    // you can copy between these types(and also basic types) without manual conversion.

    b = brp;
    br = b;
    unsigned char ch = br; 
    brp ~= ch; 
}

```