#!/bin/sh
arecord -c 2 -fCD -Dhw:2,0 -d 2 -r16000 /test.pcm
