import struct
from tkinter.filedialog import *
import matplotlib.pyplot as plt
import numpy as np
from scipy import misc
from scipy import signal
from scipy import ndimage
import traceback
import colorsys as cs
from PIL import Image, ImageFilter, ImageEnhance, ImageOps
import socket
import time
import math
import io



#########################################
##############  상 수 선 언 ###############
#########################################

inH, inW, outH, outW = [0] * 4

sx = 0 ; sy  = 0 ; ey  = 0 ; ex = 0
R  = 0 ; G   = 1 ; B   = 2
H  = 0 ; S   = 1 ; V   = 2

RECV_TCP_IP = 'localhost'
RECV_TCP_PORT = 1234



#######################################
############ LOOKUP TABLES ############
#######################################

LUT = np.zeros(256)
for i in range(255):
    LUT[i] = int(255 - 255 * math.pow(i / 128 - 1, 2))


#######################################
######### Convolution Masks ###########
#######################################

EMBOSS_MASK = [ [-1, 0, 0],
                [ 0, 0, 0],
                [ 0, 0, 1]  ]



#########################################
########### 기 반 함 수 선 언 ##############
#########################################

def malloc(w, h):
    memory = np.zeros((3, h * w), dtype = np.uint8)
    return memory


def malloc3d(w, h):
    memory = np.zeros((3, h , w), dtype = np.uint8)
    return memory


def check_Wvalid(loc_pix):
    global DRIVER_NAME, FILE_PATH, VALUE, LUT, inImage, outImage, b_img, inH, inW, outW, outH
    return 0 <= loc_pix <= outW

def check_Hvalid(loc_pix):
    global DRIVER_NAME, FILE_PATH, VALUE, LUT, inImage, outImage, b_img, inH, inW, outW, outH
    return 0 <= loc_pix <= outH


def rotateCalc(x, y, theta = 90, reverse = False):
    global DRIVER_NAME, FILE_PATH, VALUE, LUT, inImage, outImage, b_img, inH, inW, outW, outH

    Cx = inH // 2
    Cy = inW // 2

    if reverse:

        xNew = math.cos(math.radians(theta)) * (x - Cx) + math.sin(math.radians(theta)) * (y - Cy) + Cx
        yNew = -math.sin(math.radians(theta)) * (x - Cx) + math.cos(math.radians(theta)) * (y - Cy) + Cy

    else:
        xNew = math.cos(math.radians(theta)) * (x - Cx) - math.sin(math.radians(theta)) * (y- Cy) + Cx
        yNew = math.sin(math.radians(theta)) * (x - Cx) + math.cos(math.radians(theta)) * (y- Cy) + Cy

    if xNew > Cx * 2 - 1:
        xNew = Cx * 2 - 1

    if yNew > Cy * 2 - 1:
        yNew = Cy * 2 - 1

    return xNew, yNew


