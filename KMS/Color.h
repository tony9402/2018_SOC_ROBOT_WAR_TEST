#pragma once

#pragma region def
#define height 480
#define width 640
#define sizeof_Histogram 256
#define max(x, y, z) ((x) > (y) ? (x) : (y)) > (z) ? ((x) > (y) ? (x) : (y)) : (z)
#define min(x, y, z) ((x) > (y) ? (y) : (x)) > (z) ? (z) : ((x) > (y) ? (y) : (x))
#pragma endregion

#pragma region def_of_type
typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef unsigned short WORD;
typedef unsigned long DWORD;
#pragma endregion

#pragma region def_of_struct
typedef struct _rgb_ {
	BYTE blue[height][width];
	BYTE green[height][width];
	BYTE red[height][width];
}cRGB;

typedef struct _hsv_ {
	BYTE V[height][width];
	BYTE S[height][width];
	BYTE H[height][width];
}cHSV;

typedef struct _YCbCr_ {
	BYTE Y[height][width];
	BYTE Cb[height][width];
	BYTE Cr[height][width];
}cYUV;

typedef struct _CMY_ {
	BYTE C[height][width];
	BYTE M[height][width];
	BYTE Y[height][width];
}CMY;

typedef struct _gray_ {
	BYTE gray[height][width];
	BYTE histogram[sizeof_Histogram] = { 0 };
}cGRAY;
#pragma endregion

#pragma region class_of_changeColor
class change_color_ij
{
public:
	void CMYtoRGB(cRGB *rgb, CMY *cmy, short i, short j);
	void RGBtoGRAY(cRGB *rgb, cGRAY *gray, short i, short j);
	void RGBtoHSV(cRGB *rgb, cHSV *hsv, short i, short j);
	void RGBtoYUV(cRGB *rgb, cYUV *yuv, short i, short j);
	void GRAYtoRGB(cRGB *rgb, cGRAY *gray, short i, short j);
	void HSVtoRGB(cRGB *rgb, cHSV *hsv, short i, short j);
	void YUVtoRGB(cRGB *rgb, cYUV *yuv, short i, short j);
	void RGBtoCMY(cRGB *rgb, CMY *cmy, short i, short j);
};
#pragma endregion

#pragma region color_change_funtions
void change_color_ij::RGBtoGRAY(cRGB *rgb, cGRAY *gray, short i, short j)
{
	gray->gray[i][j] = 0.299 * (double)rgb->red[i][j] + 0.587 * (double)rgb->green[i][j] + 0.114 * (double)rgb->blue[i][j];
	gray->histogram[gray->gray[i][j]]++;
	return;
}

void change_color_ij::RGBtoHSV(cRGB *rgb, cHSV *hsv, short i, short j)
{
	BYTE cmax, cmin;
	cmax = max(rgb->blue[i][j], rgb->green[i][j], rgb->red[i][j]);
	cmin = min(rgb->blue[i][j], rgb->green[i][j], rgb->red[i][j]);

	hsv->V[i][j] = cmax;
	if (!hsv->V[i][j])
	{
		hsv->H[i][j] = 0;
		hsv->S[i][j] = 0;
		return;
	}

	hsv->S[i][j] = 255 * (long)(cmax - cmin) / hsv->V[i][j];
	if (!hsv->S[i][j])
	{
		hsv->H[i][j] = 0;
		return;
	}

	if (cmax == rgb->red[i][j])
	{
		hsv->H[i][j] = 0 + 43 * (rgb->green[i][j] - rgb->blue[i][j]) / (cmax - cmin);
	}
	else if (cmax == rgb->green[i][j])
	{
		hsv->H[i][j] = 85 + 43 * (rgb->blue[i][j] - rgb->red[i][j]) / (cmax - cmin);
	}
	else {
		hsv->H[i][j] = 171 + 43 * (rgb->red[i][j] - rgb->green[i][j]) / (cmax - cmin);
	}

	return;
}

