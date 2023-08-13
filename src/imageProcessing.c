#define _CRT_SECURE_NO_WARNINGS

#include "imageProcessing.h"

#define MAX_VALUE 255
#define MIN_VALUE 0
#define RGB_COLORS_AMOUNT 3 /* red, green, blue */
#define GRAYSCALE_RED_MULT 0.299
#define GRAYSCALE_BLUE_MULT 0.578
#define GRAYSCALE_GREEN_MULT 0.114
#define RGB_GREEN_INC 1
#define RGB_BLUE_INC 2
#define GET_HALF 2
#define PIXELATION_BLOCK_SIZE 4 /* best values: 4 (less pixelated), 8 (more pixelated). */
#define DEC 1
#define INC 1
#define MAX_DARK_FRAME_TONE 30
#define DARK_FRAME_TONE_MUL 8
#define GET_THIRD_PART 3
#define BOTTOM_GLOW_DEC 7
#define FOUND 1
#define NOT_FOUND 0
#define CONTRAST_DARK 85
#define CONTRAST_LIGHT MAX_VALUE-CONTRAST_DARK

#define max(x,y) (x>y?x:y)
#define min(x,y) (x<y?x:y)
#define abs(x) (x<MIN_VALUE?-x:x)

typedef struct Pixel
{
	// pixel that contains RGB data.
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} Pixel;

bool* bluePixels = NULL;
bool startBluePixelsSave = true;

void processNegative(unsigned char* in, unsigned char* out, int width, int height)
{
	int i = 0; 
	int totalPixels = width * height * RGB_COLORS_AMOUNT;

	for (i = 0; i < totalPixels; i+=RGB_COLORS_AMOUNT)
	{
		out[i] = in[i];
		out[i + RGB_GREEN_INC] = MAX_VALUE - in[i];
		out[i + RGB_BLUE_INC] = MAX_VALUE - in[i];
	}
}

void processFilterOne(unsigned char * in, unsigned char * out, int width, int height)
{
	
	int i = 0;
	int totalPixels = width * height * RGB_COLORS_AMOUNT;
	int grayscale = 0;

	for (i = 0; i < totalPixels; i+=RGB_COLORS_AMOUNT)
	{
		grayscale = (double)(in[i]) * GRAYSCALE_RED_MULT  + (double)(in[i + RGB_GREEN_INC]) * GRAYSCALE_BLUE_MULT + (double)(in[i + RGB_BLUE_INC]) * GRAYSCALE_GREEN_MULT;
		out[i] = grayscale;
		out[i + RGB_GREEN_INC] = grayscale;
		out[i + RGB_BLUE_INC] = grayscale;
	}
}

void processFilterTwo(unsigned char * in, unsigned char * out, int width, int height) {

	int i = 0; 
	int totalPixels = width * height * RGB_COLORS_AMOUNT;
	int halfTotalPixels = totalPixels / 2;

	for (i = 0; i < halfTotalPixels; i++)
	{
		out[i] = in[i + halfTotalPixels];
	}
	for (i = 0; i < halfTotalPixels; i++)
	{
		out[i + halfTotalPixels] = in[i + halfTotalPixels];
	}
}

void processFilterThree(unsigned char* in, unsigned char* out, int width, int height)
{
	int i = 0; 
	int totalPixels = width * height * RGB_COLORS_AMOUNT;
	
	for (i = 0; i < totalPixels; i+=RGB_COLORS_AMOUNT)
	{
		if (in[i] > 80 && in[i + RGB_GREEN_INC] < 80 && in[i + RGB_BLUE_INC] < 80)
		{
			out[i] = 255;
			out[i + RGB_GREEN_INC] = 0;
			out[i + RGB_BLUE_INC] = 0;
		}
		else
		{
			out[i] = in[i];
			out[i + RGB_GREEN_INC] = in[i+RGB_GREEN_INC];
			out[i + RGB_BLUE_INC] = in[i+ RGB_BLUE_INC];
		}
	}
}

bool isRedEnough(unsigned char* pixels, int pixelInc)
{
	return (pixels[pixelInc] > 240 && pixels[pixelInc+RGB_GREEN_INC] < 20 && pixels[pixelInc+RGB_BLUE_INC] < 50);
}

void grayScalePixel(unsigned char* pixels, int pixelInc)
{
	int grayscale = (double)(pixels[pixelInc]) * GRAYSCALE_RED_MULT + (double)(pixels[pixelInc+RGB_GREEN_INC]) * GRAYSCALE_BLUE_MULT + (double)(pixels[pixelInc+RGB_BLUE_INC]) * GRAYSCALE_GREEN_MULT;
	pixels[pixelInc] = grayscale;
	pixels[pixelInc+RGB_GREEN_INC] = grayscale;
	pixels[pixelInc+RGB_BLUE_INC] = grayscale;
}

void processFilterFour(unsigned char* in, unsigned char* out, int width, int height)
{
	int i = 0; 
	int totalPixels = width * height * RGB_COLORS_AMOUNT;
	int grayscale = 0;

	for (i = 0; i < totalPixels; i += RGB_COLORS_AMOUNT)
	{
		if ((in[i] > 100 && in[i + RGB_GREEN_INC] < 80 && in[i + RGB_BLUE_INC] < 80))
		{
			//out[i] = 255;
			//out[i + RGB_GREEN_INC] = 0;
			//out[i + RGB_BLUE_INC] = 0;
			out[i] = in[i];
			out[i + RGB_GREEN_INC] = in[i + RGB_GREEN_INC];
			out[i + RGB_BLUE_INC] = in[i + RGB_BLUE_INC];
		}
		else
		{
			grayscale = (double)(in[i]) * GRAYSCALE_RED_MULT + (double)(in[i + RGB_GREEN_INC]) * GRAYSCALE_BLUE_MULT + (double)(in[i + RGB_BLUE_INC]) * GRAYSCALE_GREEN_MULT;
			out[i] = grayscale;
		
			out[i + RGB_GREEN_INC] = grayscale;
			out[i + RGB_BLUE_INC] = grayscale;
		}
	}
}

void processFilterLGBT(unsigned char* in, unsigned char* out, int width, int height)
{
	int i = 0; 
	int totalPixels = width * height * RGB_COLORS_AMOUNT;

	for (i = 0; i < totalPixels; i += RGB_COLORS_AMOUNT)
	{
		if (in[i] >= 110 && in[i + RGB_GREEN_INC] >= 110 && in[i + RGB_BLUE_INC] >= 110)
		{
			if (i >= 0 && i <= (double)(totalPixels)/6)
			{
				out[i] = 255; 
				out[i + RGB_GREEN_INC] = 0;
				out[i + RGB_BLUE_INC] = 0;
			}
			else if (i >  (double)(totalPixels)/6 && i <= 2 * (double)(totalPixels)/6)
			{
				out[i] = 255;
				out[i + RGB_GREEN_INC] = 165;
				out[i + RGB_BLUE_INC] = 0;
			}
			else if (i > 2 * (double)(totalPixels)/6 && i <= 3 * (double)(totalPixels)/6)
			{
				out[i] = 255;
				out[i + RGB_GREEN_INC] = 255;
				out[i + RGB_BLUE_INC] = 0;
			}
			else if (i > 3 * (double)(totalPixels)/6 && i <= 4* (double)(totalPixels)/6)
			{
				out[i] = 0;
				out[i + RGB_GREEN_INC] = 255;
				out[i + RGB_BLUE_INC] = 0;
			}
			else if (i > 4 * (double)(totalPixels)/6 && i <= 5 * (double)(totalPixels)/6)
			{
				out[i] = 0;
				out[i + RGB_GREEN_INC] = 0;
				out[i + RGB_BLUE_INC] = 255;
			}
			else if (i > 5*(double)(totalPixels)/6 && i <= (double)(totalPixels))
			{
				out[i] = 128;
				out[i + RGB_GREEN_INC] = 0;
				out[i + RGB_BLUE_INC] = 128;
			}
		}
		else
		{
			out[i] = in[i]; 
			out[i + RGB_GREEN_INC] = in[i + RGB_GREEN_INC];
			out[i + RGB_BLUE_INC] = in[i + RGB_BLUE_INC];
		}
	}
}

void processFilterIsrael (unsigned char* in, unsigned char* out, int width, int height)
{
	int i = 0;
	int totalPixels = width * height * RGB_COLORS_AMOUNT;

	for (i = 0; i < totalPixels; i += RGB_COLORS_AMOUNT)
	{
		if (in[i] >= 100 && in[i + RGB_GREEN_INC] >= 100 && in[i + RGB_BLUE_INC] >= 100)
		{
			if (i >= 0 && i <= (double)(totalPixels) / 6)
			{
				out[i] = 255;
				out[i + RGB_GREEN_INC] = 255;
				out[i + RGB_BLUE_INC] = 255;
			}
			else if (i > (double)(totalPixels) / 6 && i <= 2 * (double)(totalPixels) / 6)
			{
				out[i] = 0;
				out[i + RGB_GREEN_INC] = 0;
				out[i + RGB_BLUE_INC] = 255;
			}
			else if (i > 2 * (double)(totalPixels) / 6 && i <= 3 * (double)(totalPixels) / 6)
			{
				out[i] = 255;
				out[i + RGB_GREEN_INC] = 255;
				out[i + RGB_BLUE_INC] = 255;
			}
			else if (i > 3 * (double)(totalPixels) / 6 && i <= 4 * (double)(totalPixels) / 6)
			{
				out[i] = 255;
				out[i + RGB_GREEN_INC] = 255;
				out[i + RGB_BLUE_INC] = 255;
			}
			else if (i > 4 * (double)(totalPixels) / 6 && i <= 5 * (double)(totalPixels) / 6)
			{
				out[i] = 0;
				out[i + RGB_GREEN_INC] = 0;
				out[i + RGB_BLUE_INC] = 255;
			}
			else if (i > 5 * (double)(totalPixels) / 6 && i <= (double)(totalPixels))
			{
				out[i] = 255;
				out[i + RGB_GREEN_INC] = 255;
				out[i + RGB_BLUE_INC] = 255;
			}
		}
		else
		{
			out[i] = in[i];
			out[i + RGB_GREEN_INC] = in[i + RGB_GREEN_INC];
			out[i + RGB_BLUE_INC] = in[i + RGB_BLUE_INC];
		}
	}
}

Pixel createPixel(unsigned char red, unsigned char green, unsigned char blue)
{
	Pixel pixel;
	pixel.red = red;
	pixel.green = green;
	pixel.blue = blue;

	return pixel;
}

Pixel** createPixelArray(unsigned char* in, int width, int height)
{
	Pixel** pixels = NULL;
	int i = 0;
	int j = 0;

	pixels = (Pixel**)malloc(sizeof(Pixel*) * height);
	if (!pixels)
	{
		printf("Memory allocation error!\n");
		exit(1);
	}

	for (i = 0; i < height; i++)
	{
		pixels[i] = (Pixel*)malloc(sizeof(Pixel) * width);
		if (!pixels[i])
		{
			printf("Memory allocation error!\n");
			exit(1);
		}
	}

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pixels[i][j].red = in[(i * width + j) * RGB_COLORS_AMOUNT];
			pixels[i][j].green = in[(i * width + j) * RGB_COLORS_AMOUNT + RGB_GREEN_INC];
			pixels[i][j].blue = in[(i * width + j) * RGB_COLORS_AMOUNT + RGB_BLUE_INC];
		}
	}

	return pixels;
}

void freePixelArray(Pixel** pixels, int height)
{
	int i = 0;

	for (i = 0; i < height; i++)
	{
		free(pixels[i]);
	}
	free(pixels);
}

