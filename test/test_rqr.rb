require File.dirname(__FILE__) + '/test_helper.rb'

class TestRqr < Test::Unit::TestCase
  TEST_JPEG='test.jpg'
  TEST_TIFF='test.tiff'
  TEST_PNG='test.png'
  TEST_EPS='test.eps'
  TEST_EPS_WITH_PREVIEW='test_preview.eps'

  def setup
    clear_test_file
  end
  
  def test_jpeg
    qrcode = RQR::QRCode.new()
    qrcode.save('http://www.google.com',TEST_JPEG)    
    assert_equal(true, File.exist?(TEST_JPEG))
    delete_file(TEST_JPEG)
  end

  def test_tiff
    qrcode = RQR::QRCode.new()
    qrcode.save('http://www.yahoo.com',TEST_TIFF)
    assert_equal(true, File.exist?(TEST_TIFF))
    delete_file(TEST_TIFF)
  end

  def test_png
    qrcode = RQR::QRCode.new()
    qrcode.save('http://www.ebay.com',TEST_PNG)    
    assert_equal(true, File.exist?(TEST_PNG))
    delete_file(TEST_PNG)
  end

  def test_eps
    qrcode = RQR::QRCode.new()
    qrcode.save('http://www.ebay.com',TEST_EPS)
    assert_equal(true, File.exist?(TEST_EPS))
    delete_file(TEST_EPS)
  end

  def test_eps_with_preview
    qrcode = RQR::QRCode.new(:eps_preview => true)
    qrcode.save('http://www.amazon.com',TEST_EPS_WITH_PREVIEW)
    assert_equal(true, File.exist?(TEST_EPS_WITH_PREVIEW))
    delete_file(TEST_EPS_WITH_PREVIEW)
  end

  def test_format_not_found_exception
    qrcode = RQR::QRCode.new()
    assert_raise(RQR::FormatNotFoundException) {qrcode.save('http://www.ebay.com',"test.error")}
    delete_file(TEST_EPS)
  end
  
  def clear_test_file
    delete_file(TEST_TIFF)
    delete_file(TEST_PNG)
    delete_file(TEST_EPS)
    delete_file(TEST_EPS_WITH_PREVIEW)
  end
  
  def delete_file(filepath)
    File.delete(filepath) if File.exist?(filepath)
  end

end
