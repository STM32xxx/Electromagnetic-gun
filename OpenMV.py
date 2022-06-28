import sensor,image,time,pyb

from pyb import UART # 导入串口模块
uart = UART(3, 9600) # 初始化串口3，波特率9600
uart.init(9600, bits=8, parity=None, stop=1) # 8数据，1停止，无校验默认就行

from pyb import LED
green_led = LED(1)
blue_led = LED(2)

green_led.on() #开机亮绿灯

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((320, 240)) #设置分辨率
sensor.skip_frames(time = 2000) #延时2秒，等待摄像头稳定
sensor.set_auto_gain(False) #关闭自动增益
sensor.set_auto_whitebal(False) #关闭白平衡
clock = time.clock() #

K = 15000 #the value should be measured
thresholds = (0, 100, 31, 127, -128, 127) # thresholds for bright white light from IR.
#ROI=(37,6,100,100) #感兴趣区域

green_led.off()
blue_led.on() #打开蓝色运行指示灯

def find_max(blobs): #找到最大色块函数
    max_size=0
    for blob in blobs:
        if blob.pixels() > max_size:
            max_blob=blob
            max_size=blob.pixels()
    return max_blob

while(True):
    clock.tick()
    img = sensor.snapshot()
    #面积限制，感兴趣取余限制
    #blobs = img.find_blobs([thresholds], roi=ROI, pixels_threshold=50, area_threshold=50, merge=True)
    blobs = img.find_blobs([thresholds], pixels_threshold=300, area_threshold=300, merge=True)
    if blobs:
        max_b=find_max(blobs)
        ratio = max_b[2] / max_b[3] #形状限制
        if(ratio >= 0.5) and (ratio <= 1.5): #形状限制
            img.draw_rectangle(max_b.rect())
            img.draw_cross(max_b[5], max_b[6])
            Lm = (max_b[2]+max_b[3])/2
            length = (int)(K/Lm)
            print(max_b[5]-160,max_b[6]-120,length)
            uart.write('#'+'%c'%(max_b[5]>>8)+'%c'%(max_b[5]&0xff)+'%c'%(max_b[6]>>8)+'%c'%(max_b[6]&0xff)+'%c'%(length>>8)+'%c'%(length&0xff)+'*')
        else:
            print("none")
            uart.write('#'+'%c'%0xfe+"11111*") #没有检测到目标发送:#(0xfe)111*
    else:
        print("none")
        uart.write('#'+'%c'%0xfe+"11111*") #没有检测到目标发送:#(0xfe)111*
    img.draw_cross(160, 120, size=105, color=(255,0,0))
    print(clock.fps())
