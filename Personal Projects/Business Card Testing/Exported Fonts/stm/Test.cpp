#include "config/stm32plus.h"
#include "config/display/font.h"

namespace stm32plus { namespace display {

  // byte definitions for FDEF_ROBOTO_THIN

  uint8_t FDEF_ROBOTO_THIN68_BYTES[]={ 0,224,137,162,40,138,30,0,};

  // character definitions for FDEF_ROBOTO_THIN

  extern const struct FontChar FDEF_ROBOTO_THIN_CHAR[]={
    { 68,6,FDEF_ROBOTO_THIN68_BYTES },
  };

} }
