#pragma once

#include "display/graphic/Font.h"

namespace stm32plus { namespace display {

  // helper so the user can just do 'new fontname' without having to know the parameters

  extern const struct FontChar FDEF_ROBOTO_THIN_CHAR[];

  class Font_ROBOTO_THIN10 : public Font {
    public:
      Font_ROBOTO_THIN10()
        : Font(68,1,10,0,FDEF_ROBOTO_THIN_CHAR) {
      }
  };
} }
