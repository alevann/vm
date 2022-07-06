BUILD_FOLDER = build
VM_FILE_NAME = a1m
VM = $(BUILD_FOLDER)/$(VM_FILE_NAME)
CMP_FILE_NAME = a1c
CMP = $(BUILD_FOLDER)/$(CMP_FILE_NAME)


build:
	mkdir $(BUILD_FOLDER)

compiler: build
	gcc compiler/*.c include/a1/*.c -o $(CMP) -I include/ $(CFLAGS)

machine: build
	gcc machine/*.c include/a1/*.c -o $(VM) -I include/

cbcates: clean build compiler machine
	$(CMP) scripts/$(SCRIPT).a1
	$(VM) $(SCRIPT).ax

clean:
	rm -rf $(BUILD_FOLDER) *.ax
