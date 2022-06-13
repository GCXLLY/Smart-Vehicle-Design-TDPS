# Shape recognition - By: chenxiao - 周一 6月 13 2022
# In patio 2, task 1, use openmv to recognize square, triangle, and cycle
import sensor, image, time, math
from pyb import UART

# set-up camera

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)# 160x120
sensor.skip_frames(time = 2000)
sensor.set_contrast(3)
sensor.set_saturation(-3)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

#set-up UART

uart = UART(3,9600)
uart.init(9600,bits = 8, parity = None, stop = 1)

def shape_recognition():
    # preprocessing
    img = sensor.snapshot().lens_corr(strength = 1.8, zoom = 1.0)
    store = 0
    for b in img.find_blobs([(1, 90)], x_stride = 3, y_stride = 3, area_threshold = 200):

        x, y, w, h = b.rect()

        #filter blob

        x_rat = ((x+w/2) / img.width())
        img.draw_string(int(x), int(y), "%.3f" %x_rat, color = (0, 255, 0))
        if x_rat < 0.1 or x_rat > 0.8: continue

        if w * h > 5000: continue
        ratio = h/w
        if ratio > 1.2 or ratio < 0.8: continue

        #pixel
        pix_sum = 0

        for i in range(w):
            for j in range(h):
                pix = img.get_pixel(int(x)+i, int(y)+j) < 120
                pix_sum += pix

        pix_rate = pix_sum/(w*h)

        circles = img.find_circles(roi = (x-20,y-20,w+40,h+40), threshold = 2000, r_min = 20, r_max = 300, r_step = 2)

        if pix_rate < 0.8 and pix_rate > 0.7: #circle
            for c in circles:
                img.draw_circle(c.x(), c.y(), c.r(), color = (0,0,0))
            store = 2
            img.draw_string(int(x + w//2), int(y + h//2), "circle", color = (0, 255, 0))
        elif pix_rate > 0.45 and pix_rate < 0.6: # triangle
            store = 1
            img.draw_string(int(x + w//2), int(y + h//2), "triangle", color = (0, 255, 0))
        elif pix_rate > 0.8: #rectangle
            store = 3
            img.draw_string(int(x + w//2), int(y + h//2), "rectangle", color = (0, 255, 0))
        img.draw_rectangle(b.rect())
        img.draw_string(int(x), int(y), "%d\n%.3f\n" % (w*h, pix_rate), color = (0, 255, 0))
        #print(pix_rate,store)
    return store

while True:

    ##for i in range(300):
        ##shape_recognition()
    #while(1):
        #u = '2'
        #uart.write(u)

    u = '0'
    time.sleep(16)
    if not int(u):

        for j in range(4):
            ratio = 0
            count = 0
            for i in range(100):
                #uart.write(u)
                count += shape_recognition()
                #if not i//20: uart.write(u)
            ratio = count/100

            print(ratio)
            if ratio < 1.1 and ratio > 0.9:
                u = '2'
                pass #triangle
            elif ratio > 1.9 and ratio < 2.1:
                u = '1'
                pass #circle
            elif ratio > 2.8 and ratio <= 3.0:
                u = '3'
                pass #rectangle
            else:
                u = '0'

            print(u)
        if u == '0': u = '1'
    while int(u):
        uart.write(u)
        print(u)
