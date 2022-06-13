# Line tracking - By: chenxiao - 周二 6月 7 2022

# In atio 1, task 1, use openmv to do line tracking


import sensor, image, time, math
import ustruct
from pid import PID
from pyb import UART

theta_pid = PID(p=0.45, i=0.2)
# initializes the PI controller and envalues the parameters

GRAYSCALE_THRESHOLD = [(128, 255)]
sensor.reset() # resets and initializes the sensor.
sensor.set_pixformat(sensor.RGB565) # sets pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA) # sets frame size to QVGA (320x240)
sensor.skip_frames(time = 2000) # waits for settings take effect.
clock = time.clock() # creates a clock object to track the FPS.

# set uart
uart = UART(3, 9600)
uart.init(9600,bits=8,parity=None,stop=1)

ROIS = [
(10, 50, 300, 30 ,0.15),
(10, 100, 300, 30, 0.15),
(10, 150, 300, 30, 0.3),
(10, 200, 300, 30, 0.4)
]
# defines four regions of interest

weight_sum = 0 # initializes weight values
for r in ROIS: weight_sum += r[4] # defines calculation for weigted

thresh=[(30, 79, -7, 30, 6, 23)]
clock.tick() # initiates timer

while(True):
    # updates the FPS clock.
    img = sensor.snapshot() # takes a picture and return the image
   # img.rotation_corr(z_rotation=180) # rectify the image because it is shot upside down
    img = img.to_grayscale() # converts to grayscale image
    img = img.find_edges(image.EDGE_CANNY, threshold=(140, 140))
    # one−step realization of Canny detection
    img = img.dilate(1)
    # makes white spots more present

    centroid_sum = 0
    # initializes the weighted distance from centroids to the central line
    for r in ROIS: # for each centroid of ROI
        blobs = img.find_blobs(GRAYSCALE_THRESHOLD , roi=r[0:4], merge=True)
        # r[0:4] is roi tuple.
        # finds all white spots and merge as one (connecting each other with lines)

        # searching for lines in ROI
        if blobs:
            # indentifies the bolb index with the most lines
            largest_blob = 0
            most_pixels = 0
            for i in range(len(blobs)):
            # there might be several line clusters in ROI, but identify the larges as the target line
                if blobs[i].pixels() > most_pixels:
                    most_pixels = blobs[i].pixels()
                    # update the value if there is any larger pixel clusters
                    largest_blob = i

            img.draw_rectangle(blobs[largest_blob].rect(),color = 0)
            # draws a rectangle around the largest pixel cluster
            img.draw_cross(blobs[largest_blob].cx(), blobs[largest_blob].cy(),color = 0)
            # marks the largest pixel cluster with a white cross

            centroid_sum += blobs[largest_blob].cx() *r[4]
            # accumulates the weighted distances to obtain the final result

    center_pos = (centroid_sum / weight_sum) # determines the center of line

    # converts the result above to an angle, using unlinear operations
    deflection_angle = 0
    # initializes the angle to turn for the robot
    deflection_angle = - math.atan((center_pos -160)/60)
    # obtains the result (in radian)
    deflection_angle = math.degrees(deflection_angle)
    # converts result to degrees

    output = theta_pid.get_pid(deflection_angle ,1)

    # generate instructions in different level
    if output<-4: # turn right
        if output>-10:
            sign = '4'
        elif output>-15:
            sign = '5'
        else:
            sign = '6'

    elif output>4: # turn left
        if output<10:
            sign = '3'
        elif output<15:
            sign = '2'
        else:
            sign = '1'
    else: # go straight
        sign = '0'
    #sign = str(sign)

    sign='1'
    print(sign)
    # write data and send
    uart.write(sign)

