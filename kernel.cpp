void kprint(char* str)
{
    static unsigned short* VidMem = (unsigned short*)0xB8000;
    static unsigned char x = 0, y = 0;

    for (int i = 0; str[i] != '\0'; ++i)
    {
        switch (str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
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

void kmain(void* multiboot_structure)
{
    kprint("Hello from Cyanide!");
    while (1);
}
