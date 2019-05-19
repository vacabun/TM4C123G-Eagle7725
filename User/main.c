
#include "User/Basic.h"

#include "User/Myuart.h"
#include "User/eagle.h"
#include "User/sccb.h"
uint8_t imgbuff[CAMERA_H*CAMERA_W/8] = {0};
uint8_t IMAGE[CAMERA_H][CAMERA_W];
int main(void)
{

    int i = 0;
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_XTAL_16MHZ|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN);

    initUART0();
    InitEagle();
    while(1)
    {

       UARTCharPut(UART0_BASE,0x01);
        UARTCharPut(UART0_BASE,0xfe);
        for (i = 0 ; i < (CAMERA_H*CAMERA_W/8); i++)
        {
            UARTCharPut(UART0_BASE,imgbuff[i]);
        }
        UARTCharPut(UART0_BASE,0xfe);
        UARTCharPut(UART0_BASE,0x01);
    }
}
