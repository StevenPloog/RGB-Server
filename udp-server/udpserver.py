from subprocess import call
import socket

UDP_PORT = 1738
MSG_START = 0xAB
MSG_END = 0xCD

def run_server():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(('', UDP_PORT))

    while True:
        message, address = sock.recvfrom(1024)
        message = message.decode('utf-8')
        msg = message.split(',')
        if int(msg[0]) == MSG_START and len(msg) == 5 and int(msg[4]) == MSG_END:
            rgb_args = str(msg[1]) + ',' + str(msg[2]) + ',' + str(msg[3]) + ','
            args_str = '1,' + '1,' + rgb_args
            #call(['/var/www/mysite/uart', str(2+len(args_str)), args_str])
            print(args_str)

# rgb_args = str(r) + ',' + str(g) + ',' + str(b) + ','
# args_str = '1,' + '1,' + rgb_args
# call(['/var/www/mysite/uart', str(2+len(args_str)), args_str])
