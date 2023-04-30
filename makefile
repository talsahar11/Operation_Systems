C_FILES := cmp.c copy.c encode.c decode.c
C_EXECS := $(patsubst %.c,%,$(C_FILES))
OBJECTS := $(patsubst %.c,%.o,$(C_FILES))


P1_C_FILES := cmp.c copy.c
P1_OBJECTS := $(patsubst %.c,%.o,$(P1_C_FILES))
P1_EXECS := $(patsubst %.c,%,$(P1_C_FILES))


P2_C_FILES := encode.c decode.c
P2_OBJECTS := $(patsubst %.c,%.o,$(P2_C_FILES))
P2_EXECS := $(patsubst %.c,%,$(P2_C_FILES))
LIBS_C_FILES := codecA.c codecB.c
SO_LIBS := $(patsubst %.c,lib%.so,$(LIBS_C_FILES))
P3_EXECS := stshell
default: PART1 PART2 PART3

PART1: $(P1_EXECS)

$(P1_EXECS): %: %.o $(P1_OBJECTS)
	gcc $< -o $@

$(P1_OBJECTS): %.o: %.c $(P1_C_FILES)
	gcc -c $< -o $@

PART2: $(P2_EXECS) $(SO_LIBS)

$(P2_EXECS): %: %.o
	gcc $< -o $@ -ldl

$(P2_OBJECTS): %.o: %.c
	gcc -c $< -o $@

$(SO_LIBS): lib%.so: %.c
	gcc -shared -fPIC $< -o $@


PART3: stshell.c
	gcc -o stshell stshell.c

clean:
	rm -f $(P1_EXECS) $(P1_OBJECTS) $(P2_EXECS) $(P2_OBJECTS) $(SO_LIBS) $(P3_EXECS)