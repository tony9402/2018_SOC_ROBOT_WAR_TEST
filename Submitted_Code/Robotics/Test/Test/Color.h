#pragma once

#pragma region DONT_TOUCH
//#define IMAGE_SCALE_640_480
#define IMAGE_SCALE_320_240

#define BYTE unsigned char
#define UINT unsigned int
#define max(x, y, z) ((x) > (y) ? (x) : (y)) > (z) ? ((x) > (y) ? (x) : (y)) : (z)
#define min(x, y, z) ((x) > (y) ? (y) : (x)) > (z) ? (z) : ((x) > (y) ? (y) : (x))
#define color_max_in(x) (x) > 255 ? 255 : ((x) < 0 ? 0 : (x))

#ifdef IMAGE_SCALE_640_480
	#define height 480
	#define width 640
	#define ColorTrans(i, j, color) (((i) * width + (j)) * 3 + color)
	#ifndef Colors
		#define RED 2
		#define GREEN 1
		#define BLUE 0
		#define H 2
		#define S 1
		#define V 0
		#define color_scale 256
	#endif
#elif defined IMAGE_SCALE_320_240
	#define height 240
	#define width 320
	#define ColorTrans(i, j, color) (((i) * 640 + (j)) * 3 + (color))
	#ifndef Colors
		#define RED 2
		#define GREEN 1
		#define BLUE 0
		#define H 2
		#define S 1
		#define V 0
		#define color_scale 256
	#endif
#endif

#define ptile_Binary     0 
#define iterative_Binary 1
#define Otsu_Binary      2

#define GaussianFiliering 0

BYTE mRGB[height][width][3] = { 0 };
BYTE mHSV[height][width][3] = { 0 };
BYTE mGRAY[height][width] = { 0 };

void ColorFilitering();
bool IsPocarIColor(short i, short j);

class __color
{
public:
	static void RGBtoGRAY(BYTE(*mRGB)[width][3], BYTE(*mGRAY)[width], int i, int j);
	static void GRAYtoRGB(BYTE (*mRGB)[width][3], BYTE (*mGRAY)[width], int i, int j);
	static void RGBtoHSV(BYTE(*mRGB)[width][3], BYTE(*mHSV)[width][3], int i, int j);
	static void HSVtoRGB(BYTE(*mRGB)[width][3], BYTE(*mHSV)[width][3], int i, int j);
	static void Input(LPVIDEOHDR lpVHdr);
	static void Output(LPVIDEOHDR lpVHdr);
	static void Filitering(BYTE(*mGRAY)[width], int value);
	
};

#pragma region DONT_TOUCH
void __color::RGBtoGRAY(BYTE (*mRGB)[width][3], BYTE (*mGRAY)[width], int i, int j)
{
	mGRAY[i][j] = (mRGB[i][j][RED] + mRGB[i][j][GREEN] + mRGB[i][j][BLUE]) / 3;
	return;
}

void __color::GRAYtoRGB(BYTE (*mRGB)[width][3], BYTE(*mGRAY)[width], int i, int j)
{
	mRGB[i][j][RED] = mRGB[i][j][GREEN] = mRGB[i][j][BLUE] = mGRAY[i][j];
	return;
}

void __color::RGBtoHSV(BYTE(*mRGB)[width][3], BYTE(*mHSV)[width][3], int i, int j)
{
	BYTE cmax, cmin;
	cmax = max(mRGB[i][j][RED], mRGB[i][j][GREEN], mRGB[i][j][BLUE]);
	cmin = min(mRGB[i][j][RED], mRGB[i][j][GREEN], mRGB[i][j][BLUE]);

	mHSV[i][j][V] = cmax;
	if (!mHSV[i][j][V])
	{
		mHSV[i][j][H] = 0;
		mHSV[i][j][S] = 0;
		return;
	}

	mHSV[i][j][S] = 255 * (long)(cmax - cmin) / mHSV[i][j][V];
	if (!mHSV[i][j][S])
	{
		mHSV[i][j][H] = 0;
		return;
	}

	if (cmax == mRGB[i][j][RED])
	{
		mHSV[i][j][H] = 0 + 43 * (mRGB[i][j][GREEN] - mRGB[i][j][BLUE]) / (cmax - cmin);
	}
	else if (cmax == mRGB[i][j][GREEN])
	{
		mHSV[i][j][H] = 85 + 43 * (mRGB[i][j][BLUE] - mRGB[i][j][RED]) / (cmax - cmin);
	}
	else {
		mHSV[i][j][H] = 171 + 43 * (mRGB[i][j][RED] - mRGB[i][j][GREEN]) / (cmax - cmin);
	}
	return;
}

void __color::HSVtoRGB(BYTE(*mRGB)[width][3], BYTE(*mHSV)[width][3], int i, int j)
{
	BYTE region, remainder, p, q, t;
	if (!mHSV[i][j][S])
	{
		mRGB[i][j][RED] = mRGB[i][j][GREEN] = mRGB[i][j][BLUE] = mHSV[i][j][V];
		return;
	}
	region = mHSV[i][j][H] / 43;
	remainder = (mHSV[i][j][H] - (region * 43)) * 6;

	p = (mHSV[i][j][V] * (255 - mHSV[i][j][S])) >> 8;
	q = (mHSV[i][j][V] * (255 - ((mHSV[i][j][S] * remainder) >> 8))) >> 8;
	t = (mHSV[i][j][V] * (255 - ((mHSV[i][j][S] * (255 - remainder)) >> 8))) >> 8;

	switch (region)
	{
	case 0:
		mRGB[i][j][RED] = mHSV[i][j][V];
		mRGB[i][j][GREEN] = t;
		mRGB[i][j][BLUE] = p;
		break;
	case 1:
		mRGB[i][j][RED] = q;
		mRGB[i][j][GREEN] = mHSV[i][j][V];
		mRGB[i][j][BLUE] = p;
		break;
	case 2:
		mRGB[i][j][RED] = p;
		mRGB[i][j][GREEN] = mHSV[i][j][V];
		mRGB[i][j][BLUE] = t;
		break;
	case 3:
		mRGB[i][j][RED] = p;
		mRGB[i][j][GREEN] = q;
		mRGB[i][j][BLUE] = mHSV[i][j][V];
		break;
	case 4:
		mRGB[i][j][RED] = t;
		mRGB[i][j][GREEN] = p;
		mRGB[i][j][BLUE] = mHSV[i][j][V];
		break;
	default:
		mRGB[i][j][RED] = mHSV[i][j][V];
		mRGB[i][j][GREEN] = p;
		mRGB[i][j][BLUE] = q;
		break;
	}

	return;
}
#pragma endregion

#pragma region Input
void __color::Input(LPVIDEOHDR lpVHdr)
{
	short i, j;
#if height==480
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			mRGB[i][j][RED] = *(lpVHdr->lpData + ColorTrans(i, j, RED));
			mRGB[i][j][GREEN] = *(lpVHdr->lpData + ColorTrans(i, j, GREEN));
			mRGB[i][j][BLUE] = *(lpVHdr->lpData + ColorTrans(i, j, BLUE));
			__color::RGBtoGRAY(mRGB, mGRAY, i, j);
			Histogram[mGRAY[i][j]]++;
		}
	}
	
#else
	short sum_R, sum_G, sum_B;
	short m, n;

	for (i = 0; i < height * 2; i += 2)
	{
		for (j = 0; j < width * 2; j += 2)
		{
			sum_R = sum_G = sum_B = 0;
			for (m = 0; m < 2; m++)
			{
				for (n = 0; n < 2; n++)
				{
					sum_R += *(lpVHdr->lpData + ColorTrans(i+m, j+n, RED));
					sum_G += *(lpVHdr->lpData + ColorTrans(i+m, j+n, GREEN));
					sum_B += *(lpVHdr->lpData + ColorTrans(i+m, j+n, BLUE));
				}
			}
			mRGB[i/2][j/2][RED] = sum_R / 4;
			mRGB[i/2][j/2][GREEN] = sum_G / 4;
			mRGB[i/2][j/2][BLUE] = sum_B / 4;
			__color::RGBtoGRAY(mRGB, mGRAY, i / 2, j / 2);
			//Histogram[mGRAY[i/2][j/2]]++;
		}
	}
