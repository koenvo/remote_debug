import time
import sys

import remote_debug


counter = 0
def tracefunc(frame, event, arg):
    global counter
    counter += 1
    1/0
    pass



#sys.settrace(remote_debug.tester)

c = 1000000


def print_s(i):
    return i + i

def callback():
    print "hello"
    #time.sleep(5)
    #1/0
    #print "asdasd"

start = time.time()

remote_debug.install_callback(callback, 100000)

#remote_debug.uninstall()
#sys.settrace(tracefunc)


for i in xrange(c):
    print_s(i)

remote_debug.uninstall()

took = time.time() - start

print "took %.2fs, %.2f loops/sec" % (took, float(c) / took)
print "tracefunc called:", counter
print "part2", remote_debug.current_counter()
