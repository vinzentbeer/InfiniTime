#include "displayapp/screens/Dice.h"
#include "displayapp/DisplayApp.h"


uint32_t lv_rand(uint32_t min, uint32_t max);
static void btnEventHandler(lv_obj_t* obj, lv_event_t event);

using namespace Pinetime::Applications::Screens;

Dice::Dice() : Screen() {

	sideCounter.Create();
	lv_obj_align(sideCounter.GetObject(), nullptr, LV_ALIGN_IN_LEFT_MID, 0, 0);

	btnRoll = lv_btn_create(lv_scr_act(), nullptr);
	btnRoll->user_data = this;
	lv_obj_set_size(btnRoll, 100, 100);
	lv_obj_align(btnRoll, nullptr, LV_ALIGN_IN_RIGHT_MID, 0, 0);
	lv_obj_set_style_local_radius(btnRoll, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	lv_obj_set_style_local_bg_color(btnRoll, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
	lv_obj_set_event_cb(btnRoll, btnEventHandler);

	title = lv_label_create(lv_scr_act(), nullptr);
	lv_label_set_text_static(title, ":");
	lv_obj_set_style_local_text_font(title, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_76);
	//lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
	lv_obj_align(title, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, -50+(lv_obj_get_width(title)>>1), 0);
    //lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
	memset(result, 0, 8);

}
uint32_t lv_rand(uint32_t min, uint32_t max)
{
    static uint32_t a = 0x1234ABCD; /*Seed*/

    /*Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"*/
    uint32_t x = a;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    a = x;

    return (a % (max - min + 1)) + min;
}
void Dice::Roll() {
    uint32_t rand_num = lv_rand(1,sideCounter.GetValue());
    snprintf(result,8, ">%d<", rand_num);
    lv_label_set_text_static(title, result);
	lv_obj_align(title, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, -50+(lv_obj_get_width(title)>>1), 0);
}

static void btnEventHandler(lv_obj_t* obj, lv_event_t event) {
  auto* screen = static_cast<Dice*>(obj->user_data);
  if (event == LV_EVENT_PRESSED) {
    //screen->ButtonPressed();
    	screen->Roll();

  } else if (event == LV_EVENT_RELEASED || event == LV_EVENT_PRESS_LOST) {
    //screen->MaskReset();

  } else if (event == LV_EVENT_SHORT_CLICKED) {
	
  }
}



Dice::~Dice() {
  lv_obj_clean(lv_scr_act());
}



