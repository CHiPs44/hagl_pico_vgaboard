#!/bin/bash

FILES="8.F08 8_LT1.F08 ALT8.F08 ALT8_LT1.F08 FANT.F08 FANT_LT1.F08 MCR.F08 MCR_LT1.F08 THIN.F08 THIN_LT1.F08 16.F16 16_LT1.F16 TALL.F16 TALL_LT1.F16"

declare -A MAPS
MAPS["8.F08"]="unscii-437-8x8"
MAPS["8_LT1.F08"]="unscii-850-8x8"
MAPS["ALT8.F08"]="unscii-alt-437-8x8"
MAPS["ALT8_LT1.F08"]="unscii-alt-850-8x8"
MAPS["FANT.F08"]="unscii-fantasy-437-8x8"
MAPS["FANT_LT1.F08"]="unscii-fantasy-850-8x8"
MAPS["MCR.F08"]="unscii-mcr-437-8x8"
MAPS["MCR_LT1.F08"]="unscii-mcr-850-8x8"
MAPS["THIN.F08"]="unscii-thin-437-8x8"
MAPS["THIN_LT1.F08"]="unscii-thin-850-8x8"
MAPS["16.F16"]="unscii-16-437-8x16"
MAPS["16_LT1.F16"]="unscii-16-850-8x16"
MAPS["TALL.F16"]="unscii-tall-437-8x16"
MAPS["TALL_LT1.F16"]="unscii-tall-850-8x16"

for FILE in $FILES; do
	NAME=${MAPS[$FILE]}
	echo "Converting ${FILE} => ${NAME}.h"
	cp -pv ${FILE} ${NAME}
	xxd -i -c 8 ${NAME} > ${NAME}.h
	rm $NAME
done

# EOF

