#include "displayapp/screens/Dice.h"
#include "displayapp/DisplayApp.h"


uint32_t lv_rand(uint32_t min, uint32_t max);
static void btnEventHandler(lv_obj_t* obj, lv_event_t event);

using namespace Pinetime::Applications::Screens;

Dice::Dice(Controllers::MotionController& motionController, Controllers::Settings& settingsController, Controllers::MotorController& motorController)
      : Screen(), motionController {motionController}, settingsController {settingsController}, motorController {motorController} 
{
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
  lv_obj_set_style_local_text_font(title, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_76);
	updateResult();
}

static uint32_t seed = 0x1234ABCD; /*Seed*/

uint32_t lv_rand(uint32_t min, uint32_t max)
{
    /*Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"*/
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;

    return (seed % (max - min + 1)) + min;
}

void Dice::updateResult() {
  lv_label_set_text_static(title, result);
  lv_obj_align(title, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, -50 + lv_obj_get_width(title) / 2, 0);
}

void Dice::Roll() {
  seed ^= motionController.CurrentShakeSpeed(); // inject randomness into seed
  snprintf(result, sizeof(result), "%d", lv_rand(1,sideCounter.GetValue()));
  updateResult();
  motorController.RunForDuration(30);
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
