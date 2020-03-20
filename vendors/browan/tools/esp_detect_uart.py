#!/usr/bin/env python

import serial.tools.list_ports

ports = list(reversed(sorted(p.device for p in serial.tools.list_ports.comports())))

try:
    print(ports[0].encode("ascii", "ignore"))
except IndexError:
    raise RuntimeError("No serial ports found. Connect a device, or use '-p PORT' option to set a specific port.")
