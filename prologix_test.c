// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project includes
#include "serial.h"
#include "prologix.h"

// Module include

int main()
{
	prologix *	p;
	char sstr[161];
	char * s = (char *)&sstr;
	int addr1v[3] = { 4, 5, 5};
	int addr2v[3] = {-1, 97, 105};

	p = prologix_create();
	prologix_open(p, "/dev/ttyUSB0");
	prologix_set_debug(p, 0);
	prologix_set_eoi(p, 1);
	prologix_set_eos(p, 1);
	//prologix_write(p, 10, "*rst\n");
	prologix_write(p, 3, "*idn?\n");
	prologix_read(p, 3, s, 160);
	printf("%s", s);
	
	prologix_write(p, 3, "MEAS:FREQ?\n");
	prologix_read(p, 3, s, 160);
	printf("%s", s);
	prologix_trg(p, 3, (int *)&addr1v, (int *)&addr2v);

	prologix_free(p);
	return 0;
}
