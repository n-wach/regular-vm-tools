//
// Created by me on 1/25/20.
//

#ifndef REGULARVM_UTIL_H
#define REGULARVM_UTIL_H

//#define DEBUG
#ifdef DEBUG
#define debug(str) printf("%s ", str)
#define debugln(str) printf("%s\n", str)
#endif
#ifndef DEBUG
#define debug(str)
    #define debugln(str)
#endif

#define sign(v) v > 0 ? 1 : v < 0 ? -1 : 0

#endif //REGULARVM_UTIL_H
