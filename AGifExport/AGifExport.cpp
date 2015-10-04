// This is the main DLL file.
#pragma warning( push )
#pragma warning( disable: 4100 )

#include "stdafx.h"
#include <stdlib.h>
#include "AGifExport.h"
#include "frmConfig.h"
#include "wand/MagickWand.h"
#include "magick/MagickCore.h"
#include <vector>
//#include <array>
using namespace AGifExport;

//using namespace System::Drawing;
//using namespace System::Drawing::Imaging;
/*
using namespace AForge::Imaging;
using namespace AForge::Imaging::Filters;
using namespace AForge::Imaging::ColorReduction;
*/

//---------------------------------------------------------------------
//		出力プラグイン構造体定義
//---------------------------------------------------------------------
OUTPUT_PLUGIN_TABLE output_plugin_table = {
	NULL,												//	フラグ
	"Animated GIF Export",										//	プラグインの名前
	"GifFile (*.gif)\0*.gif\0AllFile (*.*)\0*.*\0",		//	出力ファイルのフィルタ
	"Animated Gif Export 0.5beta by Maverick Tse",			//	プラグインの情報
	NULL,												//	DLL開始時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL,												//	DLL終了時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	func_output,										//	出力時に呼ばれる関数へのポインタ
	func_config,										//	出力設定のダイアログを要求された時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	func_config_get,									//	出力設定データを取得する時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	func_config_set,									//	出力設定データを設定する時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
};

//---------------------------------------------------------------------
//		出力プラグイン構造体のポインタを渡す関数
//---------------------------------------------------------------------
EXTERN_C OUTPUT_PLUGIN_TABLE __declspec(dllexport) * __stdcall GetOutputPluginTable(void)
{
	return &output_plugin_table;
}

//---------------------------------------------------------------------
//		Setting data
//---------------------------------------------------------------------

static CONFIG config = { true, false, false, 2, 0, 128, 3, 0, 0, 0, 0, Dither_Method::None }; //defaults

/*---------------------------------------------------------------------
//		Internal functions for export plugin
//---------------------------------------------------------------------
//
//	oip->flag;				// フラグ
//							// OUTPUT_INFO_FLAG_VIDEO	: 画像データあり
//							// OUTPUT_INFO_FLAG_AUDIO	: 音声データあり
//	oip->w,oip->h;			// 縦横サイズ
//	oip->rate,oip->scale;	// フレームレート
//	oip->n;					// フレーム数
//	oip->size;				// １フレームのバイト数
//	oip->audio_rate;		// 音声サンプリングレート
//	oip->audio_ch;			// 音声チャンネル数
//	oip->audio_n;			// 音声サンプリング数
//	oip->audio_size;		// 音声１サンプルのバイト数
//	oip->savefile;			// セーブファイル名へのポインタ
//
//	void *oip->func_get_video( int frame );
//							// DIB形式(RGB24bit)の画像データへのポインタを取得します。
//							// frame	: フレーム番号
//							// 戻り値	: データへのポインタ
//	void *oip->func_get_audio( int start,int length,int *readed );
//							// 16bitPCM形式の音声データへのポインタを取得します。
//							// start	: 開始サンプル番号
//							// length	: 読み込むサンプル数
//							// readed	: 読み込まれたサンプル数
//							// 戻り値	: データへのポインタ
//	BOOL oip->func_is_abort( void );
//							// 中断するか調べます。
//							// 戻り値	: TRUEなら中断
//	BOOL oip->func_rest_time_disp( int now,int total );
//							// 残り時間を表示させます。
//							// now		: 処理しているフレーム番号
//							// total	: 処理する総フレーム数
//							// 戻り値	: TRUEなら成功
//	int oip->func_get_flag( int frame );
//							//	フラグを取得します。
//							//	frame	: フレーム番号
//							//	戻り値	: フラグ
//							//  OUTPUT_INFO_FRAME_FLAG_KEYFRAME		: キーフレーム推奨
//							//  OUTPUT_INFO_FRAME_FLAG_COPYFRAME	: コピーフレーム推奨
//	BOOL oip->func_update_preview( void );
//							//	プレビュー画面を更新します。
//							//	最後にfunc_get_videoで読み込まれたフレームが表示されます。
//							//	戻り値	: TRUEなら成功
//*/

