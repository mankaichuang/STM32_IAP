#ifndef __UPDATA_H
#define __UPDATA_H	 

#include "sys.h"

#define APPLICATION_ADDRESS     (uint32_t)0x08010000
#define REQUEST_UPDATA 						0xF1
#define UPDATA_REDAY   						0xF2

typedef  void (*pFunction)(void);

extern pFunction JumpToApplication;
extern uint32_t JumpAddress;

extern uint8_t updataPackage;
extern uint8_t updatabuf[2048];
void sendRequest(uint8_t cmd);
void updataFlash(void);

void ASC2BCD(uint8_t *ucASCBuf,uint8_t *ucBCDBuf,uint32_t Len);
		 				    
#endif
