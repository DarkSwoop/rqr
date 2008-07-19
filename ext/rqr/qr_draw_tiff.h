#ifndef _QR_DRAW_TIFF_
#define _QR_DRAW_TIFF_

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "qr_draw.h"

#ifdef USE_TIFF
	#include <tiffio.h>
#endif

//=============================================================================
// QRDrawTIFF クラス
//=============================================================================
class QRDrawTIFF : public QRDraw
{
	private:
		int raster(unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE]);
		int write();
		
	public:
		QRDrawTIFF();
		~QRDrawTIFF();
		int draw(char *filename, int modulesize, int symbolsize,
                      unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE], void *opt);
};

#endif