void change_color_ij::RGBtoYUV(cRGB *rgb, cYUV *yuv, short i, short j)
{
	yuv->Y[i][j] = 0.299 * (double)rgb->red[i][j] + 0.587 * (double)rgb->green[i][j] + 0.114 * (double)rgb->blue[i][j];
	yuv->Cb[i][j] = -0.169 * (double)rgb->red[i][j] - 0.331 * (double)rgb->green[i][j] + 0.5 * (double)rgb->blue[i][j];
	yuv->Cr[i][j] = 0.5 * (double)rgb->red[i][j] - 0.419 *(double)rgb->green[i][j] - 0.0813 * (double)rgb->blue[i][j];
	return;
}

void change_color_ij::RGBtoCMY(cRGB *rgb, CMY *cmy, short i, short j)
{
	cmy->C[i][j] = 255 - rgb->red[i][j];
	cmy->M[i][j] = 255 - rgb->green[i][j];
	cmy->Y[i][j] = 255 - rgb->blue[i][j];
	return;
}

void change_color_ij::GRAYtoRGB(cRGB *rgb, cGRAY *gray, short i, short j)
{
	rgb->red[i][j] = rgb->blue[i][j] = rgb->green[i][j] = gray->gray[i][j];
	return;
}

void change_color_ij::HSVtoRGB(cRGB *rgb, cHSV *hsv, short i, short j)
{
	BYTE region, remainder, p, q, t;
	if (!hsv->S[i][j])
	{
		rgb->red[i][j] = rgb->green[i][j] = rgb->blue[i][j] = hsv->V[i][j];
		return;
	}
	region = hsv->H[i][j] / 43;
	remainder = (hsv->H[i][j] - (region * 43)) * 6;

	p = (hsv->V[i][j] * (255 - hsv->S[i][j])) >> 8;
	q = (hsv->V[i][j] * (255 - ((hsv->S[i][j] * remainder) >> 8))) >> 8;
	t = (hsv->V[i][j] * (255 - ((hsv->S[i][j] * (255 - remainder)) >> 8))) >> 8;

	switch (region)
	{
	case 0:
		rgb->red[i][j] = hsv->V[i][j];
		rgb->green[i][j] = t;
		rgb->blue[i][j] = p;
		break;
	case 1:
		rgb->red[i][j] = q;
		rgb->green[i][j] = hsv->V[i][j];
		rgb->blue[i][j] = p;
		break;
	case 2:
		rgb->red[i][j] = p;
		rgb->green[i][j] = hsv->V[i][j];
		rgb->blue[i][j] = t;
		break;
	case 3:
		rgb->red[i][j] = p;
		rgb->green[i][j] = q;
		rgb->blue[i][j] = hsv->V[i][j];
		break;
	case 4:
		rgb->red[i][j] = t;
		rgb->green[i][j] = p;
		rgb->blue[i][j] = hsv->V[i][j];
		break;
	default:
		rgb->red[i][j] = hsv->V[i][j];
		rgb->green[i][j] = p;
		rgb->blue[i][j] = q;
		break;
	}
	return;
}

void change_color_ij::YUVtoRGB(cRGB *rgb, cYUV *yuv, short i, short j)
{
	rgb->red[i][j] = 1 * yuv->Y[i][j] + 1.402 * yuv->Cr[i][j] + 0.000 * yuv->Cb[i][j];
	rgb->green[i][j] = 1 * yuv->Y[i][j] - 0.714 * yuv->Cr[i][j] - 0.344 * yuv->Cb[i][j];
	rgb->blue[i][j] = 1 * yuv->Y[i][j] + 0.000 * yuv->Cr[i][j] + 1.772 * yuv->Cb[i][j];
	return;
}

void change_color_ij::CMYtoRGB(cRGB *rgb, CMY *cmy, short i, short j)
{
	rgb->red[i][j] = 255 - cmy->C[i][j];
	rgb->green[i][j] = 255 - cmy->M[i][j];
	rgb->blue[i][j] = 255 - cmy->Y[i][j];
	return;
}
#pragma endregion