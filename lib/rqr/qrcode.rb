
module RQR
  class QRCode
    # options
    #  :level       L:0, M:1(default), Q:2, H:3
    #  :version     S:0(default), M:1, L:2
    #  :auto_extend  true(default)|false auto extent if over version size
    #  :masking     masking pattern 0-7, -1(default auto)
    #  :length      data length
    #  :module_size module pixel size
    #  :eps_preview true|false(default)
  	def initialize(options = {})
  		@options = { :level => 1, :version => 0, :auto_extend => true, 
  		             :masking => -1, :eps_preview => false, :module_size => 4 }
      @options.merge!(options)
  	end
  	
    # data::  data for qrcode
    # path::  path for qrcode image file
    # format:: image format. :jpg|:png|:tiff|:eps
  	def save(data, path, format=nil)
  		format ||= get_format(path)

			init_encoder(data)

  		case format.to_sym
  			when :jpg, :jpeg
  				res = save_as_jpeg(path)
  			when :png
  				res = save_as_png(path)
  			when :tiff, :tif
  				res = save_as_tiff(path)
  			when :eps
  				res = save_as_eps(path)
  			else
  			  close; raise RQR::FormatNotFoundException.new("invalid format! #{format}")
  		end
      
      close
      
      if res != 0
        raise RQR::ImageException.new("qrcode image error! #{path} wasn't created.")
  		end
  	end
	
  	def close()
  	  @encoder = nil if @encoder
  	  @imager = nil if @imager
  	end
	
  private
  	def get_format(path)
  		File.extname(path).gsub(/^\./,"").to_sym
  	end

  	def init_encoder(data)
      @encoder = QR::CQR_Encode.new
      unless @encoder.EncodeData(@options[:level], @options[:version], @options[:auto_extend], @options[:masking], data)
        close; raise EncodeException.new("qrcode encord error!")
      end
	  end

  	def save_as_jpeg(path)
      @imager=QR::QRDrawJPEG.new
      @imager.draw(path, @options[:module_size], @encoder.m_nSymbleSize, @encoder.m_byModuleData, nil)
  	end
	
  	def save_as_png(path)
      @imager=QR::QRDrawPNG.new
      @imager.draw(path, @options[:module_size], @encoder.m_nSymbleSize, @encoder.m_byModuleData, nil)
  	end

  	def save_as_tiff(path)
      @imager=QR::QRDrawTIFF.new
      @imager.draw(path, @options[:module_size], @encoder.m_nSymbleSize, @encoder.m_byModuleData, nil)
  	end

  	def save_as_eps(path)
      opt= @options[:eps_preview] ? DL::PtrData.allocate : nil
      @imager=QR::QRDrawPS.new
      ret = @imager.draw(path, @options[:module_size], @encoder.m_nSymbleSize, @encoder.m_byModuleData, opt)
      opt.free if opt
      ret
  	end
  end
end

