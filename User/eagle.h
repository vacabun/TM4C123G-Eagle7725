#pragma once

#include "User/Basic.h"
#include "driverlib/udma.h"
#include "User/ov7725_reg.h"

#include "User/sccb.h"
#define CAMERA_H 60	
#define CAMERA_W 80
//#define PCLKSize (CAMERA_H * CAMERA_W / 8)

#define ARR_SIZE(a) (sizeof((a))/sizeof(((a)[0])))




typedef struct
{
  uint8_t addr;
  uint8_t val;
}reg_s;


void InitEagle(void);
void InitEagleGPIO(void);
void VSYNCIntHandler(void);
void PCLKIntHandler(void);
uint8_t OV7725_eagle_reg_init(void);
void OV7725_EAGLE_Delay_ms(uint16_t ms);




