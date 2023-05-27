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
	//debug
  dbg_shake_speed = lv_label_create(lv_scr_act(), nullptr);
  
  updateResult();

  

  refreshTask = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}

int roll_time = 0;
int frame = 0;

int rumble[][2] = {
  {50, 270},
  {40,200},
  {20,100},
  {20,90},
  {10,70},
  {10,70},
};

static uint32_t seed = 0x1234ABCD; /*Seed*/

void Dice::Refresh() {
  //if (!isRolling && (motionController.CurrentShakeSpeed() > settingsController.GetShakeThreshold())) {
  if (state == not_rolling && motionController.ShouldShakeWake(settingsController.GetShakeThreshold())) {
    state = currently_rolling;
  }
  if (state == currently_rolling) {
    if (rumble[frame][1] <= roll_time) {
      motorController.RunForDuration(rumble[frame][1]);
      roll_time -= rumble[frame][0];
      ++frame;
    }
    roll_time += 20;
    if (frame > 6) {
      state = not_rolling;
      roll_time = 0;
      frame = 0;
    }
    seed ^= motionController.CurrentShakeSpeed(); // inject randomness into seed
    snprintf(result, sizeof(result), "%ld", lv_rand(1,sideCounter.GetValue()));
    updateResult();
  }
}



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
    
    //debug
    snprintf(wie_du_willst, sizeof(wie_du_willst), "%ld",  motionController.CurrentShakeSpeed() + abs(motionController.X()) + abs(motionController.Y()) + abs(motionController.Z()));

    lv_obj_align(dbg_shake_speed, lv_scr_act(),  LV_ALIGN_IN_TOP_RIGHT, 0, 0);
    lv_label_set_text_static(dbg_shake_speed, wie_du_willst);
}

void Dice::Roll() {
  state = currently_rolling;
  
  updateResult();
  motorController.RunForDuration(50);
  vTaskDelay(270);
  motorController.RunForDuration(40);
  vTaskDelay(200);
  motorController.RunForDuration(20);
  vTaskDelay(100);
  motorController.RunForDuration(20);
  vTaskDelay(90);
  motorController.RunForDuration(10);
  vTaskDelay(70);
  updateResult();
  motorController.RunForDuration(10);
  vTaskDelay(700);
  state = not_rolling;
}

static void btnEventHandler(lv_obj_t* obj, lv_event_t event) {
  auto* screen = static_cast<Dice*>(obj->user_data);
  if (event == LV_EVENT_PRESSED && screen->state == not_rolling) {
    	screen->state = currently_rolling;
  }
}

Dice::~Dice() {
  lv_task_del(refreshTask);
  //necessary to prevent a crash
  lv_label_set_text_static(title, "");
  lv_label_set_text_static(dbg_shake_speed, "");
  lv_obj_clean(lv_scr_act());
}
