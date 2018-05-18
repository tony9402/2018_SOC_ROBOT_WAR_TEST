#pragma once

//#define IMAGE_SCALE_640_480
#define IMAGE_SCALE_320_240

#pragma region Basic_Define
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
#pragma endregion

#define ptile_Binary     0 
#define iterative_Binary 1
#define Otsu_Binary      2

#define GaussianFilitering 0
//#define GaussianFilitering 1

#pragma region Basic_Array_Of_Color
BYTE mRGB[height][width][3] = { 0 };
BYTE mHSV[height][width][3] = { 0 };
BYTE mGRAY[height][width] = { 0 };

BYTE Histogram[color_scale] = { 0 };
float pHistogram[color_scale] = { 0 };
UINT sHistogram[color_scale] = { 0 };
#pragma endregion

float aHistogram[color_scale] = { 0 };

class __color
{
public:
#pragma region ChangeColor&ColorIO
	static void RGBtoGRAY(BYTE(*mRGB)[width][3], BYTE(*mGRAY)[width], int i, int j);
	static void GRAYtoRGB(BYTE (*mRGB)[width][3], BYTE (*mGRAY)[width], int i, int j);
	static void RGBtoHSV(BYTE(*mRGB)[width][3], BYTE(*mHSV)[width][3], int i, int j);
	static void HSVtoRGB(BYTE(*mRGB)[width][3], BYTE(*mHSV)[width][3], int i, int j);
	static void Input(LPVIDEOHDR lpVHdr, LPBYTE Histogram);
	static void Output(LPVIDEOHDR lpVHdr, LPBYTE Histogram);
#pragma endregion
	static void Filitering(BYTE(*mGRAY)[width], int value);
	static void Equalization();
};

#pragma region DONT_TOUCH
void __color::RGBtoGRAY(BYTE (*mRGB)[width][3], BYTE (*mGRAY)[width], int i, int j)
{
	mGRAY[i][j] = (mRGB[i][j][RED] + mRGB[i][j][GREEN] + mRGB[i][j][BLUE]) / 3;
	Histogram[mGRAY[i][j]]++;
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
void __color::Input(LPVIDEOHDR lpVHdr, LPBYTE Histogram)
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
			__color::RGBtoHSV(mRGB, mHSV, i / 2, j / 2);
			Histogram[mGRAY[i/2][j/2]]++;
		}
	}
#endif
	sHistogram[0] = Histogram[0];
	for (i = 0; i < color_scale; i++)
	{
		pHistogram[i] = (float)Histogram[i] / (height * width);
		if (i != 0)sHistogram[i] = sHistogram[i - 1] + Histogram[i];
	}

	return;
}
#pragma endregion

#pragma region Output
void __color::Output(LPVIDEOHDR lpVHdr, LPBYTE Histogram)
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
				if (j <= width + 256)
				{
					if (Histogram[j - width] <= i * 1.5)
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
				}
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
	short i, j;
