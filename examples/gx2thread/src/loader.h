#ifndef LOADER_H
#define LOADER_H

#include "../../../libwiiu/src/coreinit.h"

typedef struct _GX2Surface {
    unsigned int dim;
    unsigned int width;
    unsigned int height;
    unsigned int depth;
    unsigned int numMips;
    unsigned int surfaceFormat;
    unsigned int aa;
    union {
    unsigned int surfaceUse;
    unsigned int resourceFlags;
    };
    unsigned int imageSize;
    union {
    void *imagePtr;
    void *pMem;
    };
    unsigned int mipSize;
    void *mipPtr;
    unsigned int tileMode;
    unsigned int swizzle;
    unsigned int alignment;
    unsigned int pitch;
    unsigned int mipOffset[ 13 ];

} GX2Surface;

typedef struct _GX2ColorBuffer {
    GX2Surface surface;

    unsigned int viewMip;
    unsigned int viewFirstSlice;
    unsigned int viewNumSlices;

    void *auxPtr;
    unsigned int  auxSize;

    unsigned int _regs[5];

} GX2ColorBuffer;


void _start();

#endif /* LOADER_H */