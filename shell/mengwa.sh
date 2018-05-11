#!/bin/sh
#data="音乐";
#exit
killall -9 python
#killall -9 play
iat_bin="python /work/mengwa/iat.py"
tts_bin="/work/mengwa/tts_sample"
##录音调用科大讯飞得到文字
##aplay /work/snowboy_demo/rpi-arm-raspbian-8.0-1.2.0/resources/wozaiyo.wav 
##/work/tools/play /work/snowboy_demo/rpi-arm-raspbian-8.0-1.2.0/resources/wozaiyo.wav 
aplay -q -A 1 -c 1 /work/snowboy_demo/rpi-arm-raspbian-8.0-1.2.0/resources/wozaiyo.wav # arecord -c 2 -fS16_LE -Dhw:1 -d 2 -r 8000 /home/input.pcm
#killall -9 play
##sleep 5
#arecord -c 2 -fS16_LE -Dhw:1 -d 2 -r 8000 /home/input.pcm
arecord -c 2 -fCD -Dhw:1 -d 2 -r9900  /home/input.pcm
#exit
tmp=`python /work/mengwa/iat.py`
#exit
#tmp='{"code":"0","data":"你喜欢吃苹果吗？","desc":"success","sid":"zat0005d511@ch26b10e3e6f573d3700"}'

echo $tmp|awk -F '"' '{print $4,$8}'|while read code data
 do
  echo "code=${code} data=${data}"
if [ "${data}" = '音乐。' ]; then
	play /work/1.mp3
	break
fi
  ##录音的文字发送给图灵平台并得到文字结果
  str="{\"key\":\"6cbfde5c0ed14a0e842df9b8b41c3b95\",\"info\":\"${data}\"}"
  #tmp=`curl -H "Content-Type:application/json" -d${str} http://www.tuling123.com/openapi/api`
  text_to_tts=`curl -H "Content-Type:application/json" -d${str} http://www.tuling123.com/openapi/api`
  text_to_tts=`echo $text_to_tts|awk -F '"' '{print $6}'`
  echo "tts=${text_to_tts}"
  #{"code":100000,"text":"2018年04月27日 星期五 下午 9:40"}
  ###文字结果调用科大讯飞tts播放
  ${tts_bin} -s "$text_to_tts"
 done

python /work/snowboy_demo/rpi-arm-raspbian-8.0-1.2.0/demo.py  /work/snowboy_demo/rpi-arm-raspbian-8.0-1.2.0/resources/mengwa.pmdl 
