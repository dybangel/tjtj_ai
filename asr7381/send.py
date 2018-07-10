import serial
t = serial.Serial('/dev/ttyS0',115200)
t.write("FD 00 0A 01 00 D3 EE D2 F4 CC EC CF C2".encode('gb2312'))
