import socket
import optparse
import time
import threading

from queue import Queue

def get_arguments():
    parser = optparse.OptionParser()
    parser.add_option("-i", "--ip", dest="ip_address",
                      help="ip address for victim")
    parser.add_option("-p", "--port", dest="port",
                      help="scan in specific port")
    parser.add_option("-t", "--toport", dest="toport", 
               help="scan in port unitl to port.no")
    (options, arguments) = parser.parse_args()
    if not options.ip_address:
        parser.error(
            "[!] - Please specify an ip_address, use --help for more info.")
    if options.port and options.to_port:
        parser.error(
            "[!] - Please specify a specific port or to_port, use --help for more info.")
    return options



socket.setdefaulttimeout(0.25)
print_lock = threading.Lock()
options = get_arguments()

target = options.ip_address
protocol = 'tcp'
t_IP = socket.gethostbyname(target)
print ('Starting scan on host: ', t_IP)

def portscan(port):
   s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   try:
      con = s.connect((t_IP, port))
      with print_lock:
         print(str(port) + '/' + protocol , 'is open', '   ──────->>   ', socket.getservbyport(port, protocol))
      con.close()
   except:
      pass

def threader():
   while True:
      worker = q.get()
      portscan(worker)
      q.task_done()
      
q = Queue()
print(q)
startTime = time.time()
   
for x in range(100):
   t = threading.Thread(target = threader)
   t.daemon = True
   t.start()
   

if not options.port:
   range_port = 500
   if options.toport:
      range_port = options.toport

   for worker in range(1, 500):
      q.put(worker)
else:
   q.put(options.port)
      
q.join()
print('Time taken:', time.time() - startTime)