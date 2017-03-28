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

    #ifndef _WIN32 /*@asm95: _WIN32 flag enables windows.h inclusion in localization.h*/
    char *strcpy (char *orig, char *dest)
    {
        int i=0;
        while( (dest[i] = orig[i]) )
            i++;
        return dest;
    }
    #endif

char *findNextChar(char *str, char c){
    while (*str != '\0'){
        if (*str == c)
            break;
        str++;
    }

    return str;
}

#endif
