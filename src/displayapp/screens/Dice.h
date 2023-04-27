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
      class Dice : public Screen {
      public:
        Dice(Controllers::MotionController& motionController, Controllers::Settings& settingsController, Controllers::MotorController& motorController);
        ~Dice() override;
		    void Roll();
      private:
        Controllers::MotionController& motionController;
        Controllers::Settings& settingsController;
        Controllers::MotorController& motorController;
        char result[4] = "-";
        Widgets::Counter sideCounter = Widgets::Counter(2, 20, jetbrains_mono_76);
        lv_obj_t* btnRoll;
        lv_obj_t* title;
        void updateResult();
      };
	  
    }
  }
}