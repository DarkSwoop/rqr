#include "qr_draw_jpeg.h"

//=============================================================================
// QRDrawJPEG::draw
//=============================================================================
int QRDrawJPEG::draw(char *filename, int modulesize, int symbolsize,
						unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE], void *opt)
{
#ifdef USE_JPG
	setup(filename, modulesize, symbolsize);
	
	/* �O���[�X�P�[���C���[�W�̍\�z */
	if( this->raster(data) ) return(1);
	
	/* JPEG�����o�� */
	if( this->write() ) return(1);
	
	return(0);
#else
	return(1);
#endif
}

//=================================================================================
// QRDrawJPEG::raster
//=================================================================================
int QRDrawJPEG::raster(unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE])
{
#ifdef USE_JPG
	int i, j, k;
	
	/* ���ۂɃf�[�^��u���̈���m�� */
	bit_image = (unsigned char **)malloc(sizeof(unsigned char *) * this->rsize);
	for(i=0; i<this->rsize; i++){
		bit_image[i] = (unsigned char *)malloc(this->rsize);
		memset(bit_image[i], 0xFF, this->rsize);
	}
	
	for(i=0; i<this->ssize; i++){
		for(j=0; j<this->ssize; j++){
			/* 1�s������ */
			for(k=0; k<this->msize; k++){
				bit_image[(i+MARGIN_SIZE)*this->msize][ (j+MARGIN_SIZE)*this->msize + k ] = data[j][i] ? 0 : 255;
			}
		}
		/* ���W���[���T�C�Y���c�����ɑ��₷ */
		for(k=1; k<this->msize; k++){
			memcpy(bit_image[(i+MARGIN_SIZE)*this->msize+k], bit_image[(i+MARGIN_SIZE)*this->msize], this->rsize);
		}
	}

	return(0);
#else
	return(1);
#endif
}

//=================================================================================
// QRDrawJPEG::write
//=================================================================================
int QRDrawJPEG::write()
{
#ifdef USE_JPG
	struct jpeg_compress_struct oinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW   row_pointer[1];
	FILE *stream;
	int i;

	/* �o�͐�ݒ� */
	if(!this->filename){
		stream = stdout;
	}else{
		if( (stream=fopen(this->filename, "wb")) == NULL ) return(1);
	}

	oinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&oinfo);
	jpeg_stdio_dest(&oinfo, stream);

	oinfo.image_width      = this->rsize;
	oinfo.image_height     = this->rsize;
	oinfo.in_color_space   = JCS_GRAYSCALE;
	oinfo.input_components = 1;
	jpeg_set_defaults(&oinfo);
	jpeg_set_quality(&oinfo, 100, TRUE);
	oinfo.dct_method = JDCT_ISLOW;
	oinfo.write_JFIF_header  = TRUE;

	jpeg_start_compress(&oinfo, TRUE);
	for(i=0; i<this->rsize; i++){
		row_pointer[0] = bit_image[i];
		jpeg_write_scanlines(&oinfo, row_pointer, 1);
	}
	
	jpeg_finish_compress(&oinfo);
	jpeg_destroy_compress(&oinfo);
	
	fclose(stream);
	return(0);
#else
	return(1);
#endif
}
