import serial
def hexShow(argv):  
    result = ''  
    hLen = len(argv)  
    for i in xrange(hLen):  
        hvol = ord(argv[i])  
        hhex = '%02x'%hvol  
        result += hhex+' '  
    print 'hexShow:',result 

t = serial.Serial('/dev/ttyS0',115200)
n = t.write('2')
print t.portstr
print n
str = t.read(n)
#print str
hexShow(str)
