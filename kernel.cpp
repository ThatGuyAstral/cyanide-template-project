void kprint(char* str)
{
    // video memory address
    static unsigned short* VidMem = (unsigned short*)0xB8000;
    // the cursor's x and y position
    static unsigned char x = 0, y = 0;

    for (int i = 0; str[i] != '\0'; ++i)
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
        }
    }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors() 
{
  for(constructor* i = &start_ctors; i != &end_ctors; ++i)
      // invoke each constructor
    (*i)();
}

// this is what executes in the `on boot` block
void kmain(void* multiboot_structure)
{
    kprint("Hello from Cyanide!");

    while(true);
}
