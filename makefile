BUILD_FOLDER = build
VM_FILE_NAME = a1m
VM = $(BUILD_FOLDER)/$(VM_FILE_NAME)
CMP_FILE_NAME = a1c
CMP = $(BUILD_FOLDER)/$(CMP_FILE_NAME)


build:
	mkdir $(BUILD_FOLDER)

compiler: build
	gcc compiler/*.c include/*.c -o $(CMP) -I include/

machine: build
	gcc machine/*.c include/*.c -o $(VM) -I include/

cbaes: clean build compiler machine
	$(CMP) scripts/$(SCRIPT).a1
	$(VM) $(SCRIPT).ax

clean:
	rm -rf $(BUILD_FOLDER)