BOOL func_output(OUTPUT_INFO *oip)
{
	
	//Determine frame# to process
	std::vector<Int32> frame_to_encode;
	std::vector<Int32> delay;
	int base_delay = round(100 * oip->scale / oip->rate);
	frame_to_encode.push_back(0);
	if (config.fps >1)
	{
		for (int i = 1; i < oip->n-1; ++i)
		{
			if (!(i%config.fps))
			{
				frame_to_encode.push_back(i);
			}
		}
		if (oip->n > 1)
		{
			frame_to_encode.push_back(oip->n - 1); //always add the last frame
		}
		for (int i = 0; i < frame_to_encode.size()-1; ++i)
		{
			int fdelay = round((frame_to_encode.at(i + 1) - frame_to_encode.at(i))*base_delay);
			delay.push_back(fdelay);
		}
		if (oip->n > 1)
		{
			delay.push_back(base_delay); //standard delay for last frame
		}
	}
	else
	{
		//No drop
		for (int i = 1; i < oip->n; ++i)
		{
			frame_to_encode.push_back(i);
			delay.push_back(base_delay);
		}
		delay.push_back(base_delay);
	}
	
	
	//Prepare magic wand
	MagickWandGenesis();
	MagickWand *dest = NewMagickWand();
	//MagickWand *prev = NewMagickWand();
	TCHAR digit[4];
	_itoa_s(config.loop, digit, sizeof(digit), 10);
	//Loop through vector and process each frame
	for (int i = 0; i < frame_to_encode.size(); ++i)
	{
		if (!oip->func_is_abort()){
			oip->func_rest_time_disp(i, frame_to_encode.size()); //progress display
			//
			void* raw_image = oip->func_get_video(frame_to_encode.at(i));
			byte* trimmed_image = nullptr;
			Bitmap ^buffer = nullptr;
			if ((oip->w % 8) == 0)
			{
				buffer = ditherFrame(raw_image, oip->w, oip->h, &config);
			}
			else
			{
				//int w8 = (oip->w /8) *8;
				int w8 = oip->w;
				size_t pixel_count = w8* oip->h * 3;
				size_t line_bytes = w8 * 3;
				size_t src_line_bytes = oip->size / oip->h;
				trimmed_image = new byte[pixel_count];
				SecureZeroMemory(trimmed_image, line_bytes*oip->h);

				byte* src_addr = (byte*)raw_image;
				byte* dest_addr = trimmed_image;
				for (int j = 0; j < oip->h; ++j)
				{
					_CRT_SECURE_MEMCPY(dest_addr, line_bytes, src_addr, line_bytes);
					src_addr += src_line_bytes;
					dest_addr += line_bytes;
				}
				buffer = ditherFrame(trimmed_image, w8, oip->h, &config);
			}
			
			Bitmap ^stdimg = AForge::Imaging::Image::Clone(buffer, Imaging::PixelFormat::Format24bppRgb);
			if (trimmed_image) delete[] trimmed_image;
			//Lock stdimg
			System::Drawing::Rectangle rect = System::Drawing::Rectangle(0, 0, stdimg->Width, stdimg->Height);
			System::Drawing::Imaging::BitmapData ^bmpData = stdimg->LockBits(rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, Imaging::PixelFormat::Format24bppRgb);
			IntPtr rawimg = bmpData->Scan0;
			
			MagickWand *source = NewMagickWand();
			byte* trimmedDIB = nullptr;
			int stride = bmpData->Stride;
			if ((bmpData->Width * 3) == stride)
			{
				if (!MagickConstituteImage(source, stdimg->Width, stdimg->Height, "BGR", CharPixel, rawimg.ToPointer()))    //NULLだと警告
					System::Windows::Forms::MessageBox::Show("Failed to pass image from AForge to ImagickMagick.");
			}
			else
			{
				size_t frm_byte_size = stdimg->Width * stdimg->Height * 3;
				size_t dest_linebytes = stdimg->Width * 3;
				trimmedDIB = new byte[frm_byte_size];
				byte* dest_addr = trimmedDIB;
				byte* src_addr = (byte*)rawimg.ToPointer();
				for (int k = 0; k < stdimg->Height; ++k)
				{
					_CRT_SECURE_MEMCPY(dest_addr, dest_linebytes, src_addr, dest_linebytes);
					src_addr += stride;
					dest_addr += dest_linebytes;
				}
				if (!MagickConstituteImage(source, stdimg->Width, stdimg->Height, "BGR", CharPixel, trimmedDIB))    //NULLだと警告
					System::Windows::Forms::MessageBox::Show("Failed to pass image from AForge(/w stride) to ImagickMagick.");
			}
			
			//Check if the user wants to set transparency
			if (config.maketransparent)
			{
				//Set Alpha
				MagickSetImageAlphaChannel(source, SetAlphaChannel);
				//User bg color
				System::Drawing::Color user_color;
				

				PixelWand* bg = NewPixelWand();
				//PixelSetColor(bg, "#FFFFFFFF");
				PixelSetAlphaQuantum(bg, 255);
				PixelSetRedQuantum(bg, config.t_red);
				PixelSetGreenQuantum(bg, config.t_green);
				PixelSetBlueQuantum(bg, config.t_blue);
				PixelSetFuzz(bg, (double)config.fuzz);
				//Set BG
				//MagickSetImageBackgroundColor(source, bg);
				//Add transparency

				//PixelWand* target = NewPixelWand();
				//PixelSetColor(target, "#00000000");
				//PixelSetAlpha(target, 0.0);
				
				//MagickOpaquePaintImage(source, bg, target, 3.0, MagickFalse);
				MagickTransparentPaintImage(source, bg, 0.0, (double)config.fuzz, MagickFalse);
				//PixelSetAlpha(bg, 0.0);
				double sigma = 2.0 + (config.fuzz / 100); //Just enough to remove residue background
				double radius = sigma* 2.0;
				MagickBlurImageChannel(source, ChannelType::AlphaChannel, radius, sigma);
				MagickThresholdImageChannel(source, ChannelType::AlphaChannel, 25.0);
				DestroyPixelWand(bg);
				
				//DestroyPixelWand(target);
			}
			
			//Set Delay
			MagickSetImageDelay(source, delay[i]);
			//Add image
			MagickAddImage(dest, source);
			
			//Add loop info
			if (i == 0)
			{
				MagickSetImageProperty(dest, "loop", digit);
			}
			//Set Dispose?
			if (config.maketransparent)
			{
				MagickSetImageDispose(dest, DisposeType::BackgroundDispose);
			}
			
			//
			DestroyMagickWand(source);
			stdimg->UnlockBits(bmpData);
			//delete bmpData;
			rawimg = IntPtr(nullptr);
			bmpData = nullptr;
			delete stdimg;
			delete buffer;
			if (trimmedDIB) delete[] trimmedDIB;
			//stdimg = nullptr;
			//buffer = nullptr;
			GC::Collect();
			
			
									
		}
		else
		{
			System::Windows::Forms::MessageBox::Show("Export Aborted!");
			return FALSE;
		}

	}
	

//	TCHAR *imfmt = MagickGetImageFormat(dest);

	
	//ImagickMagick optimization
	MagickSetImageFuzz(dest, config.fuzz); //Fuzziness
	MagickSetFirstIterator(dest);
	TCHAR map[32];
	int lvlr = 8;
	int lvlg = 8;
	int lvlb = 4;
	if (config.color < 256)
	{
		lvlb = floor(cbrt(config.color));
		if (lvlb < 2) lvlb = 2;
		lvlg = lvlb;
		lvlr = lvlb;
	}
	switch (config.method)
	{
	case Dither_Method::FloydIM:
		{
			
			MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickTrue, MagickFalse);
		}
		break;
	case Dither_Method::imchecker:
	{
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		sprintf_s(map, sizeof(map), "checks,%d,%d,%d", lvlr, lvlg, lvlb);
		for (; MagickHasNextImage(dest); MagickNextImage(dest))
		{
			if (!MagickOrderedPosterizeImage(dest, map))
			{
				ExceptionType errortype = MagickGetExceptionType(dest);
				TCHAR* msg = MagickGetException(dest, &errortype);
				System::String ^dnMsg = gcnew String(msg);
				System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
			};
		}
		if (!MagickOrderedPosterizeImage(dest, map))
		{
			ExceptionType errortype = MagickGetExceptionType(dest);
			TCHAR* msg = MagickGetException(dest, &errortype);
			System::String ^dnMsg = gcnew String(msg);
			System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
		};
		
	}
		break;
	case Dither_Method::imo22:
	{
		sprintf_s(map, sizeof(map), "o2x2,%d,%d,%d", lvlr, lvlg, lvlb);
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		for (; MagickHasNextImage(dest); MagickNextImage(dest))
		{
			if (!MagickOrderedPosterizeImage(dest, map))
			{
				ExceptionType errortype = MagickGetExceptionType(dest);
				TCHAR* msg = MagickGetException(dest, &errortype);
				System::String ^dnMsg = gcnew String(msg);
				System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
			};
		}
		if (!MagickOrderedPosterizeImage(dest, map))
		{
			ExceptionType errortype = MagickGetExceptionType(dest);
			TCHAR* msg = MagickGetException(dest, &errortype);
			System::String ^dnMsg = gcnew String(msg);
			System::Windows::Forms::MessageBox::Show(dnMsg,"Missing thresholds.xml");
		};
		
	}
		break;
	case Dither_Method::imo33:
	{
		sprintf_s(map, sizeof(map), "o3x3,%d,%d,%d", lvlr, lvlg, lvlb);
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		for (; MagickHasNextImage(dest); MagickNextImage(dest))
		{
			if (!MagickOrderedPosterizeImage(dest, map))
			{
				ExceptionType errortype = MagickGetExceptionType(dest);
				TCHAR* msg = MagickGetException(dest, &errortype);
				System::String ^dnMsg = gcnew String(msg);
				System::Windows::Forms::MessageBox::Show(dnMsg);
			};
		}
		if (!MagickOrderedPosterizeImage(dest, map))
		{
			ExceptionType errortype = MagickGetExceptionType(dest);
			TCHAR* msg = MagickGetException(dest, &errortype);
			System::String ^dnMsg = gcnew String(msg);
			System::Windows::Forms::MessageBox::Show(dnMsg);
		};
		
		
	}
		break;
	case Dither_Method::imo88:
	{
		sprintf_s(map, sizeof(map), "o8x8,%d,%d,%d", lvlr, lvlg, lvlb);
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		for (; MagickHasNextImage(dest); MagickNextImage(dest))
		{
			if (!MagickOrderedPosterizeImage(dest, map))
			{
				ExceptionType errortype = MagickGetExceptionType(dest);
				TCHAR* msg = MagickGetException(dest, &errortype);
				System::String ^dnMsg = gcnew String(msg);
				System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
			};
		}
		if (!MagickOrderedPosterizeImage(dest, map))
		{
			ExceptionType errortype = MagickGetExceptionType(dest);
			TCHAR* msg = MagickGetException(dest, &errortype);
			System::String ^dnMsg = gcnew String(msg);
			System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
		};
		
	}
		break;
	case Dither_Method::imh44:
	{
		sprintf_s(map, sizeof(map), "h4x4a,%d,%d,%d", lvlr, lvlg, lvlb);
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		for (; MagickHasNextImage(dest); MagickNextImage(dest))
		{
			if (!MagickOrderedPosterizeImage(dest, map))
			{
				ExceptionType errortype = MagickGetExceptionType(dest);
				TCHAR* msg = MagickGetException(dest, &errortype);
				System::String ^dnMsg = gcnew String(msg);
				System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
			};
		}
		if (!MagickOrderedPosterizeImage(dest, map))
		{
			ExceptionType errortype = MagickGetExceptionType(dest);
			TCHAR* msg = MagickGetException(dest, &errortype);
			System::String ^dnMsg = gcnew String(msg);
			System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
		};
	}
		break;
	case Dither_Method::imh44o:
	{
		sprintf_s(map, sizeof(map), "h4x4o,%d,%d,%d", lvlr, lvlg, lvlb);
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		for (; MagickHasNextImage(dest); MagickNextImage(dest))
		{
			if (!MagickOrderedPosterizeImage(dest, map))
			{
				ExceptionType errortype = MagickGetExceptionType(dest);
				TCHAR* msg = MagickGetException(dest, &errortype);
				System::String ^dnMsg = gcnew String(msg);
				System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
			};
		}
		if (!MagickOrderedPosterizeImage(dest, map))
		{
			ExceptionType errortype = MagickGetExceptionType(dest);
			TCHAR* msg = MagickGetException(dest, &errortype);
			System::String ^dnMsg = gcnew String(msg);
			System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
		};
	}
		break;
	case Dither_Method::imh66:
	{
		sprintf_s(map, sizeof(map), "h6x6a,%d,%d,%d", lvlr, lvlg, lvlb);
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		for (; MagickHasNextImage(dest); MagickNextImage(dest))
		{
			if (!MagickOrderedPosterizeImage(dest, map))
			{
				ExceptionType errortype = MagickGetExceptionType(dest);
				TCHAR* msg = MagickGetException(dest, &errortype);
				System::String ^dnMsg = gcnew String(msg);
				System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
			};
		}
		if (!MagickOrderedPosterizeImage(dest, map))
		{
			ExceptionType errortype = MagickGetExceptionType(dest);
			TCHAR* msg = MagickGetException(dest, &errortype);
			System::String ^dnMsg = gcnew String(msg);
			System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
		};
	}
		break;
	case Dither_Method::imh66o:
	{
		sprintf_s(map, sizeof(map), "h6x6o,%d,%d,%d", lvlr, lvlg, lvlb);
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		for (; MagickHasNextImage(dest); MagickNextImage(dest))
		{
			if (!MagickOrderedPosterizeImage(dest, map))
			{
				ExceptionType errortype = MagickGetExceptionType(dest);
				TCHAR* msg = MagickGetException(dest, &errortype);
				System::String ^dnMsg = gcnew String(msg);
				System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
			};
		}
		if (!MagickOrderedPosterizeImage(dest, map))
		{
			ExceptionType errortype = MagickGetExceptionType(dest);
			TCHAR* msg = MagickGetException(dest, &errortype);
			System::String ^dnMsg = gcnew String(msg);
			System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
		};
	}
		break;
	case Dither_Method::imh88:
	{
		sprintf_s(map, sizeof(map), "h8x8a,%d,%d,%d", lvlr, lvlg, lvlb);
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		for (; MagickHasNextImage(dest); MagickNextImage(dest))
		{
			if (!MagickOrderedPosterizeImage(dest, map))
			{
				ExceptionType errortype = MagickGetExceptionType(dest);
				TCHAR* msg = MagickGetException(dest, &errortype);
				System::String ^dnMsg = gcnew String(msg);
				System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
			};
		}
		if (!MagickOrderedPosterizeImage(dest, map))
		{
			ExceptionType errortype = MagickGetExceptionType(dest);
			TCHAR* msg = MagickGetException(dest, &errortype);
			System::String ^dnMsg = gcnew String(msg);
			System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
		};
	}
		break;
	case Dither_Method::imh88o:
	{
		sprintf_s(map, sizeof(map), "h8x8o,%d,%d,%d", lvlr, lvlg, lvlb);
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		for (; MagickHasNextImage(dest); MagickNextImage(dest))
		{
			if (!MagickOrderedPosterizeImage(dest, map))
			{
				ExceptionType errortype = MagickGetExceptionType(dest);
				TCHAR* msg = MagickGetException(dest, &errortype);
				System::String ^dnMsg = gcnew String(msg);
				System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
			};
		}
		if (!MagickOrderedPosterizeImage(dest, map))
		{
			ExceptionType errortype = MagickGetExceptionType(dest);
			TCHAR* msg = MagickGetException(dest, &errortype);
			System::String ^dnMsg = gcnew String(msg);
			System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
		};
	}
		break;
	case Dither_Method::imc55b:
	{
		sprintf_s(map, sizeof(map), "c5x5b,%d,%d,%d", lvlr, lvlg, lvlb);
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		for (; MagickHasNextImage(dest); MagickNextImage(dest))
		{
			if (!MagickOrderedPosterizeImage(dest, map))
			{
				ExceptionType errortype = MagickGetExceptionType(dest);
				TCHAR* msg = MagickGetException(dest, &errortype);
				System::String ^dnMsg = gcnew String(msg);
				System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
			};
		}
		if (!MagickOrderedPosterizeImage(dest, map))
		{
			ExceptionType errortype = MagickGetExceptionType(dest);
			TCHAR* msg = MagickGetException(dest, &errortype);
			System::String ^dnMsg = gcnew String(msg);
			System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
		};
	}
		break;
	case Dither_Method::imc55w:
	{
		sprintf_s(map, sizeof(map), "c5x5w,%d,%d,%d", lvlr, lvlg, lvlb);
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		for (; MagickHasNextImage(dest); MagickNextImage(dest))
		{
			if (!MagickOrderedPosterizeImage(dest, map))
			{
				ExceptionType errortype = MagickGetExceptionType(dest);
				TCHAR* msg = MagickGetException(dest, &errortype);
				System::String ^dnMsg = gcnew String(msg);
				System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
			};
		}
		if (!MagickOrderedPosterizeImage(dest, map))
		{
			ExceptionType errortype = MagickGetExceptionType(dest);
			TCHAR* msg = MagickGetException(dest, &errortype);
			System::String ^dnMsg = gcnew String(msg);
			System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
		};
	}
		break;
	case Dither_Method::imc77b:
	{
		sprintf_s(map, sizeof(map), "c7x7b,%d,%d,%d", lvlr, lvlg, lvlb);
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		for (; MagickHasNextImage(dest); MagickNextImage(dest))
		{
			if (!MagickOrderedPosterizeImage(dest, map))
			{
				ExceptionType errortype = MagickGetExceptionType(dest);
				TCHAR* msg = MagickGetException(dest, &errortype);
				System::String ^dnMsg = gcnew String(msg);
				System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
			};
		}
		if (!MagickOrderedPosterizeImage(dest, map))
		{
			ExceptionType errortype = MagickGetExceptionType(dest);
			TCHAR* msg = MagickGetException(dest, &errortype);
			System::String ^dnMsg = gcnew String(msg);
			System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
		};
	}
		break;
	case Dither_Method::imc77w:
	{
		sprintf_s(map, sizeof(map), "c7x7w,%d,%d,%d", lvlr, lvlg, lvlb);
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		for (; MagickHasNextImage(dest); MagickNextImage(dest))
		{
			if (!MagickOrderedPosterizeImage(dest, map))
			{
				ExceptionType errortype = MagickGetExceptionType(dest);
				TCHAR* msg = MagickGetException(dest, &errortype);
				System::String ^dnMsg = gcnew String(msg);
				System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
			};
		}
		if (!MagickOrderedPosterizeImage(dest, map))
		{
			ExceptionType errortype = MagickGetExceptionType(dest);
			TCHAR* msg = MagickGetException(dest, &errortype);
			System::String ^dnMsg = gcnew String(msg);
			System::Windows::Forms::MessageBox::Show(dnMsg, "Missing thresholds.xml");
		};
	}
		break;
	default:
	{
		//Do Nothing
	}
	}
	//TCHAR *imfmt1 = MagickGetImageFormat(dest);
	/*
	if (config.method == Dither_Method::FloydIM)
	{
		MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickTrue, MagickFalse);
	}
	else
	{
		//MagickQuantizeImages(dest, config.color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
	}
	*/
	if (!MagickSetFormat(dest, "GIF"))
		System::Windows::Forms::MessageBox::Show("Failed to setup GIF format");
	/*
	//Set Dispose?
	if (config.maketransparent)
	{
		size_t image_count = MagickGetNumberImages(dest);
		MagickSetFirstIterator(dest);
		for (size_t i = 0; i < image_count; i++)
		{
			MagickSetImageDispose(dest, DisposeType::PreviousDispose);
			MagickNextImage(dest);
		}
		MagickSetFirstIterator(dest);
	}*/
	
	
	
	//if (config.method == Dither_Method::FloydIM)
	//{
	if (config.adaptiveinsert)
	{
		MagickWand *imCoal = MagickCoalesceImages(dest);
		MagickWand *opt = MagickOptimizePlusImageLayers(imCoal);
		//dest = MagickOptimizePlusImageLayers(MagickCoalesceImages(dest)); //Adaptive frame insert
		DestroyMagickWand(imCoal);
		ClearMagickWand(dest);
		dest = CloneMagickWand(opt);
		DestroyMagickWand(opt);
	}
	else
	{
		MagickWand *imCoal = MagickCoalesceImages(dest);
		MagickWand *opt = MagickOptimizeImageLayers(imCoal);
		//dest = MagickOptimizeImageLayers(MagickCoalesceImages(dest)); //Layer optimization
		DestroyMagickWand(imCoal);
		ClearMagickWand(dest);
		dest = CloneMagickWand(opt);
		DestroyMagickWand(opt);
	}
		
			
		MagickOptimizeImageTransparency(dest);  //Turn this back ON!
		//Discard local colormaps
		if (config.droplocal)
		{
			MagickWand* appimg = MagickAppendImages(dest, MagickFalse);
						
			int colorcount = MagickGetImageColors(appimg);
			if (colorcount > 256) //reduce to 256 colors if exceeded
			{
				MagickQuantizeImage(appimg, 256, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
			}
			MagickSetFormat(appimg, "GIF");
			MagickUniqueImageColors(appimg); //simplify the image
			
			int framecount = MagickGetNumberImages(dest);
			MagickSetFirstIterator(dest);
			for (int i = 0; i < framecount; ++i)
			{
				MagickRemapImage(dest, appimg, DitherMethod::NoDitherMethod);
				MagickNextImage(dest);
			}
			DestroyMagickWand(appimg);
		}
	//}
	//Convert MBCS path name to UTF-8
		size_t path_length = strlen(oip->savefile);
		CHAR* u8buffer = new CHAR[MAX_PATH];
		SecureZeroMemory(u8buffer, MAX_PATH);
		WCHAR* u16buffer = new WCHAR[MAX_PATH];
		SecureZeroMemory(u16buffer, MAX_PATH * 2);
		int char_count = MultiByteToWideChar(CP_THREAD_ACP, NULL, oip->savefile, (int)path_length, u16buffer, 0);
		MultiByteToWideChar(CP_THREAD_ACP, NULL, oip->savefile, (int)path_length, u16buffer, char_count);
		int u8count = WideCharToMultiByte(CP_UTF8, NULL, u16buffer, char_count, u8buffer, 0, NULL, NULL);
		WideCharToMultiByte(CP_UTF8, NULL, u16buffer, char_count, u8buffer, (int)u8count, NULL, NULL);
	//Save to file
		
	if (!MagickWriteImages(dest, u8buffer, MagickTrue))
		System::Windows::Forms::MessageBox::Show("Unable to save to file");
	delete[] u16buffer;
	delete[] u8buffer;
	
	//Clean-up
	DestroyMagickWand(dest);

	/* Debug Use
	if (MagickTrue)
	{
		System::Windows::Forms::MessageBox::Show("Remap=True");
	}
	else
	{
		System::Windows::Forms::MessageBox::Show("Remap=False");
	}*/
	MagickWandTerminus();
	return TRUE;
}


BOOL func_config(HWND hwnd, HINSTANCE dll_hinst)
{
	//OK
	ShowfrmConfig(&config);
	return FALSE;
}

int func_config_get(void *data, int size)
{
	//OK
	if (data && size == sizeof(CONFIG))
	{
		memcpy_s(data, sizeof(config), &config, sizeof(config));
	}
	return sizeof(config);
}

int func_config_set(void *data, int size)
{
	//OK
	if (size != sizeof(config)) return NULL;
	memcpy_s(&config, sizeof(config), data, sizeof(config));
	return size;
}

/// -------------------------------------------------
///     設定画面の表示
/// -------------------------------------------------

//[STAThreadAttribute]
void ShowfrmConfig(CONFIG *data) {

	frmConfig frmConf(data);
	frmConf.ShowDialog();
}

Bitmap^ ditherFrame(void *dib, int width, int height, CONFIG* settings)
{
	using namespace AForge::Imaging;
	using namespace AForge::Imaging::Filters;
	using namespace AForge::Imaging::ColorReduction;
	using namespace System::Collections::Generic;
	
	
	{
		//TODO:
		
		// Use IM to get oct-tree color palette
		//MagickWandGenesis();
		MagickWand *source = NewMagickWand();
		MagickConstituteImage(source, width, height, "BGR", CharPixel, dib);
		MagickQuantizeImage(source, settings->color, ColorspaceType::RGBColorspace, 0, MagickFalse, MagickFalse);
		PixelWand **imColorMap = MagickGetImageHistogram(source, (size_t*)&settings->color);
		List<Color> ^imCT = gcnew List<Color>();
		for (int i = 0; i < settings->color; ++i)
		{
			Quantum r, g, b; //Quantum is Unsigned Char
			r = PixelGetRedQuantum(imColorMap[i]);
			g = PixelGetGreenQuantum(imColorMap[i]);
			b = PixelGetBlueQuantum(imColorMap[i]);
			Color thispx = Color::FromArgb(r, g, b);
			imCT->Add(thispx);
			
		}
		//Close MagickWand
		DestroyMagickWand(source);
		DestroyPixelWands(imColorMap, settings->color);
		
		//MagickWandTerminus();
		//Prepre color palette for AForge
		array<Color> ^palette = gcnew array<Color>(settings->color);
		for (int i = 0; i < settings->color; ++i)
		{
			palette[i] = imCT[i];
		}
		
		UnmanagedImage ^worksrc = gcnew UnmanagedImage(IntPtr(dib), width, height, width * 3, Imaging::PixelFormat::Format24bppRgb);
		//Flip image
		Mirror ^flipper = gcnew Mirror(true, false);
		flipper->ApplyInPlace(worksrc);
		

		//Calculate color platte
		//ColorImageQuantizer ^ciq = gcnew ColorImageQuantizer(gcnew MedianCutQuantizer());
		//array<Color> ^colorTable = ciq->CalculatePalette(worksrc, settings->color);
		//Object ^dithering;
		//Perform dithering
		switch ((int)settings->method){
		case 0:
		{
			//TODO:Burkes
			BurkesColorDithering ^dithering = gcnew BurkesColorDithering();
			dithering->ColorTable = palette;
			Bitmap ^out= dithering->Apply(worksrc);
			
			delete worksrc;
			delete flipper;
			delete imCT;
			delete palette;
			GC::Collect();
			return out;
		}
			break;
		case 1:
		{
			//TODO:Floyd
			FloydSteinbergColorDithering ^dithering = gcnew FloydSteinbergColorDithering();
			dithering->ColorTable = palette;
			Bitmap ^out = dithering->Apply(worksrc);
			delete worksrc;
			delete flipper;
			delete imCT;
			delete palette;
			GC::Collect();
			return out;
		}
			break;
		case 2:
		{
			//TODO:JJN
			JarvisJudiceNinkeColorDithering ^dithering = gcnew JarvisJudiceNinkeColorDithering();
			dithering->ColorTable = palette;
			Bitmap ^out = dithering->Apply(worksrc);
			delete worksrc;
			delete flipper;
			delete imCT;
			delete palette;
			GC::Collect();
			return out;
		}
			break;
		case 3:
		{
			//TODO: Ordered
			OrderedColorDithering ^dithering = gcnew OrderedColorDithering();
			dithering->ColorTable = palette;
			
			//Array ^pattern = Array::CreateInstance(Byte::typeid, 2, 2);
			Bitmap ^out = dithering->Apply(worksrc);
			delete worksrc;
			delete flipper;
			delete imCT;
			delete palette;
			GC::Collect();
			return out;
		}
			break;
		case 4:
		{
			//TODO: Sierra3
			SierraColorDithering ^dithering = gcnew SierraColorDithering();
			dithering->ColorTable = palette;
			Bitmap ^out = dithering->Apply(worksrc);
			delete worksrc;
			delete flipper;
			delete imCT;
			delete palette;
			GC::Collect();
			return out;
		}
			break;
		case 5:
		{
			//TODO:Stucki
			StuckiColorDithering ^dithering = gcnew StuckiColorDithering();
			dithering->ColorTable = palette;
			Bitmap ^out = dithering->Apply(worksrc);
			
			//imCT->Clear();
			delete dithering;
			delete flipper;
			delete worksrc;
			delete palette;
			delete imCT;
			GC::Collect();
			return out;
		}
			break;
		case 6:
		{
			//Floyd in ImageMagick
			Bitmap ^out = worksrc->ToManagedImage();
			delete worksrc;
			delete flipper;
			delete imCT;
			delete palette;
			GC::Collect();
			return out;
		}
			break;
		case 7:
		{
			//None
			Bitmap ^out = worksrc->ToManagedImage();
			delete worksrc;
			delete flipper;
			delete imCT;
			delete palette;
			GC::Collect();
			return out;
		}
			break;
		default:
		{
			//Same as 2
			//OrderedColorDithering ^dithering = gcnew OrderedColorDithering();
			//dithering->ColorTable = palette;
			//return dithering->Apply(worksrc);
			Bitmap ^out = worksrc->ToManagedImage();

			//worksrc = nullptr;
			//flipper = nullptr;
			
			delete worksrc;
			delete flipper;
			delete imCT;
			delete palette;
			GC::Collect();
			return out;
		}
		}
	}

}

  