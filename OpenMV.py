# Code for OPENMV

# red light find & send
# send four corner x & y in while(1)
def color_blob(threshold):
    blobs = img.find_blobs(threshold,x_stride=1, y_stride=1, area_threshold=0, pixels_threshold=0,merge=False,margin=1)
    if len(blobs)>=1 :# some block
        # Draw a rect around the blob.
        b = blobs[0]
        # img.draw_rectangle(b[0:4]) # rect
        cx = b[5]
        cy = b[6]
        for i in range(len(blobs)-1):
            #img.draw_rectangle(b[0:4]) # rect
            cx = blobs[i][5]+cx
            cy = blobs[i][6]+cy
        cx=int(cx/len(blobs))
        cy=int(cy/len(blobs))
        #img.draw_cross(cx, cy) # cx, cy
        print(cx,cy)
        return int(cx), int(cy)
    return -1, -1 # not find

# some basic
import sensor, image, time, math
from pyb import UART

# UART
uart = UART(3,115200,timeout_char = 1000)
uart.init(115200,bits = 8,parity = None,stop = 1,timeout_char = 1000)
u_start = bytearray([0xb3,0xb3])
u_over = bytearray([0x0d,0x0a])
uart_buf = [0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0
            ]
# 8 low for data & 8 high for data & 1 for status

# basic
dymac = 0
#threshold_red=[(60, 255, -20, 20, -20, 20)] # threshold for red light
#threshold_red = [(26, 100, 11, 127, -128, 127)]
threshold_red = [(21, 100, 15, 127, -128, 127)]

findx = 0
findy = 0
reset1 = 0

# First Stage Black Rectangle

while(True):
    # 发送外框坐标
    reset1 = 0
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QQVGA)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(False)

    sensor.set_auto_exposure(False, 500000)

    sensor.skip_frames(time = 300)
    clock = time.clock()
    while(True):
        #break
        clock.tick()
        if uart.any():
            a = uart.read(1)
            print(a)
            if a == b'\x05':
                break
        img = sensor.snapshot()
        for r in img.find_rects(threshold = 13000):
            img.draw_rectangle(r.rect(), color = (255, 0, 0))
            for p in r.corners(): img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))
            print(r)
            if r.magnitude()>130000 or r.magnitude()<7000 or r.corners()[0][0]<-10 or r.corners()[3][0]<-10 or r.corners()[1][0]<-10 or r.corners()[2][0]<-10:
                continue
            if r.corners()[3][0]>145 or r.corners()[2][0]>145 or r.corners()[1][0]>145 or r.corners()[0][0]>145:
                continue
            if r.corners()[0][1]>120 or  r.corners()[1][1]>120 or r.corners()[2][1]>120 or r.corners()[3][1]>120:
                continue
            if r.corners()[0][1]<-5 or  r.corners()[1][1]<-5 or r.corners()[2][1]<-5 or r.corners()[3][1]<-5:
                continue
            print(r.corners())
            for i in range(8):
                dymac = int(r.corners()[int(i/2)][i%2])
                if dymac >= 256:
                    uart_buf[i+8] = 1
                else:
                    uart_buf[i+8] = 0
                uart_buf[i] = dymac
            #for i in range(16):
                #print(uart_buf[i])
            # openmv status
            uart_buf[16] = 1
            uart_buf = bytearray(uart_buf)
            uart.write(u_start)	# head data end
            uart.write(uart_buf)
            uart.write(u_over)
        print("FPS %f" % clock.fps())
    # send iner data
    #sensor.reset()
    #sensor.set_pixformat(sensor.RGB565)
    #sensor.set_framesize(sensor.QQVGA)
    #sensor.set_auto_gain(False)
    #sensor.set_auto_whitebal(False)
    
    sensor.set_auto_exposure(False, 400000)

    sensor.skip_frames(time = 500)
    clock = time.clock()
    while(True):
        #break
        if reset1 == 1:
            break
        clock.tick()
        if uart.any():
            a = uart.read(1)
            print(a)
            if a == b'\x07':
                break
            if a == b'\x09':
                reset1 = 1
        img = sensor.snapshot().invert()
        for r in img.find_rects(threshold = 10000):
            img.draw_rectangle(r.rect(), color = (255, 0, 0))
            for p in r.corners(): img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))
            print(r)
            if r.magnitude()>90000 or r.magnitude()<10000:
                continue
            if r.corners()[0][0]<-10 or r.corners()[3][0]<-10 or r.corners()[1][0]<-10 or r.corners()[2][0]<-10:
                continue
            if r.corners()[3][0]>145 or r.corners()[2][0]>145 or r.corners()[1][0]>145 or r.corners()[0][0]>145:
                continue
            if r.corners()[0][1]>120 or  r.corners()[1][1]>120 or r.corners()[2][1]>120 or r.corners()[3][1]>120:
                continue
            if r.corners()[0][1]<-5 or  r.corners()[1][1]<-5 or r.corners()[2][1]<-5 or r.corners()[3][1]<-5:
                continue
            print(r.corners())
            for i in range(8):
                dymac = int(r.corners()[int(i/2)][i%2])
                if dymac >= 256:
                    uart_buf[i+8] = 1
                else:
                    uart_buf[i+8] = 0
                uart_buf[i] = dymac
            #for i in range(16):
                #print(uart_buf[i])
            # openmv status
            uart_buf[16] = 2
            uart_buf = bytearray(uart_buf)
            uart.write(u_start)	 # head data end
            uart.write(uart_buf)
            uart.write(u_over)
        print("FPS %f" % clock.fps())

    # Red Light Find And Send
    uart_buf[16] = 0
    print("light")
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565) # or sensor.RGB565
    sensor.set_framesize(sensor.QQVGA) # or sensor.QVGA (or others)
    sensor.skip_frames(time=900) # Let new settings take affect.
    sensor.set_auto_exposure(False, 15000)
    sensor.set_auto_whitebal(False)
    sensor.set_auto_gain(False)
    while(True):
        if reset1 == 1:
            break
        clock.tick()
        if uart.any():
            a = uart.read(1)
            b = uart.read(1)
            if a == b'\x09':
                reset1 = 1
        img = sensor.snapshot()
        findx,findy = color_blob(threshold_red)
        if findx > 0 and findy > 0:
            for i in range(17):
                uart_buf[i] = 0
            uart_buf[0] = findx
            uart_buf[1] = findy
            print(uart_buf[0])
            print(uart_buf[1])
            if findx >= 256:
                uart_buf[8] = 1
            if findy >= 256:
                uart_buf[9] = 1
            uart_buf[16] = 3
            uart_buf = bytearray(uart_buf)
            uart.write(u_start)
            uart.write(uart_buf)
            uart.write(u_over)
        print("FPS %f" % clock.fps())