void flipUpperPixelsToLower(Pixel** pixels, int width, int height)
{
	int i = 0;
	int j = 0;

	for (i = 0; i < height / GET_HALF; i++)
	{
		for (j = 0; j < width; j++)
		{
			pixels[i + height / GET_HALF][j] = pixels[height / GET_HALF - i][j];
		}
	}
}

void processFilterFlip(unsigned char* in, unsigned char* out, int width, int height)
{
	Pixel** pixels = createPixelArray(in, width, height);
	int i = 0;
	int j = 0;

	flipUpperPixelsToLower(pixels, width, height);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			out[(i * width + j) * RGB_COLORS_AMOUNT] = pixels[i][j].red;
			out[(i * width + j) * RGB_COLORS_AMOUNT + RGB_GREEN_INC] = pixels[i][j].green;
			out[(i * width + j) * RGB_COLORS_AMOUNT + RGB_BLUE_INC] = pixels[i][j].blue;
		}
	}

	freePixelArray(pixels, height);
}

void processFilterPixelation(unsigned char* in, unsigned char* out, int width, int height)
{
	Pixel** pixels = createPixelArray(in, width, height);
	int i = 0;
	int j = 0;
	int k = 0;
	int q = 0;

	for (i = 0; i < height; i += PIXELATION_BLOCK_SIZE)
	{
		for (j = 0; j < width; j += PIXELATION_BLOCK_SIZE)
		{
			for (q = 0; q < PIXELATION_BLOCK_SIZE; q++)
			{
				for (k = 0; k < PIXELATION_BLOCK_SIZE; k++)
				{
					out[((i + q) * width + j + k) * RGB_COLORS_AMOUNT] = pixels[i][j].red;
					out[((i + q) * width + j + k) * RGB_COLORS_AMOUNT + RGB_GREEN_INC] = pixels[i][j].green;
					out[((i + q) * width + j + k) * RGB_COLORS_AMOUNT + RGB_BLUE_INC] = pixels[i][j].blue;
				}
			}
		}
	}

	freePixelArray(pixels, height);
}

bool isBlueish(Pixel pixel)
{
	return (pixel.blue >= 100 && pixel.blue >= 2.3*pixel.red && pixel.blue >= 2.3*pixel.green);
}

void processFilterWriteWithBlue(unsigned char* in, unsigned char* out, int width, int height)
{
	Pixel** pixels = createPixelArray(in, width, height);
	int i = 0;
	int j = 0;
	double factor = 0.0f;

	if (startBluePixelsSave)
	{
		bluePixels = (bool*)calloc(width * height, sizeof(bool)); /* calloc to initialize with false values */
		if (!bluePixels)
		{
			printf("Memory reallocation error!\n");
			exit(1);
		}
		startBluePixelsSave = false; 
	}
	else
	{
		bluePixels = (bool*)realloc(bluePixels, sizeof(bool) * width * height);
		if (!bluePixels)
		{
			printf("Memory reallocation error!\n");
			exit(1);
		}

		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				if (bluePixels[i*width+j] != true)
				{
					bluePixels[i*width+j] = false; 
				}
			}
		}
		
	}

	for (i = 0; i < height; i ++)
	{
		for (j = 0; j < width; j ++)
		{
			//factor = (double)(pixels[i][j].blue) / (double)(pixels[i][j].red + pixels[i][j].green + pixels[i][j].blue);
			if (isBlueish(pixels[i][j]))
			{
				bluePixels[i * width + j] = true; 
			}
			else
			{
				if (!bluePixels[i * width + j])
				{
					out[(i * width + j) * RGB_COLORS_AMOUNT] = pixels[i][j].red;
					out[(i * width + j) * RGB_COLORS_AMOUNT + RGB_GREEN_INC] = pixels[i][j].green;
					out[(i * width + j) * RGB_COLORS_AMOUNT + RGB_BLUE_INC] = pixels[i][j].blue;
				}
			}
		}
	}

	freePixelArray(pixels, height);
}

