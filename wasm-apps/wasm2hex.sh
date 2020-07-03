# James Zhang
# June 2020

echo "Hexdumping WASM Modules"

for file in *.wasm
do
	OUT_FILE=${file%.*}.hex
	hexdump -v -e '8/1 "0x%02x, "' -e '"\n"' $file > ${OUT_FILE}
	
	if [ -f ${OUT_FILE} ]; then
		echo "Module ${file%.*}: success"
	else
		echo "Module ${file%.*}: fail"
	fi
done

echo "Done!"
