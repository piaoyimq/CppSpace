#!/bin/bash -e
#set -x

length=$#
usage() {
        echo "Usage: ./logview.sh directory-name"
        exit 65
}


if [ $length != 1 ]
then
    usage
fi

if [ ! -d $1 ]
then
    usage
fi


isFirstFile=true

for log in `ls $1/*.log*`
do
    FILE_NAME=`basename $log`
    PROCESS_NAME=${FILE_NAME%%.*}
    if [ $isFirstFile = true ]
    then
        cat.sh $log| awk -v process_name="$PROCESS_NAME" '{$4=process_name "[" $4 "]"; print}' > .tmp-log
        isFirstFile=false
    else
        cat.sh $log| awk -v process_name="$PROCESS_NAME" '{$4=process_name "[" $4 "]"; print}' >> .tmp-log
    fi
done

cat .tmp-log |sort -t ' ' -k 1,2
rm -rf .tmp-log
