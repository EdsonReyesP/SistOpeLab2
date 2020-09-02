#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1


int main(int argc, char *argv[]) {
	write(STDOUT_FILENO, "TE ENVIO ESTE MENSAJE CUALQUIERA", 33);
	return 0;
}