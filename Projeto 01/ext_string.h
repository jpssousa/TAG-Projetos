/*
	Any custom string manipulation comes here
	They are generally not included in string.h
*/
#ifndef EXT_STRING_H
	#define EXT_STRING_H

/*	@ams95: it's basically an inline inclusion
	Means that the hole code below going to be copied in any file that includes it
	BubbleReason: too many files for a small project!
*/
/* This code is public domain -- Will Hartung 4/9/09 */
size_t getline(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while(c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}

char *findCharAt(char *str, char c, int idx){
    while (idx && str){
        if (*str == c)
            idx--;
        str++;
    }

    return str ? str-- : NULL;
}
#endif