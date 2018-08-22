#!/bin/sh
data=$1
str="{\"key\":\"6cbfde5c0ed14a0e842df9b8b41c3b95\",\"info\":\"${data}\"}"
str="{\"key\":\"59b4458112a84815bec1d5d4c62a7316\",\"info\":\"${data}\"}"
#tmp=`curl -H "Content-Type:application/json" -d${str} http://www.tuling123.com/openapi/api`
text_to_tts=`curl -H "Content-Type:application/json" -d${str} http://www.tuling123.com/openapi/api 2>&1`
text_to_tts=`echo $text_to_tts|awk -F '"' '{print $6}'`
echo "${text_to_tts}"
