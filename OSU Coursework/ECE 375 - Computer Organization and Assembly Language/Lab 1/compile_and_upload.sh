#!/bin/bash

# alternate avra BasicBumpBot.asm
/opt/gavrasm/gavrasm -X BasicBumpBot.asm
avrdude -c usbasp -p m128 -U flash:w:BasicBumpBot.hex
rm BasicBumpBot.lst
