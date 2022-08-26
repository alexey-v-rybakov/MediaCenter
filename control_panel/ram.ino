extern int __bss_end;
extern void *__brkval;
extern int __heap_start;


int get_free_ram()
{
  int v;
  return (int)&v - (__brkval == 0? (int)&__heap_start : (int) __brkval);  
}

