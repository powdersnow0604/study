#include "UI.h"


void bootloader(void);


int main(void)
{
	bootloader();
	mainloop();
	return 0;
}


void bootloader(void)
{
	loadAuxmem();
	init_filesystem();
}