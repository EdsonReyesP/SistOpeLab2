salida_out = pipeline
open_out = open
gray_process_out = gray_process
filter_out = filter
binary_out = binary
classification_out = classification
process_finish_out = process_finish

salida_headers = funciones.h
salida_source  = $(salida_headers:.h=.c) main.c
salida_objects = $(salida_source:.c=.o)

open_headers = funciones.h
open_source  = $(open_headers:.h=.c) open.c
open_objects = $(open_source:.c=.o)

gray_process_headers = funciones.h
gray_process_source  = $(gray_process_headers:.h=.c) gray_process.c
gray_process_objects = $(gray_process_source:.c=.o)

filter_headers = funciones.h
filter_source  = $(filter_headers:.h=.c) filter.c
filter_objects = $(filter_source:.c=.o)

binary_headers = funciones.h
binary_source  = $(binary_headers:.h=.c) binary.c
binary_objects = $(binary_source:.c=.o)

classification_headers = funciones.h
classification_source  = $(classification_headers:.h=.c) classification.c
classification_objects = $(classification_source:.c=.o)

process_finish_headers = funciones.h
process_finish_source  = $(process_finish_headers:.h=.c) process_finish.c
process_finish_objects = $(process_finish_source:.c=.o)

CC     = gcc
CFLAGS = -Wall

depends = .depends

build: $(salida_out)
build: $(open_out)
build: $(gray_process_out)
build: $(filter_out)
build: $(binary_out)
build: $(classification_out)
build: $(process_finish_out)

$(salida_out) : $(salida_objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(open_out) : $(open_objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(gray_process_out) : $(gray_process_objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(filter_out) : $(filter_objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(binary_out) : $(binary_objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(classification_out) : $(classification_objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(process_finish_out) : $(process_finish_objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm

clean :
	$(RM) $(salida_out) $(salida_objects) $(zipfile) $(depends)

.PHONY : build zip clean

sinclude $(depends)
