// AGifExport.h

#pragma once
#include "output.h"
#using "AForge.dll"
#using "AForge.Imaging.dll"
#using "AForge.Imaging.Formats.dll"


using namespace System;
//using namespace System::Drawing;

namespace AGifExport {





	/*
	public ref class Class1
	{
		// TODO: Add your methods for this class here.
	};*/
}

public enum class Dither_Method
{
	Burkes,
	Floyd,
	JNN,
	Ordered,
	Sierra,
	Stucki,
	FloydIM,
	None,
	imchecker,
	imo22,
	imo33,
	imo88,
	imh44,
	imh66,
	imh88,
	imh44o,
	imh66o,
	imh88o,
	imc55b,
	imc55w,
	imc77b,
	imc77w
};

typedef struct CONFIG {
	Boolean droplocal;
	Boolean adaptiveinsert;
	Int32   fps;
	Int32   loop;
	Int32   color;
	Int32   fuzz;
	Dither_Method method;
} CONFIG;

void ShowfrmConfig(CONFIG *data);

//get a dib pointer from AviUtl and return a dithered bitmap
System::Drawing::Bitmap^ ditherFrame(void *dib, int width, int height, CONFIG* settings);


