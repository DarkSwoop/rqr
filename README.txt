== rqr

* http://rubyforge.org/projects/rqr/

== DESCRIPTION:

A ruby library to create QR code image. Output: PS, JPEG, PNG, EPS, TIFF.

== SYNOPSIS:

	require "rubygem"
	require "rqrcode"

	qr = RQR::QRCode.new()

	#Define a file format with extensions.
	qr.save("http://www.google.com", "path/to/qrcode.jpg")

	#Define a file format with symbol.
	qr.save("http://www.yahoo.com", "path/to/qrcodefile", :png)	


	#QRCode options
	#Use options with hash values.
	#:level       L:0, M:1(default), Q:2, H:3
	#:version     S:0(default), M:1, L:2
	#:auto_extent  true|false auto extent if over version size
	#:masking     masking pattern 0-7, -1(default auto)
	#:length      data length
	#:module_size module size (pixel)
	#:eps_preview true|false

	#This sample creates a EPS with preview.
	qr = QRCode.new(:eps_preview => true, :auto_extent=false)
	qr.save("http://www.ebay.com", "path/to/qrcode.eps")

== REQUIREMENTS:

* libjpeg
* libpng
* libtiff

== Note:
If using OSX, Use Fink(http://www.finkproject.org) Universal Binary libs.
* sudo fink install libjpeg
* sudo fink install libpng
* sudo fink install libtiff

== INSTALL:

* sudo gem install rqr


== LICENSE:

(The MIT License)

Copyright (c) 2008 Ryota Maruko, Keiko Soezima

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.