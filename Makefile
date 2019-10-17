target = write read clr
target2 = _write _read
lib = syn.c

all: $(target) $(target2)

%: %.c
	gcc -o $@ $< $(lib)

clean: 
	rm $(target)