#if GaussianFilitering 0
	const BYTE Gaussian[5][5] =
	{ {1,4,7,4,1},
	{4,16,26,16,4},
	{7,26,41,26,7},
	{4,16,26,16,4},
	{1,4,7,4,1 } };//273

	const BYTE GaussianDivide = 273;

	for (i = 0; i < height; i++)
	{
		sGRAY[i] = (BYTE*)malloc(sizeof(BYTE)*width);
		memcpy(sGRAY[i], mGRAY[i], sizeof(BYTE)*width);
	}
	for (i = 2; i < height - 2; i++)
	{
		for (j = 2; j < width - 2; j++)
		{
			mGRAY[i][j] = (Gaussian[0][0] * sGRAY[i - 2][j - 2] + Gaussian[0][1] * sGRAY[i - 2][j - 1] + Gaussian[0][2] * sGRAY[i - 2][j] + Gaussian[0][3] * sGRAY[i - 2][j + 1] + Gaussian[0][4] * sGRAY[i - 2][j + 2] +
				Gaussian[1][0] * sGRAY[i - 1][j - 2] + Gaussian[1][1] * sGRAY[i - 1][j - 1] + Gaussian[1][2] * sGRAY[i - 1][j] + Gaussian[1][3] * sGRAY[i - 1][j + 1] + Gaussian[1][4] * sGRAY[i - 1][j + 2] +
				Gaussian[2][0] * sGRAY[i][j - 2] + Gaussian[2][1] * sGRAY[i][j - 1] + Gaussian[2][2] * sGRAY[i][j] + Gaussian[2][3] * sGRAY[i][j + 1] + Gaussian[2][4] * sGRAY[i][j + 2] +
				Gaussian[3][0] * sGRAY[i + 1][j - 2] + Gaussian[3][1] * sGRAY[i + 1][j - 1] + Gaussian[3][2] * sGRAY[i + 1][j] + Gaussian[3][3] * sGRAY[i + 1][j + 1] + Gaussian[3][4] * sGRAY[i + 1][j + 2] +
				Gaussian[4][0] * sGRAY[i + 2][j - 2] + Gaussian[4][1] * sGRAY[i + 2][j - 1] + Gaussian[4][2] * sGRAY[i + 2][j] + Gaussian[4][3] * sGRAY[i + 2][j + 1] + Gaussian[4][4] * sGRAY[i + 2][j + 2]) / GaussianDivide;
		}
	}
#else
	const BYTE Gaussian[5][5] =
	{ { 2,4,5,4,2 },
	{ 4,9,12,9,4 },
	{ 5,12,15,12,5 },
	{ 4,9,12,9,4 },
	{ 2,4,5,4,2 } };//159

	const BYTE GaussianDivide = 159;

	for (i = 0; i < height; i++)
	{
		sGRAY[i] = (BYTE*)malloc(sizeof(BYTE)*width);
		memcpy(sGRAY[i], mGRAY[i], sizeof(BYTE)*width);
	}
	for (i = 2; i < height - 2; i++)
	{
		for (j = 2; j < width - 2; j++)
		{
			mGRAY[i][j] = (Gaussian[0][0] * sGRAY[i - 2][j - 2] + Gaussian[0][1] * sGRAY[i - 2][j - 1] + Gaussian[0][2] * sGRAY[i - 2][j] + Gaussian[0][3] * sGRAY[i - 2][j + 1] + Gaussian[0][4] * sGRAY[i - 2][j + 2] +
				Gaussian[1][0] * sGRAY[i - 1][j - 2] + Gaussian[1][1] * sGRAY[i - 1][j - 1] + Gaussian[1][2] * sGRAY[i - 1][j] + Gaussian[1][3] * sGRAY[i - 1][j + 1] + Gaussian[1][4] * sGRAY[i - 1][j + 2] +
				Gaussian[2][0] * sGRAY[i][j - 2] + Gaussian[2][1] * sGRAY[i][j - 1] + Gaussian[2][2] * sGRAY[i][j] + Gaussian[2][3] * sGRAY[i][j + 1] + Gaussian[2][4] * sGRAY[i][j + 2] +
				Gaussian[3][0] * sGRAY[i + 1][j - 2] + Gaussian[3][1] * sGRAY[i + 1][j - 1] + Gaussian[3][2] * sGRAY[i + 1][j] + Gaussian[3][3] * sGRAY[i + 1][j + 1] + Gaussian[3][4] * sGRAY[i + 1][j + 2] +
				Gaussian[4][0] * sGRAY[i + 2][j - 2] + Gaussian[4][1] * sGRAY[i + 2][j - 1] + Gaussian[4][2] * sGRAY[i + 2][j] + Gaussian[4][3] * sGRAY[i + 2][j + 1] + Gaussian[4][4] * sGRAY[i + 2][j + 2]) / GaussianDivide;
		}
	}
#endif

	for (i = 0; i < height; i++)
	{
		free(sGRAY[i]);
	}
	free(sGRAY);
	return;
}
#pragma endregion



