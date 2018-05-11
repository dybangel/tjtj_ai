# -*- coding: UTF-8 -*-  
  
from aip import AipOcr  
import json  
  
# 定义常量  
APP_ID = '11166700'  
API_KEY = 'jHh9qIS91QDZuQnx7EnZGHPi'  
SECRET_KEY = '67MAOdDGypcrts4vH0FOGshzyPIq74Z3'  
  
# 初始化AipFace对象  
aipOcr = AipOcr(APP_ID, API_KEY, SECRET_KEY)  
  
# 读取图片  
filePath = "image.jpg"  
def get_file_content(filePath):  
    with open(filePath, 'rb') as fp:  
        return fp.read()  
  
# 定义参数变量  
options = {  
  'detect_direction': 'true',  
  'language_type': 'CHN_ENG',  
}  
  
# 调用通用文字识别接口  
result = aipOcr.basicGeneral(get_file_content(filePath), options)  
print(json.dumps(result).decode("unicode-escape"))  
