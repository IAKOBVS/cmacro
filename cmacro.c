#include <stdio.h>
#include <string.h>

#include "/home/james/c/nix.c/nix.h"
#include "/home/james/c/macros/global_macros.h" // noc

#define filename argv[1]

int main(int argc, char** argv)
{
	size_t file_size = nix_sizeof_file(filename);
	char buf[file_size + 1];
	nix_cat(buf, filename, file_size);
	char *bufPtr = buf;
	for (;;) {
		if (likely(bufPtr = strstr(bufPtr, "#define"))) {
			bufPtr += strlen("#define ");
			for (;;) {
				switch (*bufPtr) {
				default:
					putchar(*bufPtr++);
					continue;
				case '\0':
					goto EARLY_RET;
				case '(':
				case ' ':
				case '\t':
				case '\r':
					if (unlikely(!(bufPtr = strchr(bufPtr, '\n'))))
						goto EARLY_RET;
					++bufPtr;
				case '\n':
					break;
				}
				break;
			}
			putchar(' ');
		} else {
			break;
		}
	}
EARLY_RET:
	return 0;
}
