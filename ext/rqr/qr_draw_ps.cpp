#include "qr_draw_ps.h"

//=================================================================================
// QRDrawPS::QRDrawPS
//=================================================================================
QRDrawPS::QRDrawPS()
{
}

//=============================================================================
// QRDrawPS::draw
//=============================================================================
int QRDrawPS::draw(char *filename, int modulesize, int symbolsize, 
					unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE], void *opt)
{
	if(!filename) return(1);
	setup(filename, modulesize, symbolsize);
	
	bool preview;
	if(!opt){
		preview = false;
	}else{
		preview = (bool)opt;
	}
	
	if(preview){
#ifdef USE_TIFF
		return( this->write2(data) );
#endif
	}else{
		return( this->write(data, "wb") );
	}
	
	return(1);
}

//=============================================================================
// QRDrawPS::write_preview_eps
//=============================================================================
int QRDrawPS::write2(unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE])
{
#ifdef USE_TIFF
	FILE *stream;
	unsigned char lt[4];
	struct stat st;
	
	if((stream=fopen(this->filename, "wb")) == NULL) return(1);

	/* ダミーヘッダー書き込み */
	this->write_header(stream);
	
	/* TIFF書き込み */
	this->write_preview(stream, data);
	fclose(stream);
	
	/* EPS書き込み */
	this->write(data, "a");
	
	/* EPSサイズ算出 */
	stat(this->filename, &st);
	unsigned int esize = st.st_size - (this->tsize+30);
	
	/* ヘッダー書き換え */
	if((stream=fopen(this->filename, "rb+")) == NULL) return(1);

	/* PS開始位置 */
	fseek(stream, 4, SEEK_SET);
	this->littleEndian(30+this->tsize, lt);
	fprintf(stream, "%c%c%c%c", lt[0], lt[1], lt[2], lt[3]);

	/* PS長 */
	fseek(stream, 8, SEEK_SET);
	this->littleEndian(esize, lt);
	fprintf(stream, "%c%c%c%c", lt[0], lt[1], lt[2], lt[3]);
	
	/* TIFFプレビュー長 */
	fseek(stream, 24, SEEK_SET);
	this->littleEndian(this->tsize, lt);
	fprintf(stream, "%c%c%c%c", lt[0], lt[1], lt[2], lt[3]);
	
	fclose(stream);
	
	return(0);
#else
	return(1);
#endif
}

//=============================================================================
// QRDrawPS::write_header
//=============================================================================
void QRDrawPS::write_header(FILE *stream)
{
	unsigned char lt[4];
	
	/* ナミモニ */
	fprintf(stream, "%c%c%c%c", 0xC5, 0xD0, 0xD3, 0xC6);
	
	/* PostScript開始位置 */
	fprintf(stream, "%c%c%c%c", 0x00, 0x00, 0x00, 0x00);
	
	/* PostScript長さ */
	fprintf(stream, "%c%c%c%c", 0x00, 0x00, 0x00, 0x00);

	/* MetaFile */
	fprintf(stream, "%c%c%c%c", 0x00, 0x00, 0x00, 0x00);
	fprintf(stream, "%c%c%c%c", 0x00, 0x00, 0x00, 0x00);
	
	/* TIFFプレビュー開始位置 */
	littleEndian(30, lt);
	fprintf(stream, "%c%c%c%c", lt[0], lt[1], lt[2], lt[3]);
	
	/* TIFFプレビュー長 */
	fprintf(stream, "%c%c%c%c", 0x00, 0x00, 0x00, 0x00);

	/* チェックサム */
	fprintf(stream, "%c%c", 0xff, 0xff);
}

//=================================================================================
// littleEndian
//=================================================================================
void QRDrawPS::littleEndian(int data, unsigned char *lt)
{
	lt[0] =  data & 0x000000FF;
	lt[1] = (data & 0x0000FF00) >> 8;
	lt[2] = (data & 0x00FF0000) >> 16;
	lt[3] = (data & 0xFF000000) >> 24;
}

//=============================================================================
// QRDrawPS::write_preview
//=============================================================================
int QRDrawPS::write_preview(FILE *stream, unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE])
{
#ifdef USE_TIFF
	FILE *t;
	char tmp[L_tmpnam];
	QRDrawTIFF tif;
	unsigned char *p;
	
	tmpnam(tmp);
	if( tif.draw(tmp, this->msize, this->ssize, data, NULL) ) return(1);
	
	/* TIFFをオープンしてファイルサイズを取得する */
	if((t=fopen(tmp, "rb")) == NULL){
		remove(tmp);
		return(1);
	}
	fseek(t, 0, SEEK_END);
	this->tsize = ftell(t);
	fseek(t, 0, SEEK_SET);

	/* TIFF本体の書き込み */
	p = (unsigned char *)malloc(this->tsize);
	fread(p, 1, this->tsize, t);
	fwrite(p, 1, this->tsize, stream);
	
	free(p);
	fclose(t);
	remove(tmp);
	
	return(0);
#else
	return(1);
#endif
}

//=============================================================================
// QRDrawPS::write_eps
//=============================================================================
int QRDrawPS::write(unsigned char data[MAX_MODULESIZE][MAX_MODULESIZE], char *mode)
{
	FILE *stream;
	int i, j;
	time_t mytime;
	struct tm *lt;

	if((stream=fopen(this->filename, mode)) == NULL) return(1);

	fprintf(stream, "%%!PS-Adobe-3.0 EPSF-3.0\n");
	fprintf(stream, "%%%%Creator: kei-soe\n");
	fprintf(stream, "%%%%Title: %s\n", this->filename);
	
	time(&mytime);
	lt = localtime(&mytime);
	fprintf(stream, "%%%%CreationDate: %04d:%02d:%02d %02d:%02d:%02d\n", 
								lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday,
								lt->tm_hour, lt->tm_min, lt->tm_sec);
	fprintf(stream, "%%%%BoundingBox: 0 0 %d %d\n", this->rsize, this->rsize);
	fprintf(stream, "%%%%HiResBoundingBox: 0 0 %d %d\n", this->rsize, this->rsize);
	fprintf(stream, "%%%%DocumentProcessColors: Cyan Magenta Yellow Black\n");
	fprintf(stream, "%%%%EndComments\n");
	fprintf(stream, "%%%%BeginProlog\n");
	fprintf(stream, "%%%%EndProlog\n");
	fprintf(stream, "%%%%BeginSetup\n");
	fprintf(stream, "%%%%EndSetup\n");
	
	fprintf(stream, "/size %d def\n", this->ssize);
	fprintf(stream, "/margin %d def\n", MARGIN_SIZE);
	fprintf(stream, "gsave\n");
	fprintf(stream, "%d %d scale\n", this->msize, this->msize);
	fprintf(stream, "newpath\n");
	fprintf(stream, "0 0 moveto\n");
	fprintf(stream, "0 size margin 2 mul add rlineto\n");
	fprintf(stream, "size margin 2 mul add 0 rlineto\n");
	fprintf(stream, "0 size margin 2 mul add neg rlineto\n");
	fprintf(stream, "closepath\n");
	fprintf(stream, "1 setgray\n");
	fprintf(stream, "fill\n");
	
	// ドット描画
	for(i=0; i<this->ssize; i++){
		for(j=0; j<this->ssize; j++) fprintf(stream, "%d ", data[j][i]);
		fprintf(stream, "\n");
	}

	fprintf(stream, "margin margin translate\n");
	fprintf(stream, "0 setgray\n");
	fprintf(stream, "1 1 size {\n");
	fprintf(stream, "/ypos exch def\n");
	fprintf(stream, "size -1 1 {\n");
	fprintf(stream, "/xpos exch def\n");
	fprintf(stream, "1 eq {\n");
	fprintf(stream, "newpath\n");
	fprintf(stream, "xpos ypos moveto\n");
	fprintf(stream, "0 -1 rlineto\n");
	fprintf(stream, "-1 0 rlineto\n");
	fprintf(stream, "0 1 rlineto\n");
	fprintf(stream, "closepath\n");
	fprintf(stream, "fill\n");
	fprintf(stream, "} if\n");
	fprintf(stream, "} for\n");
	fprintf(stream, "} for\n");
	fprintf(stream, "grestore\n");

	fclose(stream);

	return(0);
}