def Calcu_k( rotate_xyz = (30, 0, 0), size_xy = (256, 256), scale_xy = (1, 1), trans_xyz = ( 0.2 , 0, 0 )):
    global DRIVER_NAME, FILE_PATH, VALUE, LUT, inImage, outImage, b_img, inH, inW, outW, outH
    global size_x, size_y


    k = [0] * 9

    rotate_x = rotate_xyz[0]
    rotate_y = rotate_xyz[1]
    rotate_z = rotate_xyz[2]

    size_x = size_xy[0] // 2  # inW
    size_y = size_xy[1] // 2  # inH

    scale_x = scale_xy[0]
    scale_y = scale_xy[1]

    trans_x = trans_xyz[0]
    trans_Y = trans_xyz[1]
    trans_Z = trans_xyz[2]

    x_cos = math.cos(rotate_x * math.pi / 180.0)
    x_sin = math.sin(rotate_x * math.pi / 180.0)

    y_cos = math.cos(rotate_y * math.pi / 180.0)
    y_sin = math.sin(rotate_y * math.pi / 180.0)

    z_cos = math.cos(rotate_z * math.pi / 180.0)
    z_sin = math.sin(rotate_z * math.pi / 180.0)

    eye_pos = 5
    screen_pos = 5

    l = np.zeros((4, 4), dtype=np.float64)
    m = np.zeros((4, 4), dtype=np.float64)

    l[0, 0] = 1.0 / size_x
    l[1, 1] = 1.0 / -size_y
    l[2, 2] = 1
    l[3, 3] = 1

    m[0, 0] = scale_x
    m[1, 1] = scale_y
    m[2, 2] = 1
    m[3, 3] = 1

    n = l.dot(m)

    l.fill(0)
    l[0, 0] = 1
    l[1, 1] = 1
    l[2, 2] = 1
    l[3, 3] = 1

    l[3, 0] = trans_x
    l[3, 1] = trans_Y
    l[3, 2] = trans_Z

    m = n.dot(l)
    n.fill(0)

    n[0, 0] = z_cos
    n[1, 0] = -z_sin
    n[0, 1] = z_sin
    n[1, 1] = z_cos
    n[2, 2] = 1
    n[3, 3] = 1

    l = m.dot(n)
    m.fill(0)

    m[0, 0] = 1
    m[1, 1] = x_cos
    m[2, 1] = -x_sin
    m[1, 2] = x_sin
    m[2, 2] = x_cos
    m[3, 3] = 1

    n = l.dot(m)
    l.fill(0)

    l[0, 0] = y_cos
    l[2, 0] = -y_sin
    l[1, 1] = 1
    l[0, 2] = y_sin
    l[2, 2] = y_cos
    l[3, 3] = 1

    m = n.dot(l)
    n.fill(0)

    n[0, 0] = 1
    n[1, 1] = 1
    n[2, 2] = -1
    n[3, 2] = eye_pos
    n[3, 3] = 1

    l = m.dot(n)
    m.fill(0)

    m[0, 0] = 1
    m[1, 1] = 1
    m[2, 2] = 1 / screen_pos
    m[3, 2] = -1
    m[2, 3] = 1 / screen_pos

    n = l.dot(m)
    l.fill(0)

    l[0, 0] = size_x
    l[1, 1] = -size_y
    l[2, 2] = 1
    l[3, 3] = 1

    m = n.dot(l)

    K1 = m[0, 3]
    K2 = m[1, 3]
    K3 = m[3, 3]
    K4 = m[0, 0]
    K5 = m[1, 0]
    K6 = m[3, 0]
    K7 = m[0, 1]
    K8 = m[1, 1]
    K9 = m[3, 1]

    k[0] = K7 * K2 - K8 * K1
    k[1] = K5 * K1 - K4 * K2
    k[2] = K4 * K8 - K7 * K5
    k[3] = K8 * K3 - K9 * K2
    k[4] = K6 * K2 - K5 * K3
    k[5] = K5 * K9 - K8 * K6
    k[6] = K9 * K1 - K7 * K3
    k[7] = K4 * K3 - K6 * K1
    k[8] = K7 * K6 - K4 * K9

    print("Calcu_k END..")
    return k


#########################################
#########  파일 관련 함 수 선 언 ############
#########################################

def loadImage(width, height):
    global DRIVER_NAME, FILE_PATH, inImage, outImage, b_img, inH, inW

    inW   = int(width)
    inH   = int(height)
    photo = Image.frombytes('RGB',( inH, inW ), b_img, 'raw' )


    inImage = malloc( inH, inW  )

    R, G, B = photo.split()

    inImage[0] = np.frombuffer(R.tobytes(), dtype = np.uint8)
    inImage[1] = np.frombuffer(G.tobytes(), dtype = np.uint8)
    inImage[2] = np.frombuffer(B.tobytes(), dtype = np.uint8)





#########################################
########### 영상처리 함 수 선 언  ###########
#########################################

def equalImage():
    global DRIVER_NAME, FILE_PATH, VALUE , LUT, inImage, outImage, b_img ,inH, inW, outW, outH

    ## 출력영상 크기 결정 ##
    outW = inW;
    outH = inH;

    outImage = inImage
    sendImage()


def addImage():
    global DRIVER_NAME, FILE_PATH, VALUE , LUT, inImage, outImage, b_img ,inH, inW, outW, outH

    ## 중요! 코드. 출력영상 크기 결정 ##
    outH = inH;
    outW = inW;

    inImage  = inImage.astype(np.int16)

    outImage = inImage + int(VALUE)

    outImage = np.where(outImage > 255, 255, outImage)
    outImage = np.where(outImage < 0, 0, outImage)

    outImage = outImage.astype(np.uint8)

    sendImage()


def reverseImage():
    global DRIVER_NAME, FILE_PATH, VALUE , LUT, inImage, outImage, b_img ,inH, inW, outW, outH

    ## 출력영상 크기 결정 ##
    outH = inH;
    outW = inW;

    ####### 알고리즘 #####

    outImage = 255 - inImage
    sendImage()


def binImage():
    global DRIVER_NAME, FILE_PATH, VALUE , LUT, inImage, outImage, b_img ,inH, inW, outW, outH

    ## 출력영상 크기 결정 ##
    outH = inH;
    outW = inW;

    ## 사전 작업 ##

    outImage = malloc(outH, outW)

    inImage   = np.reshape(inImage  , ( 3, inW, inH    ))
    outImage  = np.reshape(outImage , ( 3, inW, inH    ))

    tmpImage  = np.zeros((outH, outW))

    ## 알고리즘 ##


    for rgb in range(3):
        for i in range(outH):
            for k in range(outW):
                tmpImage[i][k] += inImage[rgb][i][k] // 3

    tmpImage = np.where(tmpImage > VALUE, 255,  0)


    for rgb in range(3):
        for i in range(outH):
            for k in range(outW):
                outImage[rgb][i][k] = tmpImage[i][k]


    sendImage()



def rotateImage():
    global DRIVER_NAME, FILE_PATH, VALUE, LUT, inImage, outImage, b_img, inH, inW, outW, outH

    ## 출력영상 크기 결정 ##
    outW = inW;
    outH = inH;

    ####### 알고리즘 #####

    outImage = malloc(outW, outH)

    inImage  = np.reshape(inImage , (3, inH ,inW ))
    outImage = np.reshape(outImage, (3 ,outH ,outW ))

    for RGB in range(3):
        for i in range(outH):
            for k in range(outW):
                newI, newK = map(int, rotateCalc(i, k, VALUE))


                if check_Wvalid(newI) and check_Hvalid(newK):
                    outImage[RGB][newI][newK] = inImage[RGB][i][k]

    sendImage()

# 파라볼라 with 룩업 테이블
def paraImage():
    global DRIVER_NAME, FILE_PATH, VALUE, LUT, inImage, outImage, b_img, inH, inW, outW, outH

    outImage = np.array(   [ LUT[i]    for i in inImage ] ,dtype = np.uint8)

    sendImage()


def zoomout():
    global DRIVER_NAME, FILE_PATH, VALUE, LUT, inImage, outImage, b_img, inH, inW, outW, outH

    VALUE = 3;

    outH = inH
    outW = inW

    Cx = inW // 4
    Cy = inH // 4


    inImage  = np.reshape(inImage, ( 3, inH, inW ))
    outImage = np.zeros( (3 ,inH, inW), dtype = np.uint8 )

    for rgb in range(3):
        for i in range( int(inH // VALUE) ):
            for k in range( int(inW // VALUE) ):

                xFrom = VALUE *  i
                xTo   = VALUE * (i + 1)
                yFrom = VALUE *  k
                yTo   = VALUE * (k + 1)

                outImage[rgb][i + Cy][k + Cx] = inImage[rgb: rgb + 1 , xFrom :  xTo , yFrom : yTo ].mean()

    sendImage()


def zoomin():
    global DRIVER_NAME, FILE_PATH, VALUE, LUT, inImage, outImage, b_img, inH, inW, outW, outH

    VALUE = 2

    outH = inH ;
    outW = inW ;

    outImage = np.zeros( (3 ,inH, inW), dtype = np.uint8 )
    inImage  = np.reshape( inImage , ((3 ,inH, inW)))

    rH, rW, iH, iW = [0] * 4  # 실수 위치 및 정수 위치
    x, y = [0] * 2  # 실수와 정수의 차이값
    C1, C2, C3, C4 = [0] * 4  # 결정할 위치(n)의 상하 좌우 픽셀

    for rgb in range(3):
        for i in range(outH):
            for k in range(outH):

                rH = i / VALUE
                rW = k / VALUE
                iH = int(rH)
                iW = int(rW)

                x = rW - iW
                y = rH - iH

                if 0 < iH < inH - 1 and 0 <= iW < inW - 1:

                    C1 = inImage[rgb][iH][iW]
                    C2 = inImage[rgb][iH][iW + 1]
                    C3 = inImage[rgb][iH + 1][iW + 1]
                    C4 = inImage[rgb][iH + 1][iW]

                    newValue = C1 * (1 - y) * (1 - x) + C2 * (1 - y) * x + C3 * y * x + C4 * (1 - x) * y
                    outImage[rgb][i][k] = int(newValue)

    sendImage()


def histoImage():
    global DRIVER_NAME, FILE_PATH, VALUE, LUT, inImage, outImage, b_img, inH, inW, outW, outH

    inImage = np.reshape(inImage, ((3, inH, inW)))
    outImage = malloc(inH, inW)

    incountList = [0] * 3

    for i in range(3):
        incountList[i] = [0] * 256

    for rgb in range(3):
        for i in range(inH):
            for k in range(inW):
                incountList[rgb][inImage[rgb][i][k]] += 1

    plt.figure()
    plt.plot(incountList[0])
    plt.plot(incountList[1])
    plt.plot(incountList[2])
    plt.title("Histogram")

    buf = io.BytesIO()

    plt.savefig(buf, format = 'png')
    buf.seek(0)

    hist = Image.open(buf,"r")
    hist = hist.resize((512, 512))

    R, G, B, A = hist.split()

    outImage[0] = np.frombuffer(R.tobytes(), dtype = np.uint8)
    outImage[1] = np.frombuffer(G.tobytes(), dtype = np.uint8)
    outImage[2] = np.frombuffer(B.tobytes(), dtype = np.uint8)

    sendImage()


# def homography():
#     global DRIVER_NAME, FILE_PATH, VALUE, LUT, inImage, outImage, b_img, inH, inW, outW, outH
#
#     ## 출력영상 크기 결정 ##
#     outH = inH;
#     outW = inW;
#
#
#
#     outImage = np.zeros((3, outH, outW), dtype = np.uint8)
#     inImage  = np.reshape(inImage , ((3, inH, inW)))
#
#
#     k = Calcu_k()
#
#     for rgb in range(3):
#         for i in range(outH):
#             for j in range(outW):
#
#                 w      = k[0] * j + k[1] * i + k[2]
#                 homo_x = k[3] * j + k[4] * i + k[5]
#                 homo_y = k[6] * j + k[7] * i + k[8]
#
#
#                 real_x = homo_x / w
#                 real_y = homo_y / w
#
#                 if(0 <= real_y <= 255 and 0 <= real_x <= 255):
#                     outImage[rgb][i + 150][j + 150] = inImage[rgb][int(real_y)][ int(real_x)]
#
#
#     sendImage()

def projection_matrix(x, _x, y, _y):
    a = []
    for i in range(8):
        a.append([0] * 8)

    b = [0] * 8

    a[0][0] = x[0];
    a[0][1] = y[0];
    a[0][2] = 1.0;
    a[0][6] = -1 * _x[0] * x[0];
    a[0][7] = -1 * _x[0] * y[0];

    a[1][0] = x[1];
    a[1][1] = y[1];
    a[1][2] = 1.0;
    a[1][6] = -1 * _x[1] * x[1];
    a[1][7] = -1 * _x[1] * y[1];

    a[2][0] = x[2];
    a[2][1] = y[2];
    a[2][2] = 1.0;
    a[2][6] = -1 * _x[2] * x[2];
    a[2][7] = -1 * _x[2] * y[2];

    a[3][0] = x[3];
    a[3][1] = y[3];
    a[3][2] = 1.0;
    a[3][6] = -1 * _x[3] * x[3];
    a[3][7] = -1 * _x[3] * y[3];

    a[4][3] = x[0];
    a[4][4] = y[0];
    a[4][5] = 1.0;
    a[4][6] = -1 * x[0] * _y[0];
    a[4][7] = -1 * y[0] * _y[0];

    a[5][3] = x[1];
    a[5][4] = y[1];
    a[5][5] = 1.0;
    a[5][6] = -1 * x[1] * _y[1];
    a[5][7] = -1 * y[1] * _y[1];

    a[6][3] = x[2];
    a[6][4] = y[2];
    a[6][5] = 1.0;
    a[6][6] = -1 * x[2] * _y[2];
    a[6][7] = -1 * y[2] * _y[2];

    a[7][3] = x[3];
    a[7][4] = y[3];
    a[7][5] = 1.0;
    a[7][6] = -1 * x[3] * _y[3];
    a[7][7] = -1 * y[3] * _y[3];

    b[0] = _x[0];
    b[1] = _x[1];
    b[2] = _x[2];
    b[3] = _x[3];
    b[4] = _y[0];
    b[5] = _y[1];
    b[6] = _y[2];
    b[7] = _y[3];

    a_inv = np.linalg.pinv(a)

    a_inv = np.array(a_inv, dtype = np.double).reshape((8,8))
    b = np.array(b, dtype = np.double).reshape((8,1))

    c = a_inv.dot(b)

    print(c)

    projection = [ [0] * 3  for _ in range(3) ]



    projection[0][1] = c[1];
    projection[0][0] = c[0];
    projection[0][2] = c[2];

    projection[1][0] = c[3];
    projection[1][1] = c[4];
    projection[1][2] = c[5];

    projection[2][0] = c[6];
    projection[2][1] = c[7];
    projection[2][2] = 1.0;

    return projection

def homography():
    global DRIVER_NAME, FILE_PATH, VALUE, LUT, inImage, outImage, b_img, inH, inW, outW, outH
    global R, G, V, H, S, V, sx , ex, sy, ey

    outH = inH
    outW = inW

    inImage = np.reshape(inImage, (3, inH, inW))
    outImage   = malloc3d(outH, outW )

    print(sx, ex, sy, ey)
    print(outImage.shape)

    # sx = [   227,   309, 360, 398]
    # sy = [   118,   512,  58, 512]


    # sx = [   0,   0, 512, 512]
    # sy = [   0, 512,   0, 512]
    #
    #
    ex = np.array(ex)
    ey = np.array(ey)


    projection = projection_matrix( ex, sx, ey, sy)
    projection = np.array(projection)


    p = projection

    for rgb in range(3):
        for j in range( ey.min(), ey.max() ):
            for i in range(ex.min(), ex.max()):

                w = p[2][0] * i + p[2][1] * j + p[2][2];

                real_x = int((p[0][0] * i + p[0][1] * j + p[0][2]) / w)
                real_y = int((p[1][0] * i + p[1][1] * j + p[1][2]) / w)

                if 0 <= real_x <= 511 and 0 <= real_y <= 511:
                    outImage[rgb][j][i] = inImage[rgb][real_y][real_x]



    sendImage()




def valueConvImage(mask):
    global DRIVER_NAME, FILE_PATH, VALUE, LUT, inImage, outImage, b_img, inH, inW, outW, outH
    global R, G, V, H, S, V

    ## 출력영상 크기 결정 ##
    outH = inH;
    outW = inW;

    inImageHSV = malloc3d(outH, outW )
    outImage   = malloc3d(outH, outW )

    Rimg = Image.fromarray(inImage[R].reshape((inW, inH)))
    Gimg = Image.fromarray(inImage[G].reshape((inW, inH)))
    Bimg = Image.fromarray(inImage[B].reshape((inW, inH)))

    inImageHSV[H], inImageHSV[S], inImageHSV[V] = Image.merge("RGB", (Rimg, Gimg, Bimg)).convert("HSV").split()

    inImageHSV    = inImageHSV.astype(np.double)
    inImageHSV[V] = (ndimage.convolve(inImageHSV[V], mask))

    inImageHSV[V] += 127

    inImageHSV[V] = np.where(inImageHSV[V] > 255, 255, inImageHSV[V]  )
    inImageHSV[V] = np.where(inImageHSV[V] < 0  ,   0, inImageHSV[V]  )
    inImageHSV    = inImageHSV.astype(np.uint8)

    Himg = Image.fromarray(inImageHSV[H])
    Simg = Image.fromarray(inImageHSV[S])
    Vimg = Image.fromarray(inImageHSV[V])

    outImage[R], outImage[G], outImage[B] = Image.merge("HSV", (Himg, Simg, Vimg)).convert("RGB").split()


    sendImage()





######################################
########### 통신 관련 함수 ##############
######################################
def recvall(sock, count):
    buf = b''

    databuf = np.array(count, dtype = np.uint8 )
    while count:
        newbuf = sock.recv(count)

        if not newbuf: return None
        buf += newbuf
        count -= len(newbuf)

    return buf


def loadRois(POINTS):
    global sx, ex, sy, ey

    p = POINTS.split("-")
    p = list(filter(lambda x: x != "", p))
    p = list(map(int, p))

    sx = [ p[0] ,p[1] ,p[2]  ,p[3]  ]
    ex = [ p[4] ,p[5] ,p[6]  ,p[7]  ]
    sy = [ p[8] ,p[9] ,p[10] ,p[11] ]
    ey = [ p[12],p[13],p[14] ,p[15] ]





def recvAndLoad():
    global DRIVER_NAME, FILE_PATH, VALUE, LUT, inImage, outImage, b_img, inH, inW, outW, outH
    global R, G, V, H, S, V, sx, ex, sy, ey

    sx = ex = sy = ey = 0
    i_totLength = b_totData = b_meta = list_meta = 0

    # 전체 데이터 크기 읽기 4바이트
    i_totLength = int.from_bytes(recvall(conn, 4), byteorder='big')

    print("TOTAL LENGTH : ", i_totLength)

    # 4바이트 용량제외하고 모두다 읽기
    b_totData = recvall(conn, i_totLength - 4)

    # 첫 200바이트는 메타데이터
    b_meta = b_totData[0: 200]

    list_meta = str(b_meta).split(":")

    DRIVER_NAME = list_meta[0]
    FILE_PATH   = list_meta[1]
    MODE        = list_meta[2]
    OPTION      = list(filter(lambda x: x != "", list_meta[3].split("|")))

    OPTION.extend( [''] * 3 );

    WIDTH       = OPTION[0]
    HEIGHT      = OPTION[1]
    COMPRESS    = OPTION[2]


    VALUE       = int( OPTION[3] ) if OPTION[3].isdigit() else OPTION[3]

    POINTS      = OPTION[4]
    if POINTS != '':
        loadRois(POINTS)


    # 나머지는 이미지 데이터
    b_img = b_totData[200:]

    print("OPTION          :", OPTION       )
    print("DRIVER_NAME     :", DRIVER_NAME  )
    print("FILE_PATH       :", FILE_PATH    )
    print("MODE            :", MODE         )
    print("IMAGE WIDTH     :", WIDTH        )
    print("IMAGE HEIGHT    :", HEIGHT       )
    print("IMAGE COMPRESS  :", COMPRESS     )
    print("VALUE           :", VALUE        )


    loadImage(WIDTH, HEIGHT)

    return int(MODE)

def sendImage():
    global DRIVER_NAME, FILE_PATH, VALUE, LUT, inImage, outImage, b_img, inH, inW, outW, outH

    R = Image.fromarray(outImage[0].reshape((outW, outH)))
    G = Image.fromarray(outImage[1].reshape((outW, outH)))
    B = Image.fromarray(outImage[2].reshape((outW, outH)))


    b_outImg = Image.merge("RGB", (R, G, B)).tobytes()

    length = len(b_outImg)

    print("bytes To Send : ", length)



    conn.send(length.to_bytes(4, byteorder='little'))
    print(conn.send(b_outImg[:]))




if __name__ == '__main__':

    ########### 통신 #############

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((RECV_TCP_IP, RECV_TCP_PORT))

    # Wating For Connect
    s.listen(True)

    # When Connetion Sucessed , Return Handler and address of client
    conn, addr = s.accept()
    conn.setblocking(True)

    print(conn)
    print(addr)

    while True:
        try:

            mode = recvAndLoad()

            if mode == 0:
                equalImage()

            elif mode == 1:
                addImage()

            elif mode == 2:
                reverseImage()

            elif mode == 3:
                binImage()

            elif mode == 4:
                paraImage()

            elif mode == 5:
                rotateImage()

            elif mode == 6:
                zoomout()

            elif mode == 7:
                zoomin()

            elif mode == 8:
                histoImage()

            elif mode == 9:
                homography()

            elif mode == 10:
                valueConvImage(EMBOSS_MASK)

        except UnicodeDecodeError as e:
            print(traceback.format_exc())

        except ConnectionResetError as e:
            print(traceback.format_exc())
            # Wating For Connect
            s.listen(True)

            # When Connetion Sucessed , Return Handler and address of client
            conn, addr = s.accept()
            conn.setblocking(True)

        except TypeError as e:
            print(traceback.format_exc())

            s.listen(True)

            # When Connetion Sucessed , Return Handler and address of client
            conn, addr = s.accept()
            print(conn)
            print(addr)


        except OSError as e:
            print(traceback.format_exc())

        except BaseException as e:
            print(traceback.format_exc())

        finally:
            pass
