
if (Area_red >= 2000 && Area_yellow >= 1300 && Area_blue >= 500)
{
	printf("포카칩 발견\n");

}
else if (Area_blue >= 2000 && !(Area_yellow >= 800) && !(Area_red >= 800))
{
	printf("포카리 발견\n");
	for (i = -5; i <= 5; i++)
	{
		mRGB[Bluexy_i + i][Bluexy_j][GREEN] = 255;
		mRGB[Bluexy_i][Bluexy_j + i][GREEN] = 255;
		mRGB[Bluexy_i + i][Bluexy_j][BLUE] = 0;
		mRGB[Bluexy_i][Bluexy_j + i][BLUE] = 0;
		mRGB[Bluexy_i + i][Bluexy_j][RED] = 0;
		mRGB[Bluexy_i][Bluexy_j + i][RED] = 0;
	}
	_blue.Maxi = _blue.Maxi > height ? height : _blue.Maxi;
	_blue.Maxj = _blue.Maxj > width ? width : _blue.Maxj;
	_blue.Mini = _blue.Mini < 0 ? 0 : _blue.Mini;
	_blue.Minj = _blue.Minj < 0 ? 0 : _blue.Minj;

	for (i = _blue.Mini; i <= _blue.Maxi; i++)
	{
		mRGB[i][_blue.Maxj][GREEN] = 0;
		mRGB[i][_blue.Maxj][RED] = 0;
		mRGB[i][_blue.Maxj][BLUE] = 255;
		mRGB[i][_blue.Minj][GREEN] = 0;
		mRGB[i][_blue.Minj][RED] = 0;
		mRGB[i][_blue.Minj][BLUE] = 255;
	}

	for (j = _blue.Minj; j <= _blue.Maxj; j++)
	{
		mRGB[_blue.Maxi][j][GREEN] = 255;
		mRGB[_blue.Maxi][j][RED] = 0;
		mRGB[_blue.Maxi][j][BLUE] = 0;
		mRGB[_blue.Mini][j][GREEN] = 255;
		mRGB[_blue.Mini][j][RED] = 0;
		mRGB[_blue.Mini][j][BLUE] = 0;
	}
}
else if (Area_red >= 800 && !(Area_blue >= 1250) && !(Area_yellow >= 1300))
{
	printf("칸타타 발견\n");

	for (i = -5; i <= 5; i++)
	{
		mRGB[Redxy_i + i][Redxy_j][GREEN] = 0;
		mRGB[Redxy_i][Redxy_j + i][GREEN] = 0;
		mRGB[Redxy_i + i][Redxy_j][BLUE] = 0;
		mRGB[Redxy_i][Redxy_j + i][BLUE] = 0;
		mRGB[Redxy_i + i][Redxy_j][RED] = 255;
		mRGB[Redxy_i][Redxy_j + i][RED] = 255;
	}
	_red.Maxi = _red.Maxi > height ? height : _red.Maxi;
	_red.Maxj = _red.Maxj > width ? width : _red.Maxj;
	_red.Mini = _red.Mini < 0 ? 0 : _red.Mini;
	_red.Minj = _red.Minj < 0 ? 0 : _red.Minj;

	for (i = _red.Mini; i <= _red.Maxi; i++)
	{
		mRGB[i][_red.Maxj][GREEN] = 0;
		mRGB[i][_red.Maxj][RED] = 255;
		mRGB[i][_red.Maxj][BLUE] = 0;
		mRGB[i][_red.Minj][GREEN] = 0;
		mRGB[i][_red.Minj][RED] = 255;
		mRGB[i][_red.Minj][BLUE] = 0;
	}

	for (j = _red.Minj; j <= _red.Maxj; j++)
	{
		mRGB[_red.Maxi][j][GREEN] = 255;
		mRGB[_red.Maxi][j][RED] = 0;
		mRGB[_red.Maxi][j][BLUE] = 0;
		mRGB[_red.Mini][j][GREEN] = 255;
		mRGB[_red.Mini][j][RED] = 0;
		mRGB[_red.Mini][j][BLUE] = 0;
	}
}
else
{
	printf("Seraching\n");
}
*/