import socket  
import random
import os
import sys
import time
import thread
import threading

#def client_send(seq,fd,target):
#    while True:  
#        message= "seq:%s message:%s" % (seq, msg if msg else "client->server message")
#        fd.sendto(message, address)
#        print "send:", message, "to", address 
#        seq=seq + 1
#        time.sleep(0.5)
#    fd.close() 

output_mutex = threading.Lock() 
def sync_output(msg):
    output_mutex.acquire()
    print data
    output_mutex.release()
    
def client_recv(fd):
    while True:
        data, addr = fd.recvfrom(2048)  
        if data:  
            sync_output(data)

def client_run(fd,dst_ip,dst_port,msg=None):
    seq = random.randint(0,1000)
    address = (str(dst_ip), int(dst_port))  
    #s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  
    
    thread.start_new_thread( client_recv, (fd) )
    while True:  
        message= "seq:%s message:%s" % (seq, msg if msg else "client->server message")
        fd.sendto(message, address)
        sync_output("send:%s to %s" % ( message, address)) 
        seq=seq + 1
        time.sleep(0.5)
    fd.close() 

def create_socket(bind_port,bind_ip="0.0.0.0"):
    address = (str(bind_ip),int(bind_port))
    s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    s.bind(address)
    return s
   

def server_run(fd):
    while True:  
        data, addr = fd.recvfrom(2048)  
        if not data:  
            print "client has exist"  
            break  
        sync_output("received:%s from %s" % (data, addr))  
        fd.sendto('recv %s ok.\n' % data,addr)
  
    fd.close()   


def start(mode):
    if mode == "client":
	print "run clinet"
	fd = create_socket('1025','192.168.50.2')
	#write to 192.168.50.2:1025->172.16.50.202:1025
	client_run(fd,'172.16.50.202','1025')
    elif mode == "server":
	print "run server"
	fd = create_socket('1025','172.16.50.202')
	server_run(fd)
    else:
	print "error parameter"

if __name__ == "__main__":
    start(sys.argv[1])

