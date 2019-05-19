#pragma once

#include "User/Basic.h"

#include "driverlib/interrupt.h"
#include "utils/uartstdio.h"

void initUART0(void);

void UART0IntHandler(void);