void flipPixelsTriangular(Pixel** pixels, int width, int height)
{
	int i = 0; 
	int j = 0; 
	float dec = 0.0f;
	float decInc = (double)(width) / height;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width - (int)(dec); j++)
		{
			pixels[height - DEC - i][width - DEC - j] = pixels[i][j];
		}
		dec += decInc;
	}
}

void processFilterTriangleFlip(unsigned char* in, unsigned char* out, int width, int height)
{
	Pixel** pixels = createPixelArray(in, width, height);
	int i = 0;
	int j = 0;

	flipPixelsTriangular(pixels, width, height);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			out[(i * width + j) * RGB_COLORS_AMOUNT] = pixels[i][j].red;
			out[(i * width + j) * RGB_COLORS_AMOUNT + RGB_GREEN_INC] = pixels[i][j].green;
			out[(i * width + j) * RGB_COLORS_AMOUNT + RGB_BLUE_INC] = pixels[i][j].blue;
		}
	}

	freePixelArray(pixels, height);
}

static int clamp(int value, int lower, int higher)
{
	if (value < lower)
	{
		return lower;
	}
	else if (value > higher)
	{
		return higher;
	}
	return value;
}

static void addDarkFrame(Pixel** pixels, int width, int height)
{
	int x = 0;
	int y = 0;
	int tone = MAX_DARK_FRAME_TONE * DARK_FRAME_TONE_MUL;

	for (x = 0; x < MAX_DARK_FRAME_TONE; x++)
	{
		for (y = 0; y < height; y++)
		{
			pixels[y][x].red = clamp((pixels[y][x].red - tone), MIN_VALUE, MAX_VALUE);
			pixels[y][x].green = clamp((pixels[y][x].green - tone), MIN_VALUE, MAX_VALUE);
			pixels[y][x].blue = clamp((pixels[y][x].blue - tone), MIN_VALUE, MAX_VALUE);
			pixels[y][width - DEC - x].red = clamp((pixels[y][width - DEC - x].red - tone), MIN_VALUE, MAX_VALUE);
			pixels[y][width - DEC - x].green = clamp((pixels[y][width - DEC - x].green - tone), MIN_VALUE, MAX_VALUE);
			pixels[y][width - DEC - x].blue = clamp((pixels[y][width - DEC - x].blue - tone), MIN_VALUE, MAX_VALUE);
		}
		tone -= DARK_FRAME_TONE_MUL;
	}

	tone = MAX_DARK_FRAME_TONE * DARK_FRAME_TONE_MUL;

	for (y = 0; y<MAX_DARK_FRAME_TONE; y++)
	{
		for (x = 0; x < width; x++)
		{
			pixels[y][x].red = clamp((pixels[y][x].red - tone), MIN_VALUE, MAX_VALUE);
			pixels[y][x].green = clamp((pixels[y][x].green - tone), MIN_VALUE, MAX_VALUE);
			pixels[y][x].blue = clamp((pixels[y][x].blue - tone), MIN_VALUE, MAX_VALUE);
			pixels[height - DEC - y][x].red = clamp((pixels[height - DEC - y][x].red - tone), MIN_VALUE, MAX_VALUE);
			pixels[height - DEC - y][x].green = clamp((pixels[height - DEC - y][x].green - tone), MIN_VALUE, MAX_VALUE);
			pixels[height - DEC - y][x].blue = clamp((pixels[height - DEC - y][x].blue - tone), MIN_VALUE, MAX_VALUE);
		}
		tone -= DARK_FRAME_TONE_MUL;
	}
}

void processFilterDarkFrame(unsigned char* in, unsigned char* out, int width, int height)
{
	Pixel** pixels = createPixelArray(in, width, height);
	int i = 0;
	int j = 0;

	addDarkFrame(pixels, width, height);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			out[(i * width + j) * RGB_COLORS_AMOUNT] = pixels[i][j].red;
			out[(i * width + j) * RGB_COLORS_AMOUNT + RGB_GREEN_INC] = pixels[i][j].green;
			out[(i * width + j) * RGB_COLORS_AMOUNT + RGB_BLUE_INC] = pixels[i][j].blue;
		}
	}

	freePixelArray(pixels, height);
}

