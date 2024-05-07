void kprint(char* str)
{
  unsigned short* VidMem = (unsigned short*)0xB8000;
  for (int i = 0; str[i] != '\0'; ++i)
    VidMem[i] = str[i]
}

void kmain(void* multiboot_structure)
{
  kprint("Hello from Cyanide!");
  while(1);
}
