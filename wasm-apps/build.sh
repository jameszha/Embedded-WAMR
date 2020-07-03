# James Zhang
# June 2020

echo "Building WASM Modules:"

WAMR_DIR=${PWD}/../wamr

for file in *.c
do
	APP_SRC="$file"
	OUT_FILE=${file%.*}.wasm

	/opt/wasi-sdk/bin/clang     \
		--target=wasm32 -O3 \
		-z stack-size=4096 -Wl,--initial-memory=65536 \
		--sysroot=${WAMR_DIR}/wamr-sdk/app/libc-builtin-sysroot    \
		-Wl,--allow-undefined-file=${WAMR_DIR}/wamr-sdk/app/libc-builtin-sysroot/share/defined-symbols.txt \
		-Wl,--export=main, \
		-Wl,--no-threads,--strip-all,--no-entry \
		-nostdlib -o ${OUT_FILE} $file

	if [ -f ${OUT_FILE} ]; then
		echo "Module ${file%.*}: success"
	else
		echo "Module ${file%.*}: fail"
	fi
done

echo "Done!"


