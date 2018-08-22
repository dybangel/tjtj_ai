#!/usr/bin/python
# -*- coding: UTF-8 -*-
import RPi.GPIO as GPIO
import time
import os

import thread
import threading
import time

import commands
import json
#import requests
import sys
#import re

BUTTON = 17

GPIO.setmode(GPIO.BCM)
GPIO.setup(BUTTON, GPIO.IN)
gstate = "a"
grecord = 0

reload(sys)                     
sys.setdefaultencoding('utf-8')    
# 为线程定义一个函数
def print_time( threadName, delay):
    while True:
        state = GPIO.input(BUTTON)
        global  gstate
        if state:
          gstate="off"
          #print("off")
        else:
          gstate="on"
          #print("on")
         #       os.system("/work/mengwa/mengwa.sh") 
        time.sleep(delay)
    
    #while 1:
    #  time.sleep(delay)
    #  print "%s: %s" % ( threadName, time.ctime(time.time()) )
 
# 创建两个线程
try:
   thread.start_new_thread( print_time, ("Thread-1", 0.1, ) )
#   thread.start_new_thread( print_time, ("Thread-2", 4, ) )
except:
   print "Error: unable to start thread"
 
while 1:
    time.sleep(0.5);
    count = 0
   #如果开关按下，开始录音 并且标记正在录音开关量
    if gstate == "on":
        #global grecord
        #grecord = 1;
        #print grecord
        #os.system("arecord -c 2 -fCD -Dhw:1  -r9900  ./input.pcm &")
        os.system("arecord -c 2 -fCD -Dhw:1  -r9900  ./input.pcm &")
        print 'on'
    #如果没有按开关 如果录音有则杀掉录音病进行识别工作
    else:
        #count=os.popen("ps -aux|grep arecord|grep -v grep|wc -l").readlines() 
        count=commands.getoutput("ps -aux|grep arecord|grep -v grep|wc -l")
        #print count
        if count == "1":
            print "结束录音，开始识别"
            os.system("killall -9 arecord >/dev/null 2>&1")
            ret=commands.getoutput("python iat.py")
            s = json.loads(ret)
            que = s["data"]
            print que
            
            ans =commands.getoutput("./tuling.sh "+que)
            print ans
            os.system("./tts_sample -s '"+ans+"'")
            #url = 'http://www.tuling123.com/openapi/api'##定义http请求的地址，即1
            #headers = {'Content-Type':'application/json'}##定义header头，用dict方式定义，即3
            #data = {'key': '6cbfde5c0ed14a0e842df9b8b41c3b95', 'info': '你好吗？'}##定义参数，同样用dict定义，即4
            #que ="您好吗？"
            #que = que.decode("utf8")
            
            #que = re.sub("[\s+\.\!\/_,$%^*(+\"\']+|[+——！，。？?、~@#￥%……&*（）]+".decode("utf8"), "".decode("utf8"),que)
            #que =re.sub("[\s+\.\!\/_,$%^*(+\"\')]+|[+——()?【】“”！，。？、~@#￥%……&*（）]+',","",que)
            #que =unicode(que, "utf-8")
            #data = '%s%s%s' % ("{'key': '6cbfde5c0ed14a0e842df9b8b41c3b95', 'info': '", que,"'}")
            #data ="{'key':'6cbfde5c0ed14a0e842df9b8b41c3b95','info':'" + que + "'}"
            #print data
            #data =json.loads(data)
            #res = requests.post(url, data, headers)##post请求，且认证user=‘tv2’，password=‘sign’
            #print res.text##就能看到打印结果了。
            #ans =json.loads(res.text)
            #ans =ans["text"]
            #print ans
        #grecord = 0;
        #print grecord
        print 'off'
    #print "this main thread"
    #global gstate
   # print gstate 
   #pass
