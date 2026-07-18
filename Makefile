all:
	mkdir -p build/bin
	gcc src/main.c -o build/bin/main
clean:
	rm -rf build
