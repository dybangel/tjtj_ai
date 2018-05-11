#!/bin/sh
question=$1
str="{\"key\":\"6cbfde5c0ed14a0e842df9b8b41c3b95\",\"info\":\"${question}\"}"
curl -H "Content-Type:application/json" -d${str} http://www.tuling123.com/openapi/api 
