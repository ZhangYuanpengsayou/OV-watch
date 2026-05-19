#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_HomePage2.h"
#include "../Inc/ui_MenuPage.h"
#include "../../../Func/Inc/HWDataAccess.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_HomePage2;
lv_obj_t * ui_HP2_TimeLabel;
lv_obj_t * ui_HP2_DateLabel;
lv_obj_t * ui_HP2_DayLabel;
lv_obj_t * ui_HP2_BatIcon;
lv_obj_t * ui_HP2_BatLabel;
lv_obj_t * ui_HP2_StepIcon;
lv_obj_t * ui_HP2_StepLabel;
lv_obj_t * ui_HP2_HRIcon;
lv_obj_t * ui_HP2_HRLabel;

lv_timer_t * ui_HomePage2Timer;

static uint8_t hp2_hour = 11;
static uint8_t hp2_min = 59;
static uint8_t hp2_month = 11;
static uint8_t hp2_day = 8;
static uint8_t hp2_weekday = 2;
static uint8_t hp2_bat = 70;
static uint16_t hp2_steps = 0;
static uint8_t hp2_hr = 75;

static const char * hp2_Days[7] = {"Mon.", "Tue.", "Wed.", "Thu.", "Fri.", "Sat.", "Sun."};

///////////////////// Page Manager //////////////////
Page_t Page_Home2 = {ui_HomePage2_screen_init, ui_HomePage2_screen_deinit, &ui_HomePage2};

///////////////////// Timer //////////////////
static void HomePage2_timer_cb(lv_timer_t * timer)
{
    uint8_t buf[10];
    if(Page_Get_NowPage()->page_obj == &ui_HomePage2)
    {
        HW_DateTimeTypeDef dt;
        HWInterface.RealTimeClock.GetTimeDate(&dt);

        if(hp2_hour != dt.Hours || hp2_min != dt.Minutes)
        {
            hp2_hour = dt.Hours;
            hp2_min = dt.Minutes;
            sprintf(buf, "%2d:%02d", hp2_hour, hp2_min);
            lv_label_set_text(ui_HP2_TimeLabel, buf);
        }

        if(hp2_day != dt.Date || hp2_month != dt.Month)
        {
            hp2_month = dt.Month;
            hp2_day = dt.Date;
            hp2_weekday = dt.WeekDay;
            sprintf(buf, "%2d-%02d  %s", hp2_month, hp2_day, hp2_Days[hp2_weekday - 1]);
            lv_label_set_text(ui_HP2_DateLabel, buf);
        }

        if(hp2_bat != HWInterface.Power.power_remain)
        {
            hp2_bat = HWInterface.Power.power_remain;
            sprintf(buf, "%d%%", hp2_bat);
            lv_label_set_text(ui_HP2_BatLabel, buf);
        }

        if(hp2_steps != HWInterface.IMU.Steps)
        {
            hp2_steps = HWInterface.IMU.Steps;
            sprintf(buf, "%d", hp2_steps);
            lv_label_set_text(ui_HP2_StepLabel, buf);
        }

        if(hp2_hr != HWInterface.HR_meter.HrRate)
        {
            hp2_hr = HWInterface.HR_meter.HrRate;
            sprintf(buf, "%d", hp2_hr);
            lv_label_set_text(ui_HP2_HRLabel, buf);
        }
    }
}

///////////////////// EVENT //////////////////
static void ui_event_HomePage2(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_GESTURE)
    {
        if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
        {
            Page_Load(&Page_Menu);
        }
    }
}

