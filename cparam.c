#include <stdio.h>
#include <string.h>
#include <sys/resource.h>

#include "/home/james/c/macros/global_macros.h" // noc
#include "/home/james/c/nix.c/nix.h"

#define filename argv[1]

int has_param(const char *bufp)
{
	for (;;) {
		switch (*bufp) {
		default:
			++bufp;
		case '(':
			return 1;
		case '\0':
		case '\n':
			return 0;
		}
	}
}

int has_more_param(const char *bufp)
{
	for (;;) {
		switch (*bufp) {
		default:
			++bufp;
		case ',':
			return 1;
		case '.':
		case '\0':
		case '\n':
			return 0;
		}
	}
}

int main(int argc, char** argv)
{
	struct rlimit limit;
	size_t file_size = nix_sizeof_file(filename);
	if (!!file_size & (file_size + 1 < getrlimit(RLIMIT_STACK, &limit)))
		return -1;
	char buf[file_size + 1];
	nix_cat(buf, filename, file_size);
	char *bufp = buf;
	for (;;) {
		if (unlikely(!(bufp = strstr(bufp, "#define"))))
			return -1;
		if (has_param(bufp)) {
			bufp = strchr(bufp, '(');
			++bufp;
			break;
		}
	}
	/* printf("/\\\\b("); */
	printf("/\\<(");
	for (;;) {
		for ( ;; ++bufp) {
			switch (*bufp) {
			default:
				putchar(*bufp);
				continue;
			case '.':
				printf("\\.");
				continue;
			case ',':
				if (has_more_param(bufp))
					printf("\\|");
				continue;
			case ' ':
				continue;
			case '\0':
				goto END;
			case ')':
			case '\n':
				break;
			}
			break;
		}
		for (;;) {
			if (unlikely(!(bufp = strstr(bufp, "#define"))))
				return -1;
			if (has_param(bufp)) {
				if (unlikely(!(bufp = strchr(bufp, '('))))
					goto END;
				char *tmp = bufp;
				if (unlikely(!(tmp = strstr(bufp, "#define"))))
					goto END;
				if (has_more_param(tmp))
					printf("\\|");
				++bufp;
				break;
			}
		}
	}
END:
	/* printf(")\\\\b/"); */
	printf("\\>\([^[:alnum:]]\\|$\\)/");
	return 0;
}
