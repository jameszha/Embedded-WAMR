# James Zhang
# June 2020

echo "Hexdumping WASM Modules"

for file in *.wasm
do
	OUT_FILE=${file%.*}.aot
	wamrc --target=thumbv7 --target-abi=eabi --cpu=cortex-m7 -o ${OUT_FILE} $file
	
	if [ -f ${OUT_FILE} ]; then
		echo "Module ${file%.*}: success"
	else
		echo "Module ${file%.*}: fail"
	fi
done

echo "Done!"
