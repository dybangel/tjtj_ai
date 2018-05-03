#!/bin/sh
arecord -c 2 -fS16_LE -Dhw:1,0 -d 2 -r 8000 /test.pcm
