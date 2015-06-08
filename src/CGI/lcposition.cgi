 #!/usr/bin/python

print "Content-type: application/json\n\n"
print "\n\n" 

import sys 
sys.path.insert(0, "/usr/lib/python2.7/bridge/")

sys.path.insert(0, "/usr/lib/python2.7/json/")
import json
from encoder import JSONEncoder
from bridgeclient import BridgeClient

bc = BridgeClient();
je = JSONEncoder();

response = {'accel':bc.get("accel"), 'qx':bc.get('qx'), 'qy':bc.get('qy'), 'qz':bc.get('qz'), 'qw':bc.get('qw')}

print(je.encode(response))