/*
1. Trans Color
2. V streching
3. H에서 순수 R, G, B만 추출
 - R : 
 - G : 
 - B : 

4. 색상 분할(each case)
*/

	
void __color::Equalization()
{
	short i, j;
	/*
	sHistogram[0] = Histogram[0];
	for (i = 0; i < color_scale; i++)
	{
		if (i != 0)sHistogram[i] = sHistogram[i - 1] + Histogram[i];
		pHistogram[i] = (float)sHistogram[i] / (height * width);
	}

	//평활화
	memset(aHistogram, 0, sizeof(float)*color_scale);
	//printf("%6.6f\n", (float)sHistogram[255] / (height * width));
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			mHSV[i][j][V] = (float)pHistogram[mGRAY[i][j]] * 255.0 * (2.0 - sHistogram[255] / (height * width));
			mGRAY[i][j] = mHSV[i][j][V];
			aHistogram[mGRAY[i][j]]++;
		}
	}
	*/
	BYTE max = 0, min = 255;

	for (i = 0; i < color_scale; i++)
	{
		if (Histogram[i] != 0)
		{
			if (max <= i)max = i;
			if (min >= i)min = i;
		}
	}

	printf("max : %.5f,  min : %.5f\n", ((255 - max) / 255.0 + 1.0, min, (1.0 - (min - 0) / 255.0)));
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if ((max + min) / 2 < mHSV[i][j][V])
			{
				mHSV[i][j][V] = mHSV[i][j][V] * ((255 - max) / 255.0 + 1.0);
			}
			else
			{
				mHSV[i][j][V] = mHSV[i][j][V] * (1.0 - ((min==0?1:min) - 0) / 255.0);
			}
			__color::HSVtoRGB(mRGB, mHSV, i, j);
		}
	}
	return;
}

void Deleting()
{
	short i, j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{

		}
	}
	return;
}
/*
#define coor(x,y) ((x) * width + (j))
#define rangeCheck(i,j) (i-1>=0&&i<=height&&j-1>=0&&j<=width)

#include<stack>

using namespace std;

struct xy
{
	short i;
	short j;
};

void Labeling()
{
	BYTE *LRED = (BYTE*)malloc(sizeof(BYTE)*(height * width));
	BYTE *LGREEN = (BYTE*)malloc(sizeof(BYTE)*(height * width));
	BYTE *LBLUE = (BYTE*)malloc(sizeof(BYTE)*(height * width));

	stack <xy> St;

	short i, j;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (LRED[coor(i, j)] != 0)
			{
				LRED[coor(i, j)] = 0;
				if (LRED[coor(i-1, j-1)] != 0)
				{
					St.push({ i-1,j-1 });
				}
				if (LRED[coor(i - 1, j)] != 0)
				{
					St.push({ i - 1,j });
				}
				if (LRED[coor(i - 1, j + 1)] != 0)
				{
					St.push({ i - 1,j + 1 });
				}

				if (LRED[coor(i, j - 1)] != 0)
				{
					St.push({ i,j - 1 });
				}

				if (LRED[coor(i, j + 1)] != 0)
				{
					St.push({ i,j + 1 });
				}

				if (LRED[coor(i + 1, j - 1)] != 0)
				{
					St.push({ i + 1,j - 1 });
				}
				if (LRED[coor(i + 1, j)] != 0)
				{
					St.push({ i + 1,j });
				}
				if (LRED[coor(i + 1, j + 1)] != 0)
				{
					St.push({ i + 1,j + 1 });
				}

			}
			else {
				if (!St.empty()) {
					if(LRED[coor(St.top().i,St.top().j)])
				}
			}
		}
	}
	

	free(LRED);
	free(LGREEN);
	free(LBLUE);
	return;
}*/

