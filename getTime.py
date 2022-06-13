# Get time - By: chenxiao - 周四 6月 9 2022
# In patio 2, task 3, use openmv to obtain time from DS1302 for communication
from machine import Pin, I2C
import time
import network


class DS1302:
    CLK_DELAY = 20

    def __init__(self):
        # CLK and CE (sometime call RST) pin are always output
        self._clk_pin = Pin("P3", Pin.OUT_OD)
        self._data_pin_no = Pin("P8", Pin.OUT_PP)
        self._ce_pin = Pin("P7", Pin.OUT_OD)

        # turn off WP (write protect)
        self._start_tx()
        self._w_byte(0x8e)
        self._w_byte(0x00)
        self._end_tx()
        # charge mode is disabled
        self._start_tx()
        self._w_byte(0x90)
        self._w_byte(0x00)
        self._end_tx()

    def _start_tx(self):
        """
        Start of transaction.
        """
        self._clk_pin.off()
        self._ce_pin.on()


    def _end_tx(self):
        """
        End of transaction.
        """
        self._data_pin = Pin(self._data_pin_no, Pin.IN)
        self._clk_pin.off()
        self._ce_pin.off()

    def _r_byte(self):
        """
        Read byte from the chip.
        :return: byte value
        :rtype: int
        """
        # data pin is now input (pull-down resistor embedded in chip)
        self._data_pin = Pin(self._data_pin_no, Pin.IN)

        # clock the byte from chip
        byte = 0
        for i in range(8):
            # make a high pulse on CLK pin
            self._clk_pin.on()
            time.sleep_us(self.CLK_DELAY)
            self._clk_pin.off()
            time.sleep_us(self.CLK_DELAY)
            # chip out data on clk falling edge: store current bit into byte
            bit = self._data_pin.value()
            byte |= ((2 ** i) * bit)
        # return byte value
        return byte

    def _w_byte(self, byte):
        """
        Write byte to the chip.
        :param byte: byte value
        :type byte: int
        """
        # data pin is now output
        self._data_pin = Pin(self._data_pin_no, Pin.OUT)

        # clock the byte to chip
        for _ in range(8):
            self._clk_pin.off()
            time.sleep_us(self.CLK_DELAY)
            # chip read data on clk rising edge
            self._data_pin.value(byte & 0x01)
            byte >>= 1
            self._clk_pin.on()
            time.sleep_us(self.CLK_DELAY)

    def read_ram(self):
        """
        Read RAM as bytes
        :return: RAM dumps
        :rtype: bytearray
        """
        # start of message
        self._start_tx()
        # read ram burst
        self._w_byte(0xff)
        # read data bytes
        byte_a = bytearray()
        for _ in range(31):
            byte_a.append(self._r_byte())
        # end of message
        self._end_tx()
        return byte_a

    def write_ram(self, byte_a):
        """
        Write RAM with bytes
        :param byte_a: bytes to write
        :type byte_a: bytearray
        """
        # start message
        self._start_tx()
        # write ram burst
        self._w_byte(0xfe)
        # write data bytes
        for i in range(min(len(byte_a), 31)):
            self._w_byte(ord(byte_a[i:i + 1]))
        # end of message
        self._end_tx()

    def read_datetime(self):
        """
        Read current date and time from RTC chip.
        :return: date and time
        :rtype: datetime.datetime
        """
        # start message
        self._start_tx()
        # read clock burst
        self._w_byte(0xbf)
        byte_l = []
        for _ in range(7):
            byte_l.append(self._r_byte())
        # end of message
        self._end_tx()
        # decode bytes
        second = ((byte_l[0] & 0x70) >> 4) * 10 + (byte_l[0] & 0x0f)
        minute = ((byte_l[1] & 0x70) >> 4) * 10 + (byte_l[1] & 0x0f)
        hour = ((byte_l[2] & 0x30) >> 4) * 10 + (byte_l[2] & 0x0f)
        day = ((byte_l[3] & 0x30) >> 4) * 10 + (byte_l[3] & 0x0f)
        month = ((byte_l[4] & 0x10) >> 4) * 10 + (byte_l[4] & 0x0f)
        year = ((byte_l[6] & 0xf0) >> 4) * 10 + (byte_l[6] & 0x0f) + 2000
        # return datetime value
        #return datetime.datetime(year, month, day, hour, minute, second)
        return (year, month, day, hour, minute, second)


    def write_datetime(self, year, month, day, hour, minute, second, weekday = 1 ):
        """
        Write a python datetime to RTC chip.
        :param dt: datetime to write
        :type dt: datetime.datetime
        """
        # format message
        byte_l = [0] * 9
        byte_l[0] = (second // 10) << 4 | second % 10
        byte_l[1] = (minute // 10) << 4 | minute % 10
        byte_l[2] = (hour // 10) << 4 | hour % 10
        byte_l[3] = (day // 10) << 4 | day % 10
        byte_l[4] = (month // 10) << 4 | month % 10
        byte_l[5] = (weekday // 10) << 4 | weekday % 10
        byte_l[6] = ((year-2000) // 10) << 4 | (year-2000) % 10
        # start message
        self._start_tx()
        # write clock burst
        self._w_byte(0xbe)
        # write all data
        for byte in byte_l:
            self._w_byte(byte)
        # end of message
        self._end_tx()


spi = DS1302()
#spi.write_datetime(2022, 6, 9, 16, 21, 0, weekday = 4 )
time = spi.read_datetime() # get time
print(time)
