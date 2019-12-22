# -*- coding: utf-8 -*

import serial
import serial.tools.list_ports as lp
import cv2
import numpy as np
import matplotlib.pyplot as plt
import time

MODE = "QQVGA"
if MODE == "QVGA":
    H = 240
    W = 320
elif MODE == "QQVGA":
    H = 120
    W = 160
elif MODE == "QCIF":
    H = 144
    W = 176

def list_com_ports():
    port_list = list(lp.comports())
    if len(port_list) == 0:
        print('No COM is found!')
    else:
        for i in port_list:
            print(i)


def receive_bytes(ser):
    print("Start logging...")
    buff_bytes = ser.read_until()  # 读取内容直到出现回车
    # buff_bytes = ser.read_all()  # 读取内容
    # buff_bytes = ser.read(size=240*320*6)  # 读取内容
    # 清空接收缓冲区
    # ser.flushInput()
    # 必要的软件延时
    time.sleep(0.05)
    return buff_bytes


def bytes2image(recv_bytes):
    # b = b'19,8531,4331,6441,6439,4439,6531,4441,'
    buff_str = ""
    buff_str += recv_bytes.decode("utf-8")
    # buff_str += b.decode("utf-8")
    buff_str = buff_str[0:-2]  # ignore last (,) & (\n)
    # print("recv_bytes buff_str:")
    # print(buff_str)
    buff_list = buff_str.split(',')
    # print(buff_list)
    buff_list_hex = [int(i, base=16) for i in buff_list]

    raw_data_rgb = [(i & 0xFF00) >> 8 for i in buff_list_hex]
    np_data = np.array(raw_data_rgb).astype('uint8')
    print("np_data:")
    print(np_data)
    print("np_data shape:", np_data.shape)

    # display_data = np_data.reshape(224, 224, 3)
    display_data = np_data.reshape(H, W)
    display_data = np.flip(display_data, [0, 1])  # [0:240) to [240:0)
    # print(display_data[0])
    # print(display_data.dtype)
    plt.imshow(display_data)
    plt.show()

    # gray_image = cv2.cvtColor(display_data, cv2.COLOR_BGR2GRAY)
    # np.save("ov7670_sample", gray_image)
    # # print(gray_image)
    # plt.imshow(gray_image, cmap='Greys')
    # plt.show()


if __name__ == '__main__':
    ser_parms = dict(port='COM6',
                     baudrate=256000,
                     bytesize=8,
                     parity='N',
                     stopbits=1,
                     timeout=None,
                     xonxoff=False,
                     rtscts=False,
                     dsrdtr=False)

    ser = serial.Serial(**ser_parms)
    print(ser)

    try:
        recv_bytes = receive_bytes(ser)
        # print("recv_bytes:", recv_bytes)
        bytes2image(recv_bytes)
        ser.close()
    except KeyboardInterrupt:
        if ser is not None:
            ser.close()
