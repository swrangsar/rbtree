#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef __DEBUG__
    #define debug(m, ...) printf(m, ##__VA_ARGS__);
#else
    #define debug(m, ...) 
#endif

#define memcheck(a) if (!(a)) { printf("DEBUG %s:%d: memory allocation failed!\n", __FILE__, __LINE__); exit(EXIT_FAILURE);}
#define errcheck(a, m, ...) if (!(a)) { printf("DEBUG %s:%d: " m "\n", __FILE__, __LINE__, ##__VA_ARGS__); exit(EXIT_FAILURE);}

#endif
