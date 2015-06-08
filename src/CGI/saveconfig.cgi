#!/bin/sh

POST_DATA=$(</dev/stdin)

echo "${POST_DATA}" > /mnt/sda1/LifeCatch/userconfig.conf

#USER=`echo "$QUERY_STRING" | sed -n 's/^.*username=\([^&]*\).*/\1/g'
#ECMETH=`echo "$QUERY_STRING" | sed -n 's/^.*econtactmethod=\([^&]*\).*/\1/g'
#ECSMS=`echo "$QUERY_STRING" | sed -n 's/^.*ecsmsnum=\([^&]*\).*/\1/g'
#ECEMAIL=`echo "$QUERY_STRING" | sed -n 's/^.*ecemail=\([^&]*\).*/\1/g'
#FALLMESSAGE=`echo "$QUERY_STRING" | sed -n 's/^.*fallmessage=\([^&]*\).*/\1/g'
#EMERGENCYMESSAGE=`echo "$QUERY_STRING" | sed -n 's/^.*buttonmessage=\([^&]*\).*/\1/g'
#echo "USER= $USER \nECMETH= $ECMETH \nECSMS= $ECSMS \nECEMAIL= $ECEMAIL \nFALLMESSAGE= $FALLMESSAGE \nEMERGENCYMESSAGE= $EMERGENCYMESSAGE \n" > /mnt/sda1/LifeCatch/userconfig.conf
#echo "config saved" > /mnt/sda1/LifeCatch/test.txt

echo "Content-type: text/html"
echo ""

/bin/cat << EOM
﻿<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<head>
<meta content="text/html; charset=utf-8" http-equiv="Content-Type" />
<title>Untitled 1</title>
</head>

<body style="background-color: #0066CC">
<h1 align="center" style="color: #000080">LifeCatch Configuration Data Saved</h1>
</body>
</html>

EOM
