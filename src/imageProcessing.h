#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct Pixel Pixel;

void processNegative(unsigned char* in, unsigned char* out, int width, int weight);
void processFilterOne(unsigned char * in, unsigned char * out, int width, int height);
void processFilterTwo(unsigned char * in, unsigned char * out, int width, int height);
void processFilterThree(unsigned char* in, unsigned char* out, int width, int height);
void processFilterFour(unsigned char* in, unsigned char* out, int width, int height);
void processFilterLGBT(unsigned char* in, unsigned char* out, int width, int height);
void processFilterIsrael(unsigned char* in, unsigned char* out, int width, int height);
void processFilterFlip(unsigned char* in, unsigned char* out, int width, int height);
void processFilterPixelation(unsigned char* in, unsigned char* out, int width, int height);
void processFilterWriteWithBlue(unsigned char* in, unsigned char* out, int width, int height);
void processFilterTriangleFlip(unsigned char* in, unsigned char* out, int width, int height);
static int clamp(int value, int lower, int higher);
static void addDarkFrame(Pixel** pixels, int width, int height);
void processFilterDarkFrame(unsigned char* in, unsigned char* out, int width, int height);
static void addWhiteBottomGlow(Pixel** pixels, int width, int height);
void processFilterWhiteBottomGlow(unsigned char* in, unsigned char* out, int width, int height);
static void stickmanWorldFill(Pixel** pixels, int width, int height);
void processFilterStickmanWorld(unsigned char* in, unsigned char* out, int width, int height);