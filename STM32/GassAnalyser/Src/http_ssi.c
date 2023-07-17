/*
 * http_ssi.c
 *
 *  Created on: 11-Oct-2021
 *      Author: controllerstech
 */


#include"http_ssi.h"
#include "string.h"
#include "stdio.h"

#include "lwip/tcp.h"
#include "lwip/apps/httpd.h"

#include "stm32f4xx_hal.h"
   
#include <string.h>
#include "ee24.h"
#include "main.h"
#include "cmsis_os.h"

extern float O2, CO2, CO, Temperature;
uint8_t dataWrite1, dataRead1;
/* we will use character "x", "y","z" as tag for SSI */
char const* TAGCHAR[]={"x", "y", "z", "s"};
char const** TAGS=TAGCHAR;

extern void device_reset();

uint16_t ssi_handler (int iIndex, char *pcInsert, int iInsertLen)
{
	switch (iIndex) {
		case 0:
			sprintf(pcInsert, "%d", (int)O2);
			return strlen(pcInsert);
			break;
		case 1:
			sprintf(pcInsert, "%d", (int)CO2);
			return strlen(pcInsert);
			break;
		case 2:
			sprintf(pcInsert, "%d", (int)CO);
			return strlen(pcInsert);
			break;
                case 3:
			sprintf(pcInsert, "%d", (int)Temperature);
			return strlen(pcInsert);
			break;
		default :
			break;
	}

	return 0;
}

/************************ CGI HANDLER ***************************/
const char *CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char *CGIReset_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

const tCGI FORM_CGI = {"/form.cgi", CGIForm_Handler};
const tCGI Reset_CGI = {"/reset.cgi", CGIReset_Handler};


char ipAdress0[3],ipAdress1[3],ipAdress2[3],ipAdress3[3];
char subentMask0[3],subentMask1[3],subentMask2[3],subentMask3[3];
char geteway0[3],geteway1[3],geteway2[3],geteway3[3];

tCGI CGI_TAB[2];

const char *CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	if (iIndex == 0)
	{
		for (int i=0; i<iNumParams; i++)
		{
			if (strcmp(pcParam[i], "Ip0") == 0)  // if the fname string is found
			{
				memset(ipAdress0, '\0', 3);
                                memset(ipAdress1, '\0', 3);
                                memset(ipAdress2, '\0', 3);
                                memset(ipAdress3, '\0', 3);
                                memset(subentMask0, '\0', 3);
                                memset(subentMask1, '\0', 3);
                                memset(subentMask2, '\0', 3);
                                memset(subentMask3, '\0', 3);
                                memset(geteway0, '\0', 3);
                                memset(geteway1, '\0', 3);
                                memset(geteway2, '\0', 3);
                                memset(geteway3, '\0', 3);
                                
				strcpy(ipAdress0, pcValue[i]);
                                dataWrite1 = atoi(ipAdress0);
                                ee24_write(3, &dataWrite1, 1, 100);
                                osDelay(1);
			}
			else if (strcmp(pcParam[i], "Ip1") == 0)  // if the fname string is found
			{
				strcpy(ipAdress1, pcValue[i]);
                                dataWrite1 = atoi(ipAdress1);
                                ee24_write(4, &dataWrite1, 1, 100);
                                osDelay(1);
			}
                        else if (strcmp(pcParam[i], "Ip2") == 0)  // if the fname string is found
			{
				strcpy(ipAdress2, pcValue[i]);
                                dataWrite1 = atoi(ipAdress2);
                                ee24_write(5, &dataWrite1, 1, 100);
                                osDelay(1);
			}
                        else if (strcmp(pcParam[i], "Ip3") == 0)  // if the fname string is found
			{
				strcpy(ipAdress3, pcValue[i]);
                                dataWrite1 = atoi(ipAdress3);
                                ee24_write(6, &dataWrite1, 1, 100);
                                osDelay(1);
			}
                        
                        else if (strcmp(pcParam[i], "Sm0") == 0)  // if the fname string is found
			{
				strcpy(subentMask0, pcValue[i]);
                                dataWrite1 = atoi(subentMask0);
                                ee24_write(7, &dataWrite1, 1, 100);
                                osDelay(1);
			}
                        else if (strcmp(pcParam[i], "Sm1") == 0)  // if the fname string is found
			{
				strcpy(subentMask1, pcValue[i]);
                                dataWrite1 = atoi(subentMask1);
                                ee24_write(8, &dataWrite1, 1, 100);
                                osDelay(1);
			}
                        else if (strcmp(pcParam[i], "Sm2") == 0)  // if the fname string is found
			{
				strcpy(subentMask2, pcValue[i]);
                                dataWrite1 = atoi(subentMask2);
                                ee24_write(9, &dataWrite1, 1, 100);
                                osDelay(1);
			}
                        else if (strcmp(pcParam[i], "Sm3") == 0)  // if the fname string is found
			{
				strcpy(subentMask3, pcValue[i]);
                                dataWrite1 = atoi(subentMask3);
                                ee24_write(10, &dataWrite1, 1, 100);
                                osDelay(1);
			}
                        
                        else if (strcmp(pcParam[i], "gw0") == 0)  // if the fname string is found
			{
				strcpy(geteway0, pcValue[i]);
                                dataWrite1 = atoi(geteway0);
                                ee24_write(11, &dataWrite1, 1, 100);
                                osDelay(1);
			}
                        else if (strcmp(pcParam[i], "gw1") == 0)  // if the fname string is found
			{
				strcpy(geteway1, pcValue[i]);
                                dataWrite1 = atoi(geteway1);
                                ee24_write(12, &dataWrite1, 1, 100);
                                osDelay(1);
			}
                        else if (strcmp(pcParam[i], "gw2") == 0)  // if the fname string is found
			{
				strcpy(geteway2, pcValue[i]);
                                dataWrite1 = atoi(geteway2);
                                ee24_write(13, &dataWrite1, 1, 100);
                                osDelay(1);
			}
                        else if (strcmp(pcParam[i], "gw3") == 0)  // if the fname string is found
			{
				strcpy(geteway3, pcValue[i]);
                                dataWrite1 = atoi(geteway3);
                                ee24_write(14, &dataWrite1, 1, 100);
                                osDelay(1);
			}
		}
                ee24_read(0, &dataRead1, 1, 100);
                osDelay(1);
                ee24_read(3, &dataRead1, 1, 100);
                osDelay(1);
                ee24_read(4, &dataRead1, 1, 100);
                osDelay(1);
                ee24_read(5, &dataRead1, 1, 100);
                osDelay(1);
                ee24_read(6, &dataRead1, 1, 100);
                osDelay(1);
                
                ee24_read(7, &dataRead1, 1, 100);
                osDelay(1);
                ee24_read(8, &dataRead1, 1, 100);
                osDelay(1);
                ee24_read(9, &dataRead1, 1, 100);
                osDelay(1);
                ee24_read(10, &dataRead1, 1, 100);
                osDelay(1);
                
                ee24_read(11, &dataRead1, 1, 100);
                osDelay(1);
                ee24_read(12, &dataRead1, 1, 100);
                osDelay(1);
                ee24_read(13, &dataRead1, 1, 100);
                osDelay(1);
                ee24_read(14, &dataRead1, 1, 100); 
                osDelay(1);
                HAL_NVIC_SystemReset();
	}

	return "/cgiform.html";
}


const char *CGIReset_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	if (iIndex == 1)
	{
            ee24_eraseChip();
            device_reset();
	}

	return "/cgiform.html";
}

void http_server_init (void)
{
	httpd_init();

	http_set_ssi_handler(ssi_handler, (char const**) TAGS, 4);

	CGI_TAB[0] = FORM_CGI;
        CGI_TAB[1] = Reset_CGI;

//	http_set_cgi_handlers (&FORM_CGI, 1);
	http_set_cgi_handlers (CGI_TAB, 2);
}
