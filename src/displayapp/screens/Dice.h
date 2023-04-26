#pragma once

#include "displayapp/screens/Screen.h"
#include "displayapp/widgets/Counter.h"
#include <lvgl/lvgl.h>

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class Dice : public Screen {
      public:
        Dice();
        ~Dice() override;
		void Roll();
	  private:
	  	char result[8];
	    Widgets::Counter sideCounter = Widgets::Counter(2, 20, jetbrains_mono_76);
		lv_obj_t* btnRoll;
		lv_obj_t* title;
      };
	  
    }
  }
}