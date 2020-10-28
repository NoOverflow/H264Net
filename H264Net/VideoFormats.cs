using System;
using System.Collections.Generic;
using System.Text;

namespace H264Net
{
    public enum VideoFormats
    {
        VideoFormatRGB = 1,           
        VideoFormatRGBA = 2,
        VideoFormatRGB555 = 3,
        VideoFormatRGB565 = 4,
        VideoFormatBGR = 5,
        VideoFormatBGRA = 6,
        VideoFormatABGR = 7,
        VideoFormatARGB = 8,
        VideoFormatYUY2 = 20,           
        VideoFormatYVYU = 21,
        VideoFormatUYVY = 22,
        VideoFormatI420 = 23,           
        VideoFormatYV12 = 24,
        VideoFormatInternal = 25,        
        VideoFormatNV12 = 26,         
    }
}
