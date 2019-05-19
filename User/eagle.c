#include "User/eagle.h"
uint8_t pui8ControlTable[1024];

//uint32_t g_ui32MemXferCount = 0;
//uint32_t g_ui32BadISR = 0;
extern uint8_t imgbuff[CAMERA_H*CAMERA_W/8];
//extern uint8_t IMAGE[CAMERA_H][CAMERA_W];

reg_s OV7725_eagle_reg[] =
{
    {OV7725_COM4, 0xC1},                                //50 hz
    {OV7725_CLKRC, 0x00},
    {OV7725_COM2, 0x03},
    {OV7725_COM3, 0xD0},
    {OV7725_COM7, 0x40},
    {OV7725_HSTART, 0x3F},
    {OV7725_HSIZE, 0x50},
    {OV7725_VSTRT, 0x03},
    {OV7725_VSIZE, 0x78},
    {OV7725_HREF, 0x00},
    {OV7725_SCAL0, 0x0A},
    {OV7725_AWB_Ctrl0, 0xE0},
    {OV7725_DSPAuto, 0xff},
    {OV7725_DSP_Ctrl2, 0x0C},
    {OV7725_DSP_Ctrl3, 0x00},
    {OV7725_DSP_Ctrl4, 0x00},

#if (CAMERA_W == 80)
    {OV7725_HOutSize, 0x14},
#elif (CAMERA_W == 160)
    {OV7725_HOutSize, 0x28},
#elif (CAMERA_W == 240)
    {OV7725_HOutSize, 0x3c},
#elif (CAMERA_W == 320)
    {OV7725_HOutSize, 0x50},
#else

#endif

#if (CAMERA_H == 60 )
    {OV7725_VOutSize, 0x1E},
#elif (CAMERA_H == 120 )
    {OV7725_VOutSize, 0x3c},
#elif (CAMERA_H == 180 )
    {OV7725_VOutSize, 0x5a},
#elif (CAMERA_H == 240 )
    {OV7725_VOutSize, 0x78},
#else

#endif

    {OV7725_EXHCH, 0x00},
    {OV7725_GAM1, 0x0c},
    {OV7725_GAM2, 0x16},
    {OV7725_GAM3, 0x2a},
    {OV7725_GAM4, 0x4e},
    {OV7725_GAM5, 0x61},
    {OV7725_GAM6, 0x6f},
    {OV7725_GAM7, 0x7b},
    {OV7725_GAM8, 0x86},
    {OV7725_GAM9, 0x8e},
    {OV7725_GAM10, 0x97},
    {OV7725_GAM11, 0xa4},
    {OV7725_GAM12, 0xaf},
    {OV7725_GAM13, 0xc5},
    {OV7725_GAM14, 0xd7},
    {OV7725_GAM15, 0xe8},
    {OV7725_SLOP, 0x20},
    {OV7725_LC_RADI, 0x00},
    {OV7725_LC_COEF, 0x13},
    {OV7725_LC_XC, 0x08},
    {OV7725_LC_COEFB, 0x14},
    {OV7725_LC_COEFR, 0x17},
    {OV7725_LC_CTR, 0x05},
    {OV7725_BDBase, 0x99},
    {OV7725_BDMStep, 0x03},
    {OV7725_SDE, 0x04},
    {OV7725_BRIGHT, 0x00},
    {OV7725_CNST, 0x4B},
    {OV7725_SIGN, 0x06},
    {OV7725_UVADJ0, 0x11},
    {OV7725_UVADJ1, 0x02},
};



//场中断
void VSYNCIntHandler(void)
{
    //UARTCharPut(UART0_BASE,0x33);
    uDMAChannelTransferSet(UDMA_CHANNEL_USBEP3RX  | UDMA_PRI_SELECT,
                           UDMA_MODE_BASIC, (void *)(GPIO_PORTB_BASE+GPIO_O_DATA+(0xff<<2)), imgbuff,
                           sizeof(imgbuff));
    uDMAChannelEnable(UDMA_CHANNEL_USBEP3RX);
    GPIOIntClear(GPIO_PORTE_BASE,GPIO_INT_PIN_2);

//    GPIOIntDisable(GPIO_PORTE_BASE,GPIO_PIN_2);	    //关闭PE2中断
//    int i;
//    for (i = 0 ; i < (CAMERA_H*CAMERA_W/8); i++)
//        UARTCharPut(UART0_BASE,imgbuff[i]);
//    GPIOIntEnable(GPIO_PORTE_BASE,GPIO_PIN_2);	    //使能PE2中断
}

//帧中断
void PCLKIntHandler(void)
{
    uDMAIntClear(UDMA_CHANNEL_USBEP3RX);
	 //UARTCharPut(UART0_BASE,0x34);
}