#endif

	ColorFilitering();

	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			__color::RGBtoHSV(mRGB, mHSV, i, j);


	return;
}
#pragma endregion

#pragma region Output
void __color::Output(LPVIDEOHDR lpVHdr)
{
	short i, j;
#if height==480
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			__color::GRAYtoRGB(mRGB, mGRAY, i, j);
			*(lpVHdr->lpData + ColorTrans(i, j, RED)) = mRGB[i][j][RED];
			*(lpVHdr->lpData + ColorTrans(i, j, GREEN)) = mRGB[i][j][GREEN];
			*(lpVHdr->lpData + ColorTrans(i, j, BLUE)) = mRGB[i][j][BLUE];
		}
	}
#else
	short sum_R, sum_G, sum_B;
	for (j = 0; j < width * 2; j++)
	{
		for (i = 0; i < height * 2; i++)
		{
			if(i<height && j<width){
				*(lpVHdr->lpData + ColorTrans(i, j, RED)) = mGRAY[i][j];
				*(lpVHdr->lpData + ColorTrans(i, j, GREEN)) = mGRAY[i][j];
				*(lpVHdr->lpData + ColorTrans(i, j, BLUE)) = mGRAY[i][j];
			}
			else if (j >= width && i < height)
			{
#pragma region No_Use
				//Just Nothing
				/*
				if (j <= width + 256)
				{
					if (pHistogram[j - width] * width <= i * 0.009)
					{
						*(lpVHdr->lpData + ColorTrans(i, j, RED)) = 0;
						*(lpVHdr->lpData + ColorTrans(i, j, GREEN)) = 0;
						*(lpVHdr->lpData + ColorTrans(i, j, BLUE)) = 0;
					}
					else {
						*(lpVHdr->lpData + ColorTrans(i, j, RED)) = 255;
						*(lpVHdr->lpData + ColorTrans(i, j, GREEN)) = 255;
						*(lpVHdr->lpData + ColorTrans(i, j, BLUE)) = 255;
					}
				}
				else {
					if (j == width + 257) {
						*(lpVHdr->lpData + ColorTrans(i, j, RED)) = 0;
						*(lpVHdr->lpData + ColorTrans(i, j, GREEN)) = 255;
						*(lpVHdr->lpData + ColorTrans(i, j, BLUE)) = 0;
					}
					else {
						*(lpVHdr->lpData + ColorTrans(i, j, RED)) = 0;
						*(lpVHdr->lpData + ColorTrans(i, j, GREEN)) = 0;
						*(lpVHdr->lpData + ColorTrans(i, j, BLUE)) = 0;
					}
				}*/
#pragma endregion

				*(lpVHdr->lpData + ColorTrans(i, j, RED)) = 0;
				*(lpVHdr->lpData + ColorTrans(i, j, GREEN)) = 0;
				*(lpVHdr->lpData + ColorTrans(i, j, BLUE)) = 0;
			}else if (j<width && i>=height)
			{
				*(lpVHdr->lpData + ColorTrans(i, j, RED)) = mRGB[i-height][j][RED];
				*(lpVHdr->lpData + ColorTrans(i, j, GREEN)) = mRGB[i - height][j][GREEN];
				*(lpVHdr->lpData + ColorTrans(i, j, BLUE)) = mRGB[i - height][j][BLUE];
			}else if (i >= height && j >= width)
			{
				sum_R = sum_G = sum_B = 0;
				if (i < (480 - height / 2) && j < (640 - width / 2)) {
					for (int m = 0; m < 2; m++)
					{
						for (int n = 0; n < 2; n++)
						{
							sum_B += mRGB[(i - height) * 2 + m][(j - width) * 2 + n][BLUE];
						}
					}
					*(lpVHdr->lpData + ColorTrans(i, j, BLUE)) = sum_B / 4;
					*(lpVHdr->lpData + ColorTrans(i, j, GREEN)) = 0;
					*(lpVHdr->lpData + ColorTrans(i, j, RED)) = 0;
				}
				else if (i >= (480 - height / 2) && j < (640 - width / 2))
				{
					for (int m = 0; m < 2; m++)
					{
						for (int n = 0; n < 2; n++)
						{
							sum_R += mRGB[(i + height / 2 - 480) * 2 + m][(j - width) * 2 + n][RED];
						}
					}
					*(lpVHdr->lpData + ColorTrans(i, j, RED)) = sum_R / 4;
					*(lpVHdr->lpData + ColorTrans(i, j, GREEN)) = 0;
					*(lpVHdr->lpData + ColorTrans(i, j, BLUE)) = 0;
				}
				else if (i >= (480 - height / 2) && j >= (640 - width / 2))
				{
					for (int m = 0; m < 2; m++)
					{
						for (int n = 0; n < 2; n++)
						{
							sum_G += mRGB[(i + height / 2 - 480) * 2 + m][(j + width / 2 - 640) * 2 + n][GREEN];
						}
					}
					*(lpVHdr->lpData + ColorTrans(i, j, GREEN)) = sum_G / 4;
					*(lpVHdr->lpData + ColorTrans(i, j, RED)) = 0;
					*(lpVHdr->lpData + ColorTrans(i, j, BLUE)) = 0;
				}
				else {
					for (int m = 0; m < 2; m++)
					{
						for (int n = 0; n < 2; n++)
						{
							sum_G += mHSV[(i - height) * 2 + m][(j + width / 2 - 640) * 2 + n][H];
						}
					}
					*(lpVHdr->lpData + ColorTrans(i, j, RED)) = *(lpVHdr->lpData + ColorTrans(i, j, GREEN)) = *(lpVHdr->lpData + ColorTrans(i, j, BLUE)) = color_max_in(sum_G / 4);
				}
			}
				
		}
	}
#endif
}
#pragma endregion
  
#pragma region Filitering
void __color::Filitering(BYTE(*mGRAY)[width], int value)
{
	BYTE **sGRAY = (BYTE**)malloc(sizeof(BYTE*)*height);
	

	const BYTE Gaussian[5][5] =
	{ { 2,4,5,4,2 },
	{ 4,9,12,9,4 },
	{ 5,12,15,12,5 },
	{ 4,9,12,9,4 },
	{ 2,4,5,4,2 } };

	const BYTE GaussianDivide = 159;

	short i, j;
	for (i = 0; i < height; i++)
	{
		sGRAY[i] = (BYTE*)malloc(sizeof(BYTE)*width);
		memcpy(sGRAY[i], mGRAY[i], sizeof(BYTE)*width);
	}
	for (i = 2; i < height - 2; i++)
	{
		for (j = 2; j < width - 2; j++)
		{
			mGRAY[i][j] = (Gaussian[0][0] * sGRAY[i-2][j-2] + Gaussian[0][1] * sGRAY[i-2][j-1] + Gaussian[0][2] * sGRAY[i-2][j] + Gaussian[0][3] * sGRAY[i-2][j+1] + Gaussian[0][4] * sGRAY[i-2][j+2]+
				Gaussian[1][0] * sGRAY[i - 1][j - 2] + Gaussian[1][1] * sGRAY[i - 1][j - 1] + Gaussian[1][2] * sGRAY[i - 1][j] + Gaussian[1][3] * sGRAY[i - 1][j + 1] + Gaussian[1][4] * sGRAY[i - 1][j + 2] +
				Gaussian[2][0] * sGRAY[i][j - 2] + Gaussian[2][1] * sGRAY[i][j - 1] + Gaussian[2][2] * sGRAY[i][j] + Gaussian[2][3] * sGRAY[i][j + 1] + Gaussian[2][4] * sGRAY[i][j + 2] +
				Gaussian[3][0] * sGRAY[i + 1][j - 2] + Gaussian[3][1] * sGRAY[i + 1][j - 1] + Gaussian[3][2] * sGRAY[i + 1][j] + Gaussian[3][3] * sGRAY[i + 1][j + 1] + Gaussian[3][4] * sGRAY[i + 1][j + 2] +
				Gaussian[4][0] * sGRAY[i + 2][j - 2] + Gaussian[4][1] * sGRAY[i + 2][j - 1] + Gaussian[4][2] * sGRAY[i + 2][j] + Gaussian[4][3] * sGRAY[i + 2][j + 1] + Gaussian[4][4] * sGRAY[i + 2][j + 2]) / GaussianDivide;
		}
	}

	for (i = 0; i < height; i++)
	{
		free(sGRAY[i]);
	}
	free(sGRAY);
	return;
}
#pragma endregion

void ColorFilitering()
{
#pragma region 1
	short i, j;
	BYTE **FRED = (BYTE**)malloc(sizeof(BYTE*) * height);
	BYTE **FGREEN = (BYTE**)malloc(sizeof(BYTE*) * height);
	BYTE **FBLUE = (BYTE**)malloc(sizeof(BYTE*) * height);

	for (i = 0; i < height; i++)
	{
		FRED[i] = (BYTE*)malloc(sizeof(BYTE) * width);
		FGREEN[i] = (BYTE*)malloc(sizeof(BYTE) * width);
		FBLUE[i] = (BYTE*)malloc(sizeof(BYTE) * width);
		for (j = 0; j < width; j++)
		{
			FRED[i][j] = mRGB[i][j][RED];
			FGREEN[i][j] = mRGB[i][j][GREEN];
			FBLUE[i][j] = mRGB[i][j][BLUE];
		}
	}

	const BYTE Gaussian[5][5] =
	{ { 2,4,5,4,2 },
	{ 4,9,12,9,4 },
	{ 5,12,15,12,5 },
	{ 4,9,12,9,4 },
	{ 2,4,5,4,2 } };

	const BYTE GaussianDivide = 159;

#pragma endregion
	

	for (i = 2; i < height - 2; i++)
	{
		for (j = 2; j < width - 2; j++)
		{
			mRGB[i][j][RED] = (Gaussian[0][0] * FRED[i - 2][j - 2] + Gaussian[0][1] * FRED[i - 2][j - 1] + Gaussian[0][2] * FRED[i - 2][j] + Gaussian[0][3] * FRED[i - 2][j + 1] + Gaussian[0][4] * FRED[i - 2][j + 2] +
				Gaussian[1][0] * FRED[i - 1][j - 2] + Gaussian[1][1] * FRED[i - 1][j - 1] + Gaussian[1][2] * FRED[i - 1][j] + Gaussian[1][3] * FRED[i - 1][j + 1] + Gaussian[1][4] * FRED[i - 1][j + 2] +
				Gaussian[2][0] * FRED[i][j - 2] + Gaussian[2][1] * FRED[i][j - 1] + Gaussian[2][2] * FRED[i][j] + Gaussian[2][3] * FRED[i][j + 1] + Gaussian[2][4] * FRED[i][j + 2] +
				Gaussian[3][0] * FRED[i + 1][j - 2] + Gaussian[3][1] * FRED[i + 1][j - 1] + Gaussian[3][2] * FRED[i + 1][j] + Gaussian[3][3] * FRED[i + 1][j + 1] + Gaussian[3][4] * FRED[i + 1][j + 2] +
				Gaussian[4][0] * FRED[i + 2][j - 2] + Gaussian[4][1] * FRED[i + 2][j - 1] + Gaussian[4][2] * FRED[i + 2][j] + Gaussian[4][3] * FRED[i + 2][j + 1] + Gaussian[4][4] * FRED[i + 2][j + 2]) / GaussianDivide;

			mRGB[i][j][GREEN] = (Gaussian[0][0] * FGREEN[i - 2][j - 2] + Gaussian[0][1] * FGREEN[i - 2][j - 1] + Gaussian[0][2] * FGREEN[i - 2][j] + Gaussian[0][3] * FGREEN[i - 2][j + 1] + Gaussian[0][4] * FGREEN[i - 2][j + 2] +
				Gaussian[1][0] * FGREEN[i - 1][j - 2] + Gaussian[1][1] * FGREEN[i - 1][j - 1] + Gaussian[1][2] * FGREEN[i - 1][j] + Gaussian[1][3] * FGREEN[i - 1][j + 1] + Gaussian[1][4] * FGREEN[i - 1][j + 2] +
				Gaussian[2][0] * FGREEN[i][j - 2] + Gaussian[2][1] * FGREEN[i][j - 1] + Gaussian[2][2] * FGREEN[i][j] + Gaussian[2][3] * FGREEN[i][j + 1] + Gaussian[2][4] * FGREEN[i][j + 2] +
				Gaussian[3][0] * FGREEN[i + 1][j - 2] + Gaussian[3][1] * FGREEN[i + 1][j - 1] + Gaussian[3][2] * FGREEN[i + 1][j] + Gaussian[3][3] * FGREEN[i + 1][j + 1] + Gaussian[3][4] * FGREEN[i + 1][j + 2] +
				Gaussian[4][0] * FGREEN[i + 2][j - 2] + Gaussian[4][1] * FGREEN[i + 2][j - 1] + Gaussian[4][2] * FGREEN[i + 2][j] + Gaussian[4][3] * FGREEN[i + 2][j + 1] + Gaussian[4][4] * FGREEN[i + 2][j + 2]) / GaussianDivide;

			mRGB[i][j][BLUE] = (Gaussian[0][0] * FBLUE[i - 2][j - 2] + Gaussian[0][1] * FBLUE[i - 2][j - 1] + Gaussian[0][2] * FBLUE[i - 2][j] + Gaussian[0][3] * FBLUE[i - 2][j + 1] + Gaussian[0][4] * FBLUE[i - 2][j + 2] +
				Gaussian[1][0] * FBLUE[i - 1][j - 2] + Gaussian[1][1] * FBLUE[i - 1][j - 1] + Gaussian[1][2] * FBLUE[i - 1][j] + Gaussian[1][3] * FBLUE[i - 1][j + 1] + Gaussian[1][4] * FBLUE[i - 1][j + 2] +
				Gaussian[2][0] * FBLUE[i][j - 2] + Gaussian[2][1] * FBLUE[i][j - 1] + Gaussian[2][2] * FBLUE[i][j] + Gaussian[2][3] * FBLUE[i][j + 1] + Gaussian[2][4] * FBLUE[i][j + 2] +
				Gaussian[3][0] * FBLUE[i + 1][j - 2] + Gaussian[3][1] * FBLUE[i + 1][j - 1] + Gaussian[3][2] * FBLUE[i + 1][j] + Gaussian[3][3] * FBLUE[i + 1][j + 1] + Gaussian[3][4] * FBLUE[i + 1][j + 2] +
				Gaussian[4][0] * FBLUE[i + 2][j - 2] + Gaussian[4][1] * FBLUE[i + 2][j - 1] + Gaussian[4][2] * FBLUE[i + 2][j] + Gaussian[4][3] * FBLUE[i + 2][j + 1] + Gaussian[4][4] * FBLUE[i + 2][j + 2]) / GaussianDivide;
		}
	}


#pragma region 1
	for (i = 0; i < height; i++)
	{
		free(FRED[i]);
		free(FGREEN[i]);
		free(FBLUE[i]);
	}
	free(FRED);
	free(FGREEN);
	free(FBLUE);
	return;
#pragma endregion
}
#pragma endregion

bool IsPocarIColor(short i, short j)
{
	
	if (mHSV[i][j][H] >= 158 && mHSV[i][j][H] <= 162)
		if (mHSV[i][j][V] >= 240)
		{
			if (mHSV[i][j][S] >= 100)
			{
				return true;
			}
		}else if (mHSV[i][j][S] >= 150)
			return true;
	return false;
}

bool IsCantataColor(short i, short j)
{
	if (mHSV[i][j][H] >= 235 || mHSV[i][j][H] <= 12)
		if(mHSV[i][j][S] > 150)
			return true;
	return false;
}

bool IsPocaColor(short i, short j)
{
	if (mHSV[i][j][H] >= 32 && mHSV[i][j][H] <= 50)
		if (mHSV[i][j][S] > 155)
			return true;
	return false;
}

bool IsWheelColor(short i, short j)
{
	if (mHSV[i][j][H] >= 50 && mHSV[i][j][H] <= 80)
		if (mHSV[i][j][S] > 160)
			return true;
	return false;
}

bool IsShrimp(short i, short j)
{
	if ((mHSV[i][j][H] >= 250 || mHSV[i][j][H] <= 12) || mHSV[i][j][H] >= 22 && mHSV[i][j][H] <= 30)
		if(mHSV[i][j][S] >= 170)
			return true;
	return false;
}