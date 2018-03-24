

#!/usr/bin/env python
# encoding:utf-8
#@author:Lastprism
#@file:check_ali_server.py
#@time:2018/3/112:18
#@contact:Lastprism@163.com

import os
import sys
import subprocess
import time


def get_process_count(imagename):
    p = os.popen('ps -ef | grep %s | grep -v "grep"' % imagename)
    return p.read().count(imagename)

if __name__ == "__main__":
    while True:
        print(get_process_count('./PC3_server_v1'))
        if get_process_count('./PC3_server_v1') == 0:
            print("the program is dead!")
            print(os.popen("./PC3_server_v1 >> pc3.out &"))
        else:
        	print("The program is alive!")
        time.sleep(1)