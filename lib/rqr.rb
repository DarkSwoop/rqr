$:.unshift(File.dirname(__FILE__)) unless
  $:.include?(File.dirname(__FILE__)) || $:.include?(File.expand_path(File.dirname(__FILE__)))

module RQR
  require 'dl'
  require "rqr/QR"
  require "rqr/errors"
  require "rqr/qrcode"  
end