#include "sccb.h"

static void SCCB_delay(uint16_t i);

static void SCCB_delay(volatile uint16_t time)
{
	while(time)
    time--;
}

void SCCB_GPIO_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, 
						GPIO_PIN_0
						|GPIO_PIN_1);
	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_0,GPIO_PIN_0);
	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_1,GPIO_PIN_1);
	
}

static uint8_t SCCB_Start(void)
{
	SDA_H;
	SCL_H;
	SCCB_DELAY();

	SDA_DDR_I;
	if(!SDA_IN)
	{
		SDA_DDR_O;
		return 0;
	}
	SDA_DDR_O;
	SDA_L;

	SCCB_DELAY();
	SCL_L;

	if(SDA_IN)
	{
		SDA_DDR_O;
		return 0;
	}
	return 1;
}

static void SCCB_Stop(void)
{
	SCL_L;
	SDA_L;
	SCCB_DELAY();
	SCL_H;
	SCCB_DELAY();
	SDA_H;
	SCCB_DELAY();
}

static void SCCB_Ack(void)
{
	SCL_L;
	SCCB_DELAY();
	SDA_L;
	SCCB_DELAY();
	SCL_H;
	SCCB_DELAY();
	SCL_L;
	SCCB_DELAY();
}

static void SCCB_NoAck(void)
{
	SCL_L;
	SCCB_DELAY();
	SDA_H;
	SCCB_DELAY();
	SCL_H;
	SCCB_DELAY();
	SCL_L;
	SCCB_DELAY();
}

static int SCCB_WaitAck(void)
{
	SCL_L;
	SDA_DDR_I;

	SCCB_DELAY();
	SCL_H;

	SCCB_DELAY();
	if(SDA_IN)
	{
		SDA_DDR_O;
		SCL_L;
		return 0;
	}

	SDA_DDR_O;
	SCL_L;
	return 1;
}

static void SCCB_SendByte(uint8_t SendByte)
{
	uint8_t i = 8;
	while(i--)
	{
		if(SendByte&0x80)
			SDA_H;
		else
			SDA_L;
		SendByte<<=1;
		SCCB_DELAY();
		SCL_H;
		SCCB_DELAY();
		SCL_L;
	}
}

static int SCCB_ReceiveByte(void)
{
	uint8_t i = 8;
	uint8_t ReceiveByte=0;
  
	SDA_DDR_I;
	while(i--)
	{
		ReceiveByte<<=1;
		SCL_L;
		SCCB_DELAY();
		SCL_H;
		SCCB_DELAY();
    
		if(SDA_IN)
		ReceiveByte |=0x01;
	}
	SDA_DDR_O;
	SCL_L;
	return ReceiveByte;
}

static int SCCB_WriteByte_one(uint16_t WriteAddress , uint8_t SendByte);

int SCCB_WriteByte(uint16_t WriteAddress , uint8_t SendByte)
{
	uint8_t i = 0;
	while(0==SCCB_WriteByte_one ( WriteAddress, SendByte ))
	{
		i++;
		if(i==20)
			return 0;
	}
	return 1;
}

int SCCB_WriteByte_one(uint16_t WriteAddress , uint8_t SendByte)
{
	if(!SCCB_Start())
		return 0;
	SCCB_SendByte(DEV_ADR);
	if(!SCCB_WaitAck())
	{
		SCCB_Stop();
		return 0;
	}
	SCCB_SendByte((uint8_t)(WriteAddress&0x00ff));
	SCCB_WaitAck();
	SCCB_SendByte(SendByte);
	SCCB_WaitAck();
	SCCB_Stop();
	return 1;
}

static int SCCB_ReadByte_one(uint8_t *pBuffer , uint16_t length , uint8_t ReadAddress);

int SCCB_ReadByte(uint8_t *pBuffer , uint8_t length , uint8_t ReadAddress)
{
	uint8_t i=0;
	while(0==SCCB_ReadByte_one(pBuffer , length , ReadAddress))
	{
		i++;
		if(i==30)
		return 0;
	}
	return 1;
}

int SCCB_ReadByte_one(uint8_t *pBuffer , uint16_t length , uint8_t ReadAddress)
{
	if(!SCCB_Start())
	{
		return 0;
	}
	SCCB_SendByte(DEV_ADR);
	if(!SCCB_WaitAck())
	{
		SCCB_Stop();
		return 0;
	}
	SCCB_SendByte(ReadAddress);
	SCCB_WaitAck();
	SCCB_Stop();

	if(!SCCB_Start())
		return 0;
	SCCB_SendByte(DEV_ADR+1);

	if(!SCCB_WaitAck())
	{
		SCCB_Stop();
		return 0;
	}
	while(length)
	{
		*pBuffer=SCCB_ReceiveByte();
		if(length==1)
			SCCB_NoAck();
		else
			SCCB_Ack();
		pBuffer++;
		length--;
	}
	SCCB_Stop();
	return 1;
}
