import serial
from serial.tools import list_ports
import time
import os

class uart:
    def __init__(self, name='no name', baudrate=9600, bytesize=8, parity='N', port=None):
        """ initialization """
        self.name = name
        self.ser = serial.Serial()
        self.ser.baudrate = baudrate
        self.ser.bytesize = bytesize
        self.ser.parity = parity
        self.ser.timeout = 0.1                 # in seconds
        if port == None:
            self.ser.port = self.find_device()
        else:
            self.ser.port = port
        self.open_connection()

    def cmd_delay(self):
        """ command delay """
        time.sleep(0.02)

    def find_device(self):
        """ find port when is connected device """
        for port in list_ports.comports():
            if self.name in port.description:
                return port.device

        print(self.name + ' is not connected')
        self.list_ports()
        exit(1)

    def open_connection(self):
        """ open connection """
        try:
            self.ser.open()
            print('port {} is open'.format(self.ser.port))
        except serial.SerialException:
            print('port {} opening is fail'.format(self.ser.port))
            exit(1)

        time.sleep(2)
        self.ser.reset_input_buffer()

    def close_connection(self):
        """ end connection """
        self.ser.close()
        print('port is closed \n')

    def read_byte(self):
        """ read one byte """
        try:
            tmp = self.ser.read(1)
        except serial.SerialException:
            print('the device was disconnected')
        if tmp == b'':
            return None
        return int.from_bytes(tmp, byteorder='little', signed=False)

    def writeword(self, cmd):
        """ send command """
        if type(cmd) == str:
            packet = bytearray()
            for c in cmd:            
                packet.append(ord(c))
            self.ser.write(packet)
            print(cmd)
