#include <stdio.h>
#include <ctype.h>
#include "cook.h"

void reset(int* target, int* val, int* se)
{
	*target = -1;
	*val = 0;
	*se = -1;
}

int main(void)
{
	int	c;
	int	se;
	int 	target;
	int	val;

	init("/dev/ttyUSB0");

	printf("s:= se, g:= get\n"
		"i:=pump in\n "
		"m:=mixer\n"
		"h=heater\n"
		"c=cooler\n"
		"t=temp (only for get)\n"
		"l=level (only for get)\n"
		"v=tells that now comes an integer\n"
		"example: \"s i 10\" means se pump in to 10\n");

	val = 0;
	while (EOF != (c = getchar())) {
		switch (c) {
			case 's':
				se = 1;		
				break;
			case 'g':
				se = 0;
				break;
			case 'i':
				if (se) 
					target = IN_PUMP;
				else 
					target = IN_PUMP_RATE;
				break;
			case 'o':
				if (se)
					target = OUT_PUMP;
				else
					target = OUT_PUMP_RATE;
				break;
			case 'm':
				if (se) 
					target = MIXER;
				else 
					target = MIXER_RATE;
				break;
			case 'c':
				if (se) 
					target = COOLER;
				else 
					target = COOLER_RATE;
				break;
			case 'h': 
				if (se) 
					target = HEATER;
				else  
					target = HEATER_RATE;
				break;
			case 't':
				if (!se) 
					target = TEMP;
				 else {
					reset(&target, &val, &se);
					fprintf(stderr, "Can't set TEMP\n");
				}
				break;
			case 'l':
				if (!se) 
					target = LEVEL;
				 else {
					fprintf(stderr, "Can't set LEVEL\n");
					reset(&target, &val, &se);
				}
				break;
			case '\n':
				if (se && target != -1) 
					set(target, val);
				else if (target != -1) {
					printf("%d\n", get(target));
				} else  {
					fprintf(stderr, "bad input\n");
					reset(&target, &val, &se);
				}

				val = 0;		
				se = -1;
				target = -1;
				break;
			case ' ':
				break;
			default:
				if (isdigit(c)) {
					val = 10*val + c - '0';
				}  else {
					printf("Expected an integer, it was not\n");
					reset(&target, &val, &se);
				}
		}


	}

	destroy();
	return 0;
}