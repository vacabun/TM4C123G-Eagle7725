#ifndef _SCCB_H_
#define _SCCB_H_

#include "User/Basic.h"




#define SCL_H      HWREG(GPIO_PORTE_BASE + (GPIO_O_DATA + (GPIO_PIN_0 << 2))) = GPIO_PIN_0
#define SCL_L      HWREG(GPIO_PORTE_BASE + (GPIO_O_DATA + (GPIO_PIN_0 << 2))) = ~GPIO_PIN_0
#define SCL_DDR_O  GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0)
#define SCL_DDR_I  GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0)

#define SDA_H      HWREG(GPIO_PORTE_BASE + (GPIO_O_DATA + (GPIO_PIN_1 << 2))) = GPIO_PIN_1
#define SDA_L      HWREG(GPIO_PORTE_BASE + (GPIO_O_DATA + (GPIO_PIN_1 << 2))) = ~GPIO_PIN_1
#define SDA_DDR_O  GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1)
#define SDA_DDR_I  GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1)
#define SDA_IN     GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_1)

#define ADDR_OV7725  0x42

#define DEV_ADR    ADDR_OV7725

#define SCCB_DELAY()   SCCB_delay(400)

extern void SCCB_GPIO_Init(void);
extern int SCCB_WriteByte(uint16_t WriteAddress , uint8_t SendByte);
extern int SCCB_ReadByte(uint8_t *pBuffer , uint8_t length , uint8_t ReadAddress);

#endif
