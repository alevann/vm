BUILD_FOLDER = build

build:
	mkdir $(BUILD_FOLDER)

compiler: build
	gcc compiler/*.c -o $(BUILD_FOLDER)/a1c

vm: build
	gcc vm.c -o $(BUILD_FOLDER)/vm

clean:
	rm -rf $(BUILD_FOLDER)
