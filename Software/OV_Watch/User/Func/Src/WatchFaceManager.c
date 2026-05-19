#include "WatchFaceManager.h"
#include "../../GUI_App/Screens/Inc/ui_HomePage.h"
#include "../../GUI_App/Screens/Inc/ui_HomePage2.h"
#include "../../BSP/BL24C02/DataSave.h"
#include "../Tasks/Inc/user_TasksInit.h"
#include "lvgl.h"

static Page_t* WatchFaces[] = {
    &Page_Home,
    &Page_Home2,
};
static uint8_t wf_count = sizeof(WatchFaces) / sizeof(WatchFaces[0]);
static uint8_t current_index = 0;

void WatchFace_Init(void)
{
    uint8_t saved = 0;
    if(SettingGet(&saved, WF_EEPROM_ADDR, 1) == 0 && saved < wf_count)
    {
        current_index = saved;
    }
    else
    {
        current_index = 0;
    }
}

void WatchFace_SwitchToNext(void)
{
    if(wf_count <= 1) return;
    if(PageStack.top != 1) return;

    // deinit current watch face
    if(WatchFaces[current_index]->deinit)
        WatchFaces[current_index]->deinit();

    // switch to next
    current_index = (current_index + 1) % wf_count;

    // replace bottom of page stack
    PageStack.pages[0] = WatchFaces[current_index];

    // init and load new watch face
    if(WatchFaces[current_index]->init)
        WatchFaces[current_index]->init();
    lv_scr_load_anim(*WatchFaces[current_index]->page_obj, LV_SCR_LOAD_ANIM_MOVE_LEFT, 100, 0, true);

    // save to EEPROM
    SettingSave(&current_index, WF_EEPROM_ADDR, 1);
    uint8_t dummy = 0;
    osMessageQueuePut(DataSave_MessageQueue, &dummy, 0, 1);
}

uint8_t WatchFace_GetCurrent(void)
{
    return current_index;
}

Page_t* WatchFace_GetCurrentPage(void)
{
    return WatchFaces[current_index];
}
