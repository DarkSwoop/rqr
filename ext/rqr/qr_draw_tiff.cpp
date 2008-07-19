#include "qr_draw_tiff.h"

//=================================================================================
// QRDrawPNG::QRDrawPNG
//=================================================================================
QRDrawTIFF::QRDrawTIFF()
{
#ifdef USE_TIFF
	bit_image  = NULL;
#endif
}

//=================================================================================
// QRDrawTIFF::~QRDrawTIFF
//=================================================================================
QRDrawTIFF::~QRDrawTIFF()
{
#ifdef USE_TIFF
	int i;
	
	if(bit_image){
		for(i=0; i<this->rsize; i++){
			free(bit_image[i]);
		}
		free(bit_image);
	}
#endif
}

//=============================================================================
// QRDrawPNG::draw
//=============================================================================
int QRDrawTIFF::draw(char *filename, int modulesize, int symbolsize,
						unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE], void *opt)
{
#ifdef USE_TIFF
	if(!filename) return(1);
	setup(filename, modulesize, symbolsize);
	if( this->raster(data) )return(1);
	if( this->write() ) return(1);
	
	return(0);
#else
	return(1);
#endif
}

//=================================================================================
// QRDrawTIFF::raster
//=================================================================================
int QRDrawTIFF::raster(unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE])
{
#ifdef USE_TIFF
	int bitw = (int)ceil(this->rsize/8) + 1;
	
	/* 実際にデータを置く領域を確保 */
	bit_image = (unsigned char **)malloc(sizeof(unsigned char *) * this->rsize);
	for(int i=0; i<this->rsize; i++){
		bit_image[i] = (unsigned char *)malloc(bitw);
		memset(bit_image[i], 0, bitw);
	}

	for(int i=0; i<this->ssize; i++){
		int dp  = MARGIN_SIZE*this->msize / 8;			//横方向のバイト位置
		int sht =(MARGIN_SIZE*this->msize % 8) ? 3 : 7;	//ビットシフト
		unsigned char c = 0;							//1バイトの構成を保存

		for(int j=0; j<this->ssize; j++){
			/* 1行分生成 */
			for(int k=0; k<this->msize; k++){
				c += (data[j][i] << sht);
				sht--;
				
				bit_image[(i+MARGIN_SIZE)*this->msize][ dp ] = c;
				
				if(sht < 0){
					sht = 7;
					c   = 0;
					dp++;
				}
			}
		}
		/* モジュールサイズ分縦方向に増やす */
		for(int k=1; k<this->msize; k++){
			memcpy(bit_image[(i+MARGIN_SIZE)*this->msize+k], bit_image[(i+MARGIN_SIZE)*this->msize], bitw);
		}
	}

	return(0);
#else
	return(1);
#endif
}

//=================================================================================
// QRDrawTIFF::write_png
//=================================================================================
int QRDrawTIFF::write()
{
#ifdef USE_TIFF
	TIFF *tiff;
	int  i;

	/* Open the TIFF file */
	if( (tiff=TIFFOpen(this->filename, "w")) == NULL ) return(1);

	/* タグ */
	TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, this->rsize);				/* 幅(ピクセル数) */
	TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, this->rsize);				/* 高(スキャンライン数) */
	TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_NONE);			/* 圧縮モード */
	TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, 1);						/* ピクセルの深さ */
	TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);	/* カラータイプ */
	TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 1);						/* カラープレーン数 */
	TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);		/* スキャン方向 */
	TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);		/* ? */
	TIFFSetField(tiff, TIFFTAG_XRESOLUTION, 72.0);						/* 解像度 */
	TIFFSetField(tiff, TIFFTAG_YRESOLUTION, 72.0);						/* 解像度 */
	TIFFSetField(tiff, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);			/* 解像度の単位(RESUNIT_INCH:インチ) */
	TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, this->rsize);				/* 1ストリップに何行格納されるか */
	
	/* 1行ずつ書き込み */
	for(i=0 ; i<this->rsize ; i++){
		if( TIFFWriteScanline(tiff, bit_image[i], i, 0) < 0 ){
			TIFFClose(tiff);
			return(1);
		}
	}

	TIFFClose(tiff);

	return(0);
#else
	return(1);
#endif
}
