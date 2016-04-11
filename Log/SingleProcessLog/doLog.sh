#!/bin/bash
var=message
mv $var message.1
echo "mv $var message.1"
for((i=9;i>0;i--));
do
  mv  "$var.$i.gz" "$var.$((i+1)).gz"
  echo  "mv $var.$i.gz" "$var.$((i+1)).gz"
done
gzip message.1
echo "gzip message.1"
   
