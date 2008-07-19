%module QR
%{
#include "win2ansi.h"
#include "QR_Encode.h"
#include "qr_draw.h"
#include "qr_draw_jpeg.h"
#include "qr_draw_png.h"
#include "qr_draw_ps.h"
#include "qr_draw_tiff.h"
%}
%include win2ansi.h
%include qr_draw.h
%include qr_draw_jpeg.h
%include qr_draw_png.h
%include qr_draw_ps.h
%include qr_draw_tiff.h
%include QR_Encode.h
