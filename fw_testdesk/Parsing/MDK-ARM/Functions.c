#include "Functions.h"
#include "stm32f3xx_hal.h"
#include  <stdbool.h>

uint8_t buffer_analyze(bool flag,uint8_t * buffer[10]){
	uint8_t i;
	uint8_t * cmd[5];
	if(flag==true)
	{
		if(buffer[0]==(uint8_t *)"x")
		{
			for(i=1;i<=5;i++)
			{
				cmd[i-1]=buffer[i];
			}
			if(strcmp(cmd,"aucal")==0);
			if(strcmp(cmd,"getpo")==0);
			if(strcmp(cmd,"go_up")==0);
			if(strcmp(cmd,"go_dw")==0);
			if(strcmp(cmd,"go_to")==0);
		}
	}
}
