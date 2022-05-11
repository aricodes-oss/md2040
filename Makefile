OUT_FILE = build/md2040.elf
ENTRYPOINT = src

OCD = openocd
OCD_FLAGS = -f interface/picoprobe.cfg -f target/rp2040.cfg

all: flash

flash:
	mkdir -p build && cd build && cmake ..
	$(MAKE) -C build
	$(OCD) $(OCD_FLAGS) -c "program $(OUT_FILE) reset verify exit"

compile_commands.json: CMakeLists.txt
	mkdir -p build && cd build && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && cp compile_commands.json ..

clean:
	rm -rf build
