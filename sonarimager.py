import serial
import time
import numpy as np
from PIL import Image
from datetime import datetime as dt

ser = serial.Serial(
    port='/dev/tty.usbserial-1440',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
    #timeout=1
    )

time.sleep(5)
ser.write(b's')

#x:y:d\r\n

#image should be: 75 points y, 90 points x

def mapRange(value, inMin, inMax, outMin, outMax):
    return outMin + (((value - inMin) / (inMax - inMin)) * (outMax - outMin))

count = 0 #6750

im = Image.new(mode="RGB", size=(90, 75))

print("scanning... please be patient")
print("you can check progress.jpg in the meantime")

run = True
while run:

    inp=ser.readline()
    #print(inp.decode())
    dat = inp.decode().split(":")
    # x = dat(0), y = dat(1), val = dat(2)
    intensity = int(float(dat[2].replace('\r\n','')))
    x = int(dat[0])-46
    y = int(dat[1])-60
    
    color=(0,0,255)

    if(intensity > 400):
        intensity = 400
    intMapped = int(mapRange(intensity, 0, 400, 0, 255))
    intMappedInverted = 255-intMapped
    

    try:
        im.putpixel((x,y), (intMapped,0,0))
    except:
        print("error!")
        print("x:"+str(x)+" y:"+str(y)+"intensity:"+str(intMapped))
        
    count = count + 1

    if(count % 100 == 0):
        im2 = im.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
        im2 = im2.transpose(Image.Transpose.FLIP_LEFT_RIGHT)
        im2 = im2.save("/Users/mariussuflea/Documents/soonarv2/progress.jpg")

    if(count == 6750):
        count = 0
        im2 = im.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
        im2 = im2.transpose(Image.Transpose.FLIP_LEFT_RIGHT)
        #im2.show()
        d = im2.resize((90*10,75*10), resample=Image.Resampling.BOX)
        d.show()
        now = dt.now()
        dtitle = "/Users/mariussuflea/Documents/soonarv2/scan" + now.strftime("%Y%m%d%H%M") + ".jpg"
        d = d.save(str(dtitle))
        

        print("scanning done!")


    