static void addWhiteBottomGlow(Pixel** pixels, int width, int height)
{
	int y = 0; 
	int x = 0; 
	int thirdPartHeight = (height - DEC) / GET_THIRD_PART;
	int tone = MAX_VALUE;

	for (y = height - DEC - thirdPartHeight; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pixels[y][x].red = clamp(MAX_VALUE + pixels[y][x].red - tone, MIN_VALUE, MAX_VALUE);
			pixels[y][x].green = clamp(MAX_VALUE + pixels[y][x].green - tone, MIN_VALUE, MAX_VALUE);
			pixels[y][x].blue = clamp(MAX_VALUE + pixels[y][x].blue - tone, MIN_VALUE, MAX_VALUE);
		}
		tone -= BOTTOM_GLOW_DEC;
	}

}

void processFilterWhiteBottomGlow(unsigned char* in, unsigned char* out, int width, int height)
{
	Pixel** pixels = createPixelArray(in, width, height);
	int i = 0;
	int j = 0;

	addWhiteBottomGlow(pixels, width, height);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			out[(i * width + j) * RGB_COLORS_AMOUNT] = pixels[i][j].red;
			out[(i * width + j) * RGB_COLORS_AMOUNT + RGB_GREEN_INC] = pixels[i][j].green;
			out[(i * width + j) * RGB_COLORS_AMOUNT + RGB_BLUE_INC] = pixels[i][j].blue;
		}
	}

	freePixelArray(pixels, height);
}

static void stickmanWorldFill(Pixel** pixels, int width, int height)
{
	int y = 0;
	int x = 0;
	int yi = 0;
	int xi = 0;
	int found = 0;

	for (y = 1; y < height - DEC; y++)
	{
		for (x = 1; x < width - DEC; x++)
		{
			found = 0;
			for (yi = y - DEC; yi <= y + INC; yi++)
			{
				for (xi = x - DEC; xi <= x + INC; xi++)
				{
					if (((pixels[y][x].red >= CONTRAST_LIGHT && pixels[yi][xi].red <= CONTRAST_DARK) && (pixels[y][x].blue >= CONTRAST_LIGHT && pixels[yi][xi].blue <= CONTRAST_DARK) && (pixels[y][x].green >= CONTRAST_LIGHT && pixels[yi][xi].green <= CONTRAST_DARK)) || ((pixels[y][x].red <= CONTRAST_DARK && pixels[yi][xi].red >= CONTRAST_LIGHT) && (pixels[y][x].blue <= CONTRAST_DARK && pixels[yi][xi].blue >= CONTRAST_LIGHT) && (pixels[y][x].green <= CONTRAST_DARK && pixels[yi][xi].green >= CONTRAST_LIGHT)))
					{
						found = 1;
						pixels[y][x].red = MIN_VALUE;
						pixels[y][x].green = MIN_VALUE;
						pixels[y][x].blue = MIN_VALUE;
					}
				}
			}
			if (!found)
			{
				pixels[y][x].red = MAX_VALUE;
				pixels[y][x].green = MAX_VALUE;
				pixels[y][x].blue = MAX_VALUE;
			}
		}
	}
}

void processFilterStickmanWorld(unsigned char* in, unsigned char* out, int width, int height)
{
	Pixel** pixels = createPixelArray(in, width, height);
	int i = 0;
	int j = 0;

	stickmanWorldFill(pixels, width, height);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			out[(i * width + j) * RGB_COLORS_AMOUNT] = pixels[i][j].red;
			out[(i * width + j) * RGB_COLORS_AMOUNT + RGB_GREEN_INC] = pixels[i][j].green;
			out[(i * width + j) * RGB_COLORS_AMOUNT + RGB_BLUE_INC] = pixels[i][j].blue;
		}
	}

	freePixelArray(pixels, height);
}