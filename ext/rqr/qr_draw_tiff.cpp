#include "qr_draw_tiff.h"

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
	
	/* ���ۂɃf�[�^��u���̈���m�� */
	bit_image = (unsigned char **)malloc(sizeof(unsigned char *) * this->rsize);
	for(int i=0; i<this->rsize; i++){
		bit_image[i] = (unsigned char *)malloc(bitw);
		memset(bit_image[i], 0, bitw);
	}

	for(int i=0; i<this->ssize; i++){
		int dp  = MARGIN_SIZE*this->msize / 8;			//�������̃o�C�g�ʒu
		int sht =(MARGIN_SIZE*this->msize % 8) ? 3 : 7;	//�r�b�g�V�t�g
		unsigned char c = 0;							//1�o�C�g�̍\����ۑ�

		for(int j=0; j<this->ssize; j++){
			/* 1�s������ */
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
		/* ���W���[���T�C�Y���c�����ɑ��₷ */
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

	/* �^�O */
	TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, this->rsize);				/* ��(�s�N�Z����) */
	TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, this->rsize);				/* ��(�X�L�������C����) */
	TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_NONE);			/* ���k���[�h */
	TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, 1);						/* �s�N�Z���̐[�� */
	TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);	/* �J���[�^�C�v */
	TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 1);						/* �J���[�v���[���� */
	TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);		/* �X�L�������� */
	TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);		/* ? */
	TIFFSetField(tiff, TIFFTAG_XRESOLUTION, 72.0);						/* �𑜓x */
	TIFFSetField(tiff, TIFFTAG_YRESOLUTION, 72.0);						/* �𑜓x */
	TIFFSetField(tiff, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);			/* �𑜓x�̒P��(RESUNIT_INCH:�C���`) */
	TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, this->rsize);				/* 1�X�g���b�v�ɉ��s�i�[����邩 */
	
	/* 1�s���������� */
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
