from onl.platform.base import *
from onl.platform.accton import *
from time import sleep

class OnlPlatform_x86_64_accton_as7946_30xb_r0(OnlPlatformAccton,
                                              OnlPlatformPortConfig_4x400_22x100_4x25):
    PLATFORM='x86-64-accton-as7946-30xb-r0'
    MODEL="AS7946-30XB"
    SYS_OBJECT_ID=".7946.30"

    def baseconfig(self):
        self.insmod('optoe')
        for m in [ 'sys' , 'cpld', 'fan', 'psu', 'leds', 'thermal' ]:
            self.insmod("x86-64-accton-as7946-30xb-%s.ko" % m)

        ########### initialize I2C bus 0 ###########
        self.new_i2c_devices([
                # initialize multiplexer (PCA9548)
                ('pca9548', 0x77, 0), # i2c 1-8

                # initialize multiplexer (PCA9548) of main board
                ('pca9548', 0x76, 1), # i2c 9-16

                # initiate  multiplexer (PCA9548) for QSFP ports
                ('pca9548', 0x73, 9),  # i2c 17-24

                # initiate multiplexer for QSFP ports
                ('pca9548', 0x74, 17), # i2c 25-32
                ('pca9548', 0x74, 18), # i2c 33-40
                ('pca9548', 0x74, 19), # i2c 41-48
                ('pca9548', 0x74, 20), # i2c 49-56

                #initiate CPLD
                ('as7946_30xb_cpld1', 0x61, 12),
                ('as7946_30xb_cpld2', 0x62, 13),
                ])

        # initialize QSFP port(0-3), QSFP28 port(4-25), SFP port(26-29)
        port_i2c_bus = [ 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
                         35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
                         45, 46, 47, 48, 49, 50, 51, 52, 53, 54]

        # initialize QSFP port 25-50
        for port in range(0, 26):
            self.new_i2c_device('optoe1', 0x50, port_i2c_bus[port])
            subprocess.call('echo port%d > /sys/bus/i2c/devices/%d-0050/port_name' % (port, port_i2c_bus[port]), shell=True)

        # initialize SFP port 51-54
        for port in range(26, 30):
            self.new_i2c_device('optoe2', 0x50, port_i2c_bus[port])
            subprocess.call('echo port%d > /sys/bus/i2c/devices/%d-0050/port_name' % (port, port_i2c_bus[port]), shell=True)

        return True
