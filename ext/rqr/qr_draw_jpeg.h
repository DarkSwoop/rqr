#ifndef _QR_DRAW_JPEG_
#define _QR_DRAW_JPEG_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "qr_draw.h"

#ifdef USE_JPG
	#include <jpeglib.h>
#endif

//=============================================================================
// QRDrawJPEG クラス
//=============================================================================
class QRDrawJPEG : public QRDraw
{
	private:
		int raster(unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE]);
		int write();
	
	public:
		QRDrawJPEG();
		~QRDrawJPEG();
		int draw(char *filename, int modulesize, int symbolsize,
                      unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE], void *opt);
};

#endif
