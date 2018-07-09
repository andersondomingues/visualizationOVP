#!/bin/sh

# Check Installation supports this example
checkinstall.exe -p install.pkg --nobanner || exit


CROSS=OR1K

make -C application CROSS=${CROSS}
make -C module      NOVLNV=1
make -C harness     NOVLNV=1

ANSWER=
echo "Do you want to open a browser to see the visualization [y/N]"
read ANSWER
if [ "${ANSWER}" = "y" ]; then
  if [ -x /usr/bin/firefox ]; then 
	(sleep 1; firefox http://localhost:8000) &
  elif [ "$(uname)" = "MINGW32_NT-6.1" ]; then 
    (sleep 1; explorer http://localhost:8000) &
  else
    echo "Use 'firefox http://localhost:8000' to see visualization"
  fi
fi

harness/harness.${IMPERAS_ARCH}.exe \
    --program top/u1/cpu1=application/application1.${CROSS}.elf \
    --program top/u1/cpu2=application/application2.${CROSS}.elf \
	--verbose --output imperas.log \
	--stoponcontrolc  \
	--Q 0.0001 \
    \
    --httpvis \
    $*