///////////////////// SCREEN INIT //////////////////
void ui_HomePage2_screen_init(void)
{
    uint8_t buf[10];

    ui_HomePage2 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_HomePage2, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_HomePage2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    // time label (large, centered)
    ui_HP2_TimeLabel = lv_label_create(ui_HomePage2);
    lv_obj_set_width(ui_HP2_TimeLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_HP2_TimeLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_HP2_TimeLabel, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_HP2_TimeLabel, -25);
    sprintf(buf, "%2d:%02d", hp2_hour, hp2_min);
    lv_label_set_text(ui_HP2_TimeLabel, buf);
    lv_obj_set_style_text_font(ui_HP2_TimeLabel, &ui_font_Cuyuan80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_HP2_TimeLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    // date label (below time)
    ui_HP2_DateLabel = lv_label_create(ui_HomePage2);
    lv_obj_set_width(ui_HP2_DateLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_HP2_DateLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_HP2_DateLabel, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_HP2_DateLabel, 50);
    sprintf(buf, "%2d-%02d  %s", hp2_month, hp2_day, hp2_Days[hp2_weekday - 1]);
    lv_label_set_text(ui_HP2_DateLabel, buf);
    lv_obj_set_style_text_font(ui_HP2_DateLabel, &ui_font_Cuyuan24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_HP2_DateLabel, lv_color_hex(0xFF461E), LV_PART_MAIN | LV_STATE_DEFAULT);

    // battery icon + label (top-left)
    ui_HP2_BatIcon = lv_label_create(ui_HomePage2);
    lv_obj_set_width(ui_HP2_BatIcon, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_HP2_BatIcon, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_HP2_BatIcon, 5);
    lv_obj_set_y(ui_HP2_BatIcon, 5);
    lv_label_set_text(ui_HP2_BatIcon, "");
    lv_obj_set_style_text_color(ui_HP2_BatIcon, lv_color_hex(0x19C819), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_HP2_BatIcon, &ui_font_iconfont30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_HP2_BatLabel = lv_label_create(ui_HomePage2);
    lv_obj_set_width(ui_HP2_BatLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_HP2_BatLabel, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_HP2_BatLabel, 38);
    lv_obj_set_y(ui_HP2_BatLabel, 10);
    sprintf(buf, "%d%%", hp2_bat);
    lv_label_set_text(ui_HP2_BatLabel, buf);
    lv_obj_set_style_text_font(ui_HP2_BatLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_HP2_BatLabel, lv_color_hex(0x19C819), LV_PART_MAIN | LV_STATE_DEFAULT);

    // steps icon + label (bottom area)
    ui_HP2_StepIcon = lv_label_create(ui_HomePage2);
    lv_obj_set_width(ui_HP2_StepIcon, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_HP2_StepIcon, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_HP2_StepIcon, -80);
    lv_obj_set_y(ui_HP2_StepIcon, 110);
    lv_obj_set_align(ui_HP2_StepIcon, LV_ALIGN_CENTER);
    lv_label_set_text(ui_HP2_StepIcon, "");
    lv_obj_set_style_text_color(ui_HP2_StepIcon, lv_color_hex(0x3278FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_HP2_StepIcon, &ui_font_iconfont30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_HP2_StepLabel = lv_label_create(ui_HomePage2);
    lv_obj_set_width(ui_HP2_StepLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_HP2_StepLabel, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_HP2_StepLabel, -45);
    lv_obj_set_y(ui_HP2_StepLabel, 113);
    lv_obj_set_align(ui_HP2_StepLabel, LV_ALIGN_CENTER);
    sprintf(buf, "%d", hp2_steps);
    lv_label_set_text(ui_HP2_StepLabel, buf);
    lv_obj_set_style_text_font(ui_HP2_StepLabel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_HP2_StepLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    // heart rate icon + label (bottom area)
    ui_HP2_HRIcon = lv_label_create(ui_HomePage2);
    lv_obj_set_width(ui_HP2_HRIcon, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_HP2_HRIcon, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_HP2_HRIcon, 40);
    lv_obj_set_y(ui_HP2_HRIcon, 110);
    lv_obj_set_align(ui_HP2_HRIcon, LV_ALIGN_CENTER);
    lv_label_set_text(ui_HP2_HRIcon, "");
    lv_obj_set_style_text_color(ui_HP2_HRIcon, lv_color_hex(0xE11432), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_HP2_HRIcon, &ui_font_iconfont32, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_HP2_HRLabel = lv_label_create(ui_HomePage2);
    lv_obj_set_width(ui_HP2_HRLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_HP2_HRLabel, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_HP2_HRLabel, 80);
    lv_obj_set_y(ui_HP2_HRLabel, 113);
    lv_obj_set_align(ui_HP2_HRLabel, LV_ALIGN_CENTER);
    sprintf(buf, "%d", hp2_hr);
    lv_label_set_text(ui_HP2_HRLabel, buf);
    lv_obj_set_style_text_font(ui_HP2_HRLabel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_HP2_HRLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    // events
    lv_obj_add_event_cb(ui_HomePage2, ui_event_HomePage2, LV_EVENT_ALL, NULL);

    // timer
    ui_HomePage2Timer = lv_timer_create(HomePage2_timer_cb, 500, NULL);
}

///////////////////// SCREEN DEINIT //////////////////
void ui_HomePage2_screen_deinit(void)
{
    lv_timer_del(ui_HomePage2Timer);
}
