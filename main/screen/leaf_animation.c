#include "leaf_animation.h"


static const uint8_t leaf_frames[][128] = {
    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 124, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 128, 0, 8, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 124, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 128, 0, 8, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 124, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 128, 0, 8, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 124, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 128, 0, 8, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 60, 32, 0, 3, 224, 48, 0, 15, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 192, 0, 8, 1, 128, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 16, 8, 6, 0, 32, 8, 2, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 32, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 60, 48, 0, 1, 224, 16, 0, 7, 0, 16, 0, 24, 0, 24, 0, 96, 0, 24, 0, 192, 0, 8, 1, 128, 1, 8, 3, 0, 3, 8, 2, 0, 2, 8, 6, 0, 6, 12, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 24, 8, 4, 0, 16, 8, 6, 0, 48, 8, 2, 0, 96, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 193, 0, 16, 0, 118, 0, 48, 0, 60, 0, 32, 0, 28, 0, 96, 0, 54, 0, 192, 0, 195, 1, 128, 63, 1, 255, 0, 24, 0, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 96, 0, 0, 0, 224, 0, 0, 3, 160, 0, 0, 30, 48, 0, 0, 248, 16, 0, 3, 128, 16, 0, 28, 0, 24, 0, 48, 0, 8, 0, 96, 0, 8, 0, 128, 1, 8, 1, 0, 1, 8, 3, 0, 3, 12, 2, 0, 2, 12, 6, 0, 6, 12, 6, 0, 4, 12, 4, 0, 8, 12, 4, 0, 24, 12, 6, 0, 48, 12, 2, 0, 32, 8, 3, 0, 64, 8, 1, 128, 128, 24, 0, 193, 128, 16, 0, 115, 0, 16, 0, 62, 0, 32, 0, 28, 0, 96, 0, 54, 0, 64, 0, 227, 1, 128, 63, 129, 199, 0, 24, 0, 124, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 96, 0, 0, 1, 224, 0, 0, 15, 48, 0, 0, 124, 16, 0, 3, 192, 16, 0, 14, 0, 24, 0, 48, 0, 8, 0, 96, 0, 8, 0, 192, 0, 8, 1, 128, 1, 8, 1, 0, 1, 12, 3, 0, 3, 4, 2, 0, 2, 4, 2, 0, 6, 4, 6, 0, 12, 4, 6, 0, 8, 12, 6, 0, 24, 12, 2, 0, 48, 12, 3, 0, 96, 8, 1, 128, 192, 8, 0, 193, 128, 24, 0, 99, 0, 16, 0, 62, 0, 48, 0, 28, 0, 32, 0, 54, 0, 64, 0, 227, 0, 192, 63, 129, 131, 0, 16, 0, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 96, 0, 0, 0, 240, 0, 0, 3, 144, 0, 0, 30, 16, 0, 1, 240, 24, 0, 7, 0, 8, 0, 28, 0, 8, 0, 48, 0, 8, 0, 96, 0, 8, 0, 128, 1, 140, 1, 0, 1, 4, 3, 0, 3, 4, 2, 0, 2, 4, 2, 0, 6, 4, 6, 0, 4, 4, 6, 0, 12, 4, 2, 0, 24, 4, 2, 0, 48, 12, 1, 0, 96, 8, 1, 128, 192, 8, 0, 193, 128, 8, 0, 99, 0, 24, 0, 62, 0, 16, 0, 28, 0, 48, 0, 54, 0, 96, 0, 227, 0, 192, 63, 131, 129, 128, 16, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 112, 0, 0, 1, 144, 0, 0, 14, 16, 0, 0, 240, 24, 0, 3, 128, 8, 0, 12, 0, 8, 0, 48, 0, 8, 0, 96, 0, 12, 0, 192, 0, 140, 1, 128, 1, 132, 1, 0, 1, 4, 3, 0, 3, 4, 2, 0, 2, 4, 2, 0, 6, 4, 2, 0, 12, 4, 2, 0, 24, 4, 2, 0, 16, 4, 1, 0, 48, 12, 1, 128, 96, 8, 0, 192, 128, 8, 0, 99, 0, 24, 0, 62, 0, 16, 0, 28, 0, 48, 0, 54, 0, 96, 1, 226, 0, 192, 63, 3, 129, 192, 16, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 112, 0, 0, 1, 144, 0, 0, 15, 16, 0, 0, 248, 24, 0, 7, 128, 8, 0, 28, 0, 8, 0, 48, 0, 12, 0, 96, 0, 12, 0, 192, 0, 132, 1, 128, 1, 132, 1, 0, 1, 4, 3, 0, 3, 4, 2, 0, 2, 4, 2, 0, 6, 4, 2, 0, 12, 4, 2, 0, 8, 4, 2, 0, 16, 4, 1, 0, 48, 12, 1, 128, 96, 8, 0, 192, 192, 8, 0, 97, 0, 24, 0, 62, 0, 16, 0, 12, 0, 48, 0, 54, 0, 32, 0, 226, 0, 64, 63, 3, 129, 192, 16, 1, 255, 0, 0, 0, 60, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 96, 0, 0, 0, 240, 0, 0, 3, 144, 0, 0, 30, 16, 0, 0, 240, 24, 0, 3, 128, 8, 0, 12, 0, 8, 0, 48, 0, 8, 0, 96, 0, 12, 0, 192, 0, 140, 1, 128, 1, 132, 1, 0, 1, 4, 3, 0, 3, 4, 2, 0, 2, 4, 2, 0, 6, 4, 2, 0, 12, 4, 2, 0, 24, 4, 2, 0, 16, 4, 1, 0, 48, 12, 1, 128, 96, 8, 0, 192, 192, 8, 0, 99, 0, 24, 0, 62, 0, 16, 0, 28, 0, 48, 0, 54, 0, 96, 1, 226, 0, 192, 63, 3, 129, 192, 16, 0, 255, 0, 0, 0, 8, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 96, 0, 0, 0, 240, 0, 0, 3, 48, 0, 0, 60, 16, 0, 1, 224, 16, 0, 7, 0, 24, 0, 28, 0, 8, 0, 48, 0, 8, 0, 96, 0, 8, 0, 128, 1, 140, 1, 0, 1, 4, 3, 0, 1, 4, 2, 0, 2, 4, 2, 0, 6, 4, 2, 0, 4, 4, 6, 0, 12, 4, 2, 0, 24, 4, 2, 0, 48, 12, 1, 0, 96, 8, 1, 128, 192, 8, 0, 193, 128, 8, 0, 99, 0, 24, 0, 62, 0, 16, 0, 28, 0, 48, 0, 54, 0, 96, 0, 227, 0, 192, 63, 131, 129, 128, 16, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 96, 0, 0, 0, 224, 0, 0, 1, 160, 0, 0, 6, 48, 0, 0, 120, 16, 0, 3, 192, 16, 0, 14, 0, 24, 0, 56, 0, 8, 0, 96, 0, 8, 0, 192, 0, 8, 1, 128, 1, 8, 1, 0, 1, 12, 3, 0, 3, 4, 2, 0, 2, 4, 2, 0, 6, 4, 6, 0, 12, 4, 6, 0, 8, 12, 2, 0, 24, 12, 2, 0, 48, 12, 3, 0, 96, 8, 1, 128, 192, 8, 0, 193, 128, 24, 0, 99, 0, 16, 0, 62, 0, 48, 0, 28, 0, 32, 0, 118, 0, 96, 1, 195, 0, 192, 63, 1, 131, 128, 0, 0, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 224, 0, 0, 3, 160, 0, 0, 14, 32, 0, 0, 240, 48, 0, 3, 128, 16, 0, 12, 0, 16, 0, 48, 0, 24, 0, 96, 0, 8, 0, 192, 1, 8, 1, 128, 1, 8, 3, 0, 3, 8, 2, 0, 2, 12, 2, 0, 6, 12, 6, 0, 4, 12, 4, 0, 8, 12, 4, 0, 24, 12, 6, 0, 16, 12, 2, 0, 32, 8, 3, 0, 96, 8, 1, 128, 192, 24, 0, 193, 128, 24, 0, 115, 0, 16, 0, 62, 0, 48, 0, 28, 0, 32, 0, 54, 0, 64, 0, 227, 0, 128, 63, 129, 227, 0, 0, 0, 124, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 192, 0, 0, 1, 192, 0, 0, 7, 32, 0, 0, 60, 32, 0, 1, 224, 48, 0, 7, 128, 16, 0, 28, 0, 16, 0, 48, 0, 16, 0, 192, 0, 24, 1, 128, 1, 8, 1, 0, 3, 8, 2, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 16, 8, 6, 0, 48, 8, 2, 0, 96, 8, 3, 0, 192, 24, 1, 129, 128, 24, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 32, 0, 28, 0, 96, 0, 118, 0, 192, 1, 195, 1, 128, 63, 1, 255, 0, 0, 0, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 128, 0, 0, 0, 192, 0, 0, 3, 64, 0, 0, 14, 96, 0, 0, 120, 32, 0, 3, 192, 32, 0, 15, 0, 48, 0, 56, 0, 16, 0, 96, 0, 16, 0, 192, 1, 24, 1, 128, 3, 24, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 96, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 32, 0, 28, 0, 96, 0, 102, 0, 192, 1, 195, 3, 128, 63, 1, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 128, 0, 0, 1, 192, 0, 0, 3, 64, 0, 0, 28, 64, 0, 0, 112, 32, 0, 3, 192, 32, 0, 14, 0, 48, 0, 56, 0, 16, 0, 96, 0, 16, 0, 192, 2, 16, 1, 0, 2, 24, 3, 0, 2, 8, 2, 0, 6, 8, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 16, 8, 4, 0, 48, 8, 4, 0, 96, 8, 6, 0, 64, 24, 3, 0, 192, 16, 1, 129, 128, 16, 0, 195, 0, 48, 0, 126, 0, 32, 0, 28, 0, 96, 0, 28, 0, 64, 0, 98, 0, 128, 1, 195, 131, 0, 63, 0, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 128, 0, 0, 1, 192, 0, 0, 7, 64, 0, 0, 28, 96, 0, 0, 240, 32, 0, 7, 128, 32, 0, 28, 0, 48, 0, 48, 0, 16, 0, 192, 0, 16, 1, 128, 2, 16, 1, 0, 2, 24, 2, 0, 6, 24, 6, 0, 4, 8, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 24, 8, 4, 0, 16, 8, 4, 0, 48, 8, 4, 0, 96, 24, 6, 0, 64, 24, 3, 0, 192, 16, 1, 129, 128, 16, 0, 195, 0, 48, 0, 126, 0, 32, 0, 28, 0, 96, 0, 28, 0, 192, 0, 98, 0, 128, 1, 195, 131, 0, 63, 0, 252, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 128, 0, 0, 1, 192, 0, 0, 7, 64, 0, 0, 28, 96, 0, 0, 240, 32, 0, 7, 128, 32, 0, 28, 0, 48, 0, 48, 0, 16, 0, 224, 0, 16, 1, 128, 2, 16, 1, 0, 2, 24, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 24, 8, 4, 0, 16, 8, 4, 0, 48, 8, 4, 0, 96, 24, 6, 0, 64, 24, 3, 0, 192, 16, 1, 129, 128, 16, 0, 195, 0, 48, 0, 126, 0, 32, 0, 28, 0, 96, 0, 28, 0, 192, 0, 114, 0, 128, 1, 195, 131, 0, 63, 0, 252, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 192, 0, 0, 3, 64, 0, 0, 30, 96, 0, 0, 248, 32, 0, 7, 128, 32, 0, 28, 0, 48, 0, 48, 0, 16, 0, 96, 0, 16, 1, 128, 2, 16, 1, 0, 2, 24, 2, 0, 2, 8, 6, 0, 4, 8, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 24, 8, 4, 0, 16, 8, 4, 0, 48, 8, 4, 0, 96, 8, 6, 0, 64, 24, 3, 0, 128, 16, 1, 129, 128, 16, 0, 195, 0, 48, 0, 126, 0, 32, 0, 28, 0, 96, 0, 28, 0, 192, 0, 98, 1, 128, 1, 195, 131, 0, 63, 0, 252, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 192, 0, 0, 3, 192, 0, 0, 30, 96, 0, 0, 240, 32, 0, 7, 128, 32, 0, 12, 0, 48, 0, 48, 0, 16, 0, 96, 0, 16, 0, 128, 0, 24, 1, 0, 2, 24, 3, 0, 2, 8, 6, 0, 6, 8, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 24, 8, 4, 0, 16, 8, 4, 0, 48, 8, 4, 0, 96, 8, 6, 0, 64, 24, 3, 0, 128, 16, 1, 129, 128, 16, 0, 195, 0, 48, 0, 126, 0, 32, 0, 28, 0, 96, 0, 28, 0, 192, 0, 98, 1, 128, 0, 195, 135, 0, 63, 0, 252, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 192, 0, 0, 3, 224, 0, 0, 14, 32, 0, 0, 248, 32, 0, 3, 192, 48, 0, 14, 0, 16, 0, 48, 0, 16, 0, 96, 0, 16, 0, 128, 0, 24, 1, 0, 3, 8, 3, 0, 2, 8, 6, 0, 6, 8, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 96, 8, 6, 0, 64, 24, 3, 0, 192, 16, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 32, 0, 28, 0, 96, 0, 28, 0, 64, 0, 114, 0, 128, 0, 195, 3, 0, 63, 128, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 192, 0, 0, 1, 224, 0, 0, 14, 32, 0, 0, 120, 32, 0, 3, 192, 48, 0, 14, 0, 16, 0, 56, 0, 16, 0, 224, 0, 16, 0, 128, 0, 24, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 4, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 96, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 32, 0, 28, 0, 96, 0, 28, 0, 64, 0, 118, 0, 128, 0, 195, 3, 0, 63, 129, 254, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 6, 32, 0, 0, 120, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 224, 0, 24, 0, 128, 0, 24, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 32, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 128, 0, 195, 3, 0, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 124, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 128, 0, 8, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 124, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 128, 0, 8, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 1, 224, 0, 0, 7, 32, 0, 0, 124, 32, 0, 3, 192, 48, 0, 30, 0, 16, 0, 56, 0, 16, 0, 96, 0, 24, 0, 128, 0, 8, 1, 0, 3, 8, 3, 0, 2, 8, 2, 0, 6, 8, 6, 0, 4, 8, 4, 0, 12, 8, 4, 0, 8, 8, 4, 0, 24, 8, 4, 0, 48, 8, 4, 0, 32, 8, 6, 0, 64, 8, 3, 0, 192, 24, 1, 129, 128, 16, 0, 195, 0, 16, 0, 118, 0, 48, 0, 28, 0, 96, 0, 28, 0, 64, 0, 54, 0, 192, 0, 195, 3, 128, 63, 129, 254, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

void leaf_animation_play(ssd1306_handle_t handle)
{
    int frame = 0;
    while (1)
    {
        for (int y = 0; y < 48; y++)
        {
            ssd1306_draw_line(handle, 0, y, 32, y, 0);
        }
        ssd1306_draw_bitmap(handle, 5, 16, leaf_frames[frame], 32, 32);
        ssd1306_refresh_gram(handle);
        frame = (frame + 1) % (sizeof(leaf_frames) / sizeof(leaf_frames[0]));
        vTaskDelay(pdMS_TO_TICKS(42));
    }
}