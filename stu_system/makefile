FUNCTION = $(wildcard fun/*.c)
MAIN_PATH = $(wildcard src/*.c)
FUNCTION_OBJ = $(patsubst %.c,%.o,$(FUNCTION))
INCLUDE = -I inc
LIB_PATH = libfile/libfun.a
LIBRARY = -L libfile/ -lfun -lfont
EXE_PATH = bin/main
%.o:%.c
	arm-linux-gcc $^ -c -o $@ $(INCLUDE)

$(LIB_PATH):$(FUNCTION_OBJ)
	ar rcs $@ $^
	arm-linux-gcc ${MAIN_PATH} -o ${EXE_PATH} ${INCLUDE} ${LIBRARY}

clean:
	rm ${EXE_PATH}
	rm ${LIB_PATH}
	rm ${FUNCTION_OBJ}
