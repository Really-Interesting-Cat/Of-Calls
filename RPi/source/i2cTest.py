import smbus
import time

class I2C(object):
    I2C_BUS_NUM = 1

    def __init__(self):
        self.master = smbus.SMBus(self.I2C_BUS_NUM)
        self.slave_addr_list = [0x1e]

    def run(self):
        me = self.master
        on_off = True
        while True:
            for addr in self.slave_addr_list:
                try:
                    me.write_byte(addr, int(on_off))
                except IOError:
                    print("IOError")
                    pass
            on_off = not on_off
            time.sleep(1)

def main():
    i2c = I2C()
    i2c.run()


if __name__ == "__main__":
    main()
