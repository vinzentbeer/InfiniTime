#pragma once

#include "displayapp/screens/Screen.h"
#include "displayapp/widgets/Counter.h"

#include <components/settings/Settings.h>
#include <components/motor/MotorController.h>
#include <components/motion/MotionController.h>
#include <lvgl/lvgl.h>

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      enum State { not_rolling, rolling_soon, currently_rolling, prevent_rolling };

      class Dice : public Screen {
      public:
        Dice(Controllers::MotionController& motionController,
             Controllers::Settings& settingsController,
             Controllers::MotorController& motorController);
        ~Dice() override;
        void Roll();
        void Refresh() override;
        State state = not_rolling;

      private:
        Controllers::MotionController& motionController;
        Controllers::Settings& settingsController;
        Controllers::MotorController& motorController;
        char result[4] = "-";
        char wie_du_willst[20] = "-";
        Widgets::Counter sideCounter = Widgets::Counter(2, 20, jetbrains_mono_76);
        lv_obj_t* btnRoll;
        lv_obj_t* title;
        lv_obj_t* dbg_shake_speed;
        lv_task_t* refreshTask;
        void updateResult();
        bool isRolling = false;
      };

    }
  }
}