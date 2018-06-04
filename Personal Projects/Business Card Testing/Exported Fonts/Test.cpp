#include <avr/pgmspace.h>
#include "Font.h"

namespace lcd {

  // byte definitions for FDEF_ROBOTO_THIN

  const uint8_t __attribute__((progmem)) FDEF_ROBOTO_THIN68_BYTES[] PROGMEM={ 0,224,137,162,40,138,30,0,};

  // character definitions for FDEF_ROBOTO_THIN

  extern const struct FontChar __attribute__((progmem)) FDEF_ROBOTO_THIN_CHAR[] PROGMEM={
    { 68,6,FDEF_ROBOTO_THIN68_BYTES },
  };

}
