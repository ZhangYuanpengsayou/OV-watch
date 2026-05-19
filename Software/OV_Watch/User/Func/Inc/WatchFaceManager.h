#ifndef WATCHFACEMANAGER_H
#define WATCHFACEMANAGER_H

#include "PageManager.h"

#define WF_MAX_COUNT    5
#define WF_EEPROM_ADDR  0x12

void WatchFace_Init(void);
void WatchFace_SwitchToNext(void);
uint8_t WatchFace_GetCurrent(void);
Page_t* WatchFace_GetCurrentPage(void);

#endif
