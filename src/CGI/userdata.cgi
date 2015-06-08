#!/usr/bin/env python

import cgi
import subprocess

form = cgi.FieldStorage()

print("Content-Type: text/plain\n\n")

alldata = ""

for data in form:
	alldata += data + "=" + "\"" + form[data].value + "\"" + "\n"

f = open("/mnt/sda1/LifeCatch/userdata.conf", 'w')
f.write(alldata)
f.close()