void InitEagleGPIO(void)
{
    //初始化数据口
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    //PD解锁
    HWREG(GPIO_PORTB_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTB_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTB_BASE + GPIO_O_LOCK) = 0;
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
//    GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);//配置F0为上拉电阻 输出电流能力2mA

    //初始化场中断
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    //PE解锁
    HWREG(GPIO_PORTE_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTE_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTE_BASE + GPIO_O_LOCK) = 0;
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2);
    GPIOIntTypeSet(GPIO_PORTE_BASE,GPIO_PIN_2,GPIO_RISING_EDGE);	//设置PE2中断类型
    GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_2,GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);//配置F0为上拉电阻 输出电流能力2mA
    GPIOIntEnable(GPIO_PORTE_BASE,GPIO_PIN_2);	    //使能PE2中断
    GPIOIntRegister(GPIO_PORTE_BASE,VSYNCIntHandler);  //为PE2注册一个中断处理句柄


    //初始化帧DMA
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    //PA解锁
    HWREG(GPIO_PORTA_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTA_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTA_BASE + GPIO_O_LOCK) = 0;
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_6,GPIO_FALLING_EDGE);	//设置PE3中断类型
    GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_6,GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);//配置F0为上拉电阻 输出电流能力2mA
    //GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_6);	    //使能PE3中断
    //GPIOIntRegister(GPIO_PORTA_BASE, PCLKIntHandler);      //GPIO注册中断--PCLKIntHandler
    GPIODMATriggerEnable(GPIO_PORTA_BASE,GPIO_PIN_6);       //设置PF0为DMA触发源
    uDMAIntRegister(UDMA_CHANNEL_USBEP3RX,PCLKIntHandler);
	
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    uDMAEnable();
	uDMAChannelAssign(UDMA_CH4_GPIOA);
    IntEnable(INT_UDMA);
    uDMAControlBaseSet(&pui8ControlTable[0]);
    //uDMAChannelAttributeDisable(UDMA_CHANNEL_USBEP3RX,UDMA_ATTR_ALL);
    uDMAChannelAttributeDisable(UDMA_CHANNEL_USBEP3RX,
                                UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |
                                UDMA_ATTR_HIGH_PRIORITY |
                                UDMA_ATTR_REQMASK);
    //uDMAChannelControlSet（uint32_t ui32ChannelStructIndex，uint32_t ui32Control）
    //设置uDMA通道控制结构的控制参数。这些参数是通常不会经常改变。
    //ui32Control是几个控制值的逻辑或，用于设置通道的控制参数。
    uDMAChannelControlSet(UDMA_CHANNEL_USBEP3RX  | UDMA_PRI_SELECT,	            //UDMA_CHANNEL_ETH0TX | UDMA_PRI_SELECT
                          //许多通道参数逻辑或UDMA_PRI_SELECT  UDMA_ALT_SELECT
                          //选择主要或备用控制结构。
                          //对于基本和自动传输模式，只有主控制结构需要。
                          //只有Pingpong的复杂传输模式才需要备用控制结构。
                          UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 );
    //ui32Control参数是五个值的逻辑或：
    //1数据大小，2源地址增量，3目标地址增量，4仲裁大小 ,5使用突发标志。
    uDMAChannelTransferSet(UDMA_CHANNEL_USBEP3RX  | UDMA_PRI_SELECT,
                           UDMA_MODE_AUTO, (void *)(GPIO_PORTB_BASE+GPIO_O_DATA+(0xff<<2)), imgbuff,
                           sizeof(imgbuff));
    //uDMAChannelAssign(UDMA_CH4_GPIOA);
    uDMAChannelEnable(UDMA_CHANNEL_USBEP3RX);
}

void InitEagle(void)
{

    while(OV7725_eagle_reg_init() == 0);
    InitEagleGPIO();
}
void OV7725_EAGLE_Delay_ms(uint16_t ms)
{
    uint16_t i,j;
    for(i=0; i<ms; i++)
        for(j=0; j<ms; j++);
}

uint8_t OV7725_eagle_cfgnum = ARR_SIZE(OV7725_eagle_reg);

uint8_t OV7725_eagle_reg_init(void)
{
    uint16_t i = 0;
    uint8_t Sensor_IDCode = 0;
    SCCB_GPIO_Init();

    if( 0 == SCCB_WriteByte ( OV7725_COM7, 0x80 ) ) /*??sensor */
    {
        return 0 ;
    }

    OV7725_EAGLE_Delay_ms(800);

    if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, OV7725_VER ) )    /* ??sensor ID?*/
    {
        return 0;
    }
    if(Sensor_IDCode == OV7725_ID)
    {
        for( i = 0 ; i < OV7725_eagle_cfgnum ; i++ )
        {
            if( 0 == SCCB_WriteByte(OV7725_eagle_reg[i].addr, OV7725_eagle_reg[i].val) )
            {
                return 0;
            }
        }
    }
    else
    {
        return 0;
    }
    return 1;
}

