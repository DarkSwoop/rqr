#ifndef _QR_DRAW_PS_
#define _QR_DRAW_PS_

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include "qr_draw.h"

#ifdef USE_TIFF
	#include "qr_draw_tiff.h"
#endif

//=============================================================================
// QRDrawPS ƒNƒ‰ƒX
//=============================================================================
class QRDrawPS : public QRDraw
{
	private:
		unsigned int tsize;
		
	private:
		int write(unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE], char *mode);	//normal ps
		int write2(unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE]);	//preview ps
		void write_header(FILE *stream);
		int write_preview(FILE *stream, unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE]);
		void littleEndian(int data, unsigned char *lt);
		
	public:
		QRDrawPS(){}
		~QRDrawPS(){}
		int draw(char *filename, int modulesize, int symbolsize, 
							unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE], void *opt);
};

#endif
