# coding:utf-8
import serial

class Ser(object):
    def __init__(self):
        # 打开端口
        self.port = serial.Serial(port='/dev/ttyS0', baudrate=115200, bytesize=8, parity='E', stopbits=1, timeout=2)

    # 发送指令的完整流程
    def send_cmd(self, cmd):
        self.port.write(cmd)
        response = self.port.readall()
        response = self.convert_hex(response)
        return response

    # 转成16进制的函数
    def convert_hex(self, string):
        res = []
        result = []
        for item in string:
            res.append(item)
        for i in res:
            result.append(hex(i))
        return result
cmd = [0x01, 0x05, 0x91, 0xF5, 0x00, 0x00, 0xF1, 0x04]
obj=Ser()
#obj.send_cmd(self,cmd)
