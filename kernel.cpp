#include "types.h"
#include "gdt.h"
#include "Hardware/interrupts.h"
#include "Drivers/driver.h"
#include "Drivers/keyboard.h"
#include "Drivers/mouse.h"

using namespace cyanide;
using namespace cyanide::common;
using namespace cyanide::hardware;
using namespace cyanide::drivers;

void kprint(char* str)
{
    // video memory address
    static uint16_t* VidMem = (uint16_t*)0xB8000;
    // the cursor's x and y position
    static uint8_t x = 0, y = 0;

    for (int32_t i = 0; str[i] != '\0'; ++i)
    {
        // check each character
        switch (str[i])
        {
            case '\n':
                // creates a new line
                x = 0;
                y++;
                break;
            default:
                // prints each char from `str`
                VidMem[80 * y + x] = (VidMem[80 * y + x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if (x >= 80)
        {
            x = 0;
            y++;
        }

        if (y >= 25)
        {
            // scrolls up
            for (int y = 1; y < 20; ++y)
            {
                for (int x = 0; x < 80; ++x)
                {
                    unsigned int offset = (y * 80 + x) * 2;
                    unsigned int offset_up = offset - (80 * 2);

                    VidMem[offset_up + 0] = VidMem[offset + 0];
                    VidMem[offset_up + 1] = VidMem[offset + 1];
                    VidMem[offset + 0] = 0;
                    VidMem[offset + 1] = 0;
                }
            }
            x = 0;
        }
    }
}

void kprintHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    kprint(foo);
}

class KprintKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        kprint(foo);
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
        uint16_t* VidMem = (uint16_t*)0xB8000;
        x = 40;
        y = 12;
        VidMem[80*y+x] = (VidMem[80*y+x] & 0x0F00) << 4
                            | (VidMem[80*y+x] & 0xF000) >> 4
                            | (VidMem[80*y+x] & 0x00FF);        
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VidMem = (uint16_t*)0xB8000;
        VidMem[80*y+x] = (VidMem[80*y+x] & 0x0F00) << 4
                            | (VidMem[80*y+x] & 0xF000) >> 4
                            | (VidMem[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VidMem[80*y+x] = (VidMem[80*y+x] & 0x0F00) << 4
                            | (VidMem[80*y+x] & 0xF000) >> 4
                            | (VidMem[80*y+x] & 0x00FF);
    }
    
};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors() 
{
  for(constructor* i = &start_ctors; i != &end_ctors; ++i)
      // invoke each constructor
    (*i)();
}

void clear()
{
    static uint16_t* VidMem = (uint16_t*)0xB8000;
    for(int y = 0; y < 25; y++)
    	for(int x = 0; x < 80; x++) VidMem[80*y+x] = (VidMem[80*y+x] & 0xFF00) | ' ';
}

extern "C" void kmain(void* multiboot_structure)
{
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);
    DriverManager drvmgr;

    clear();

    kprint("[SYS] Initializing Keyboard\n");

    KprintKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvmgr.AddDriver(&keyboard);

    kprint("[SYS] Initializing Mouse\n");

    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
    drvmgr.AddDriver(&mouse);

    kprint("[SYS] [OK] Initialized Input Devices\n");

    kprint("[SYS] Initializing Drivers\n");
    drvmgr.ActivateAll();

    kprint("[SYS] [OK] Initialized Drivers\n");

    kprint("[SYS] Activating Interrupts\n");
    interrupts.Activate();

    kprint("Cyanide has booted successfuly.\n\n");

    // this is what executes in the `on boot` block
    kprint("Hello from Cyanide!\nInput: ");

    // basically a forever loop
    while(true);
}
