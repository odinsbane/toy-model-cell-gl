#ifndef _TIFF_WRITER
#define _TIFF_WRITER


#include "tiffio.h"

class TiffWriter{
    int height;
    int width;
    int count;
    TIFF* image;
    public:
        TiffWriter(const char* filename, int height, int width);
        void writeFrame(char* buffer);
        void close();
        int bpp;

    
};
#endif