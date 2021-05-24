#/bin.sh
cp -p asm/arm-eabi-32/*.s .
for i in *.s; do
  rm ${i%%.*}.c
done
