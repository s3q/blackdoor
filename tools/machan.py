import subprocess
import optparse
import re
import random
import time


def get_arguments():
    parser = optparse.OptionParser()
    parser.add_option("-i", "--interface", dest="interface",
                      help="Interface to change its MAC address")
    parser.add_option("-r", "--random", dest="random_mac",
                      help="Random MAC address")
    parser.add_option("-m", "--mac", dest="new_mac", help="New MAC adrress")
    (options, arguments) = parser.parse_args()
    if not options.interface:
        parser.error(
            "[!] - Please specify an interface, use --help for more info.")
    if not options.new_mac and not options.random_mac:
        parser.error(
            "[!] - Please specify a new or random MAC, use --help for more info.")
    return options


def change_mac(interface, new_mac, random_mac):
    global new_mac_addr
    mac_formate = "00-00-00-00-00-00"


    if new_mac:
        new_mac_addr = new_mac
    elif random_mac:
        if random_mac == "-":
            random_mac = "macaddr.txt"
    
        with open(random_mac, "r") as f:
            list_mac = []
            for line in f.readlines():
                line = str(line).strip()
                list_mac.append(line)
            new_mac_addr = str(random.choice(list_mac)).lower()

    new_mac_addr = str(new_mac_addr)

    print(f"[+] - Changing MAC address for {interface} to {new_mac_addr}")
    if new_mac_addr:
        subprocess.call(["ifconfig", interface, "down"])
        time.sleep(1)
        subprocess.call(["ifconfig", interface, "hw", "ether", new_mac_addr])
        time.sleep(1)
        subprocess.call(["ifconfig", interface, "up"])
    else:
        print("[!] - Invalid MAC address.")


def get_current_mac(interface):
    check_result = subprocess.check_output(["ifconfig", interface])
    check_mac = re.search(r"\w\w:\w\w:\w\w:\w\w:\w\w:\w\w", str(check_result))
    if check_mac:
        return check_mac.group(0)
    else:
        print("[!] - Could not read MAC addrress.")


def run():
    options = get_arguments()
    current_mac = get_current_mac(options.interface)
    print(f"[-] - Current Mac : {current_mac}")

    change_mac(options.interface, options.new_mac, options.random_mac)
    current_mac = get_current_mac(options.interface)

    if current_mac == new_mac_addr:
        print(f"[+] - MAC address has successfully changed to {current_mac}")
    else:
        print(f"[-] - MAC address did not get changed.")


run()
