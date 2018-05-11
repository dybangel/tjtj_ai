## 简介

使用bash 脚本方式测试rest api 识别接口

- windows 下需要安装 cygwin，在cygwin内安装curl



## 测试流程

### 修改tts.php

从网页中申请的应用获取appKey和appSecret

```php
# 填写网页上申请的appkey 如 $apiKey="g8eBUMSokVB1BHGmgxxxxxx"
$apiKey = "4E1BG9lTnlSeIf1NQFlrSq6h";
# 填写网页上申请的APP SECRET 如 $secretKey="94dc99566550d87f8fa8ece112xxxxx"
$secretKey = "544ca4657ba8002e3dea3ac2f5fdd241";
```






## 运行 tts.php，进行合成

命令为 php tts.php

结果在result.mp3，如果遇见错误，结果在result.txt

其中

- Content-Type: audio/mp3，表示合成成功，可以播放MP3 result.mp3
- Content-Type: application/json 表示错误   result.txt打开可以看到错误信息的json

### 修改合成参数

```php
$text = "欢迎使用百度语音";

#发音人选择, 0为普通女声，1为普通男生，3为情感合成-度逍遥，4为情感合成-度丫丫，默认为普通女声
$per = 0;
#语速，取值0-9，默认为5中语速
$spd = 5;
#音调，取值0-9，默认为5中语调
$pit = 5;
#音量，取值0-9，默认为5中音量
$vol = 5;

$cuid = "123456PHP";

```