void ColorGaussian()
{
	const BYTE Gaussian[5][5] =
	{ { 2,4,5,4,2 },
	{ 4,9,12,9,4 },
	{ 5,12,15,12,5 },
	{ 4,9,12,9,4 },
	{ 2,4,5,4,2 } };//159

	const BYTE GaussianDivide = 159;
	short i, j;

	BYTE *mRED = (BYTE*)malloc(sizeof(BYTE)*height * width);
	BYTE *mGREEN = (BYTE*)malloc(sizeof(BYTE)*height * width);
	BYTE *mBLUE = (BYTE*)malloc(sizeof(BYTE)*height * width);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			mRED[i * width + j] = mRGB[i][j][RED];
			mGREEN[i * width + j] = mRGB[i][j][GREEN];
			mBLUE[i * width + j] = mRGB[i][j][BLUE];
		}
	}
	
	for (i = 2; i < height - 2; i++)
	{
		for (j = 2; j < width - 2; j++)
		{
			mRGB[i][j][RED] = (Gaussian[0][0] * mRED[(i - 2) * width + (j - 2)] + Gaussian[0][1] * mRED[(i - 2) * width + (j - 1)] + Gaussian[0][2] * mRED[(i - 2) * width + (j)] + Gaussian[0][3] * mRED[(i - 2) * width + (j + 1)] + Gaussian[0][4] * mRED[(i - 2) * width + (j + 2)] +
				Gaussian[1][0] * mRED[(i - 1) * width + (j - 2)] + Gaussian[1][1] * mRED[(i - 1) * width + (j - 1)] + Gaussian[1][2] * mRED[(i - 1) * width + (j)] + Gaussian[1][3] * mRED[(i - 1) * width + (j + 1)] + Gaussian[1][4] * mRED[(i - 1) * width + (j + 2)] +
				Gaussian[2][0] * mRED[(i) * width + (j - 2)] + Gaussian[2][1] * mRED[(i) * width + (j - 1)] + Gaussian[2][2] * mRED[(i) * width + (j)] + Gaussian[2][3] * mRED[(i) * width + (j + 1)] + Gaussian[2][4] * mRED[(i) * width + (j + 2)] +
				Gaussian[3][0] * mRED[(i + 1) * width + (j - 2)] + Gaussian[3][1] * mRED[(i + 1) * width + (j - 1)] + Gaussian[3][2] * mRED[(i + 1) * width + (j)] + Gaussian[3][3] * mRED[(i + 1) * width + (j + 1)] + Gaussian[3][4] * mRED[(i + 1) * width + (j + 2)] +
				Gaussian[4][0] * mRED[(i + 2) * width + (j - 2)] + Gaussian[4][1] * mRED[(i + 2) * width + (j - 1)] + Gaussian[4][2] * mRED[(i + 2) * width + (j)] + Gaussian[4][3] * mRED[(i + 2) * width + (j + 1)] + Gaussian[4][4] * mRED[(i + 2) * width + (j + 2)]) / GaussianDivide;

			mRGB[i][j][GREEN] = (Gaussian[0][0] * mGREEN[(i - 2) * width + (j - 2)] + Gaussian[0][1] * mGREEN[(i - 2) * width + (j - 1)] + Gaussian[0][2] * mGREEN[(i - 2) * width + (j)] + Gaussian[0][3] * mGREEN[(i - 2) * width + (j + 1)] + Gaussian[0][4] * mGREEN[(i - 2) * width + (j + 2)] +
				Gaussian[1][0] * mGREEN[(i - 1) * width + (j - 2)] + Gaussian[1][1] * mGREEN[(i - 1) * width + (j - 1)] + Gaussian[1][2] * mGREEN[(i - 1) * width + (j)] + Gaussian[1][3] * mGREEN[(i - 1) * width + (j + 1)] + Gaussian[1][4] * mGREEN[(i - 1) * width + (j + 2)] +
				Gaussian[2][0] * mGREEN[(i)* width + (j - 2)] + Gaussian[2][1] * mGREEN[(i)* width + (j - 1)] + Gaussian[2][2] * mGREEN[(i)* width + (j)] + Gaussian[2][3] * mGREEN[(i)* width + (j + 1)] + Gaussian[2][4] * mGREEN[(i)* width + (j + 2)] +
				Gaussian[3][0] * mGREEN[(i + 1) * width + (j - 2)] + Gaussian[3][1] * mGREEN[(i + 1) * width + (j - 1)] + Gaussian[3][2] * mGREEN[(i + 1) * width + (j)] + Gaussian[3][3] * mGREEN[(i + 1) * width + (j + 1)] + Gaussian[3][4] * mGREEN[(i + 1) * width + (j + 2)] +
				Gaussian[4][0] * mGREEN[(i + 2) * width + (j - 2)] + Gaussian[4][1] * mGREEN[(i + 2) * width + (j - 1)] + Gaussian[4][2] * mGREEN[(i + 2) * width + (j)] + Gaussian[4][3] * mGREEN[(i + 2) * width + (j + 1)] + Gaussian[4][4] * mGREEN[(i + 2) * width + (j + 2)]) / GaussianDivide;

			mRGB[i][j][BLUE] = (Gaussian[0][0] * mBLUE[(i - 2) * width + (j - 2)] + Gaussian[0][1] * mBLUE[(i - 2) * width + (j - 1)] + Gaussian[0][2] * mBLUE[(i - 2) * width + (j)] + Gaussian[0][3] * mBLUE[(i - 2) * width + (j + 1)] + Gaussian[0][4] * mBLUE[(i - 2) * width + (j + 2)] +
				Gaussian[1][0] * mBLUE[(i - 1) * width + (j - 2)] + Gaussian[1][1] * mBLUE[(i - 1) * width + (j - 1)] + Gaussian[1][2] * mBLUE[(i - 1) * width + (j)] + Gaussian[1][3] * mBLUE[(i - 1) * width + (j + 1)] + Gaussian[1][4] * mBLUE[(i - 1) * width + (j + 2)] +
				Gaussian[2][0] * mBLUE[(i)* width + (j - 2)] + Gaussian[2][1] * mBLUE[(i)* width + (j - 1)] + Gaussian[2][2] * mBLUE[(i)* width + (j)] + Gaussian[2][3] * mBLUE[(i)* width + (j + 1)] + Gaussian[2][4] * mBLUE[(i)* width + (j + 2)] +
				Gaussian[3][0] * mBLUE[(i + 1) * width + (j - 2)] + Gaussian[3][1] * mBLUE[(i + 1) * width + (j - 1)] + Gaussian[3][2] * mBLUE[(i + 1) * width + (j)] + Gaussian[3][3] * mBLUE[(i + 1) * width + (j + 1)] + Gaussian[3][4] * mBLUE[(i + 1) * width + (j + 2)] +
				Gaussian[4][0] * mBLUE[(i + 2) * width + (j - 2)] + Gaussian[4][1] * mBLUE[(i + 2) * width + (j - 1)] + Gaussian[4][2] * mBLUE[(i + 2) * width + (j)] + Gaussian[4][3] * mBLUE[(i + 2) * width + (j + 1)] + Gaussian[4][4] * mBLUE[(i + 2) * width + (j + 2)]) / GaussianDivide;


		}
	}

	free(mRED);
	free(mGREEN);
	free(mBLUE);

	return;
}

void Colorero()
{
	BYTE *mRED = (BYTE*)malloc(sizeof(BYTE) * height * width);
	BYTE *mGREEN = (BYTE*)malloc(sizeof(BYTE) * height * width);
	BYTE *mBLUE = (BYTE*)malloc(sizeof(BYTE) * height * width);

	short i, j;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			mRED[i * width + j] = mRGB[i][j][RED];
			mGREEN[i * width + j] = mRGB[i][j][GREEN];
			mBLUE[i * width + j] = mRGB[i][j][BLUE];
		}
	}

	for (i = 1; i < height - 1; i++)
	{
		for (j = 1; j < width - 1; j++)
		{
			for (int n = -1; n <= 1; n++)
			{
				for (int m = -1; m <= 1; m++)
				{
					if (mRED[(i + n) * width + (j + m)] != 0)
					{
						mRGB[i][j][RED] = 0;
					}
					if (mGREEN[(i + n) * width + (j + m)] != 0)
					{
						mRGB[i][j][GREEN] = 255;
					}
					if (mBLUE[(i + n) * width + (j + m)] != 0)
					{
						mRGB[i][j][BLUE] = 0;
					}

				}
			}
		}
	}

	free(mRED);
	free(mGREEN);
	free(mBLUE);
	return;
}