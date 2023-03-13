#include <stdio.h>
#include <string.h>
#include <sys/resource.h>

#include "/home/james/c/nix.c/nix.h"
#include "/home/james/c/macros/global_macros.h" // noc

#define filename argv[1]

int main(int argc, char** argv)
{
	struct rlimit limit;
	size_t file_size = nix_sizeof_file(filename);
	if (!!file_size & (file_size + 1 < getrlimit(RLIMIT_STACK, &limit)))
		return -1;
	char buf[file_size + 1];
	nix_cat(buf, filename, file_size);
	char *bufp = buf;
	if (unlikely(!(bufp = strstr(bufp, "#define"))))
		return -1;
	bufp += strlen("#define ");
	printf("/\\\\b(");
	/* printf("/\\<("); */
	for (;;) {
		for (;;) {
			switch (*bufp) {
			default:
				putchar(*bufp++);
				continue;
			case '\0':
				goto END;
			case '(':
			case '\n':
				if (unlikely(!(bufp = strstr(bufp, "#define"))))
					goto END;
				bufp += strlen("#define ");
				printf("\\|");
				break;
			}
			break;
		}
	}
END:
	printf(")\\\\b/");
	/* printf("\\>\([^[:alnum:]]\\|$\\)/"); */
	return 0;
}
