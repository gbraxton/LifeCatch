#!/usr/bin/python

import sys 
import json
sys.path.insert(0, "/usr/lib/python2.7/json/")
from decoder import JSONDecoder
from encoder import JSONEncoder

jd = JSONDecoder()
je = JSONEncoder()
df =  open('lclog.json')
data = json.load(df)
dlist = data['lclog']
dlist.update( {'alert_type': sys.argv[1], 'alert_time': sys.argv[2]} )
data['lclog'] = dlist
df.write(data)
df.close()
	
