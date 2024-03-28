#LIBRERIAS
from _py_librerias  import Bluetooth as Bt #Bluetooth
from _py_librerias  import Camera as cam #Camara
import cv2 #Opencv2

def normalize(wpn, wm):
    if(wpn>wm):
        wpn=wm
    elif(wpn<-wm):
        wpn=-wm
    return wpn

#INICIALIZACION ROBOT Y CAMARA 
macAddress = "E8:6B:EA:CA:74:CA"  
robot0=Bt.connect(macAddress)

robot_bt1=robot0 #Seleccion del robot a utilizar en el programa

#RESOLUCION CAMARA HD
resolucionx=1280
resoluciony=720
camara=cam.initialize(0,resolucionx,resoluciony)

#PARAMETROS ROBOT UNICILO
wmax=150 #Velocidad angular robot maxima
r=48.5 #radio llanta (pixeles)
Lx=107.5 #mmdistancia entre centros de llantas (pixeles)
Ly=107.5 #mm

#INICIALIZACION DE VALORES ROBOT FISICO
vx = 0      #Velocidad en x
vy = 0      #Velocidad en y
wfr = 0     #W front right
wfl = 0     #W front left
wbr = 0     #W back right
wbl = 0     #W back left
w = 0       #Velocidad angular inicial del robot

#PROGRAMA PRINCIPAL
while (True):

    #Cálculos de velocidades
    print(f"Vx = {vx}, Vy = {vy}, w = {w}")

    #Velocidades de nuestro robot
    wfl = (1/r)*(vx + vy - (Ly + Lx)*w)
    wfr = (1/r)*(-vx + vy + (Ly + Lx )*w)
    wbl = (1/r)*(-vx + vy - (Ly + Lx )*w)
    wbr = (1/r)*(vx + vy + (Ly + Lx )*w)

    #Normalización de velocidades de cada motor
    wfl = normalize(wfl, wmax)
    wfr = normalize(wfr, wmax)
    wbl = normalize(wbl, wmax)
    wbr = normalize(wbr, wmax)

    #Mostramos los cálculos
    print(f"Vx = {vx}, Vy = {vy}, w = {w}")
    print(f"wfl = {wfl}, wfr = {wfr}, wbl = {wbl}, wbr = {wbr}")

    #Control bluetooth y camara
    Bt.move(robot_bt1, wbl, wbr, wfl, wfr)
    cam.preview(camara,resolucionx, resoluciony)

    #Detectamos tecla presionada en teclado
    k=cv2.waitKey(33)
    if k == 27: #Presiona esc para salir 
        break
    elif k == 32: #Presiona space para detener al robot
        vx = 0
        vy = 0
        w = 0
    elif k == 119: #Presiona w para avanzar
        vx = 0
        vy = 8700
        w = 0
    elif k == 115: #Presiona s para ir atras
        vx = 0
        vy=-8700
        w=0
    elif k == 100: #Presiona d para rotar derecha
        vx = 0
        vy = 0
        w = 70
    elif k== 97: #Presiona a para rotar izquierda
        vx = 0
        vy = 0
        w=-70
    
Bt.move(robot_bt1,wfr,wfl,wbr,wbl) #Enviamos wd y wi en cero a robot 1 bluetooth para detenerlo
Bt.disconnect(robot_bt1)    #Desconectamos Bluetooth
camara.release()            #Liberamos la camara
cv2.destroyAllWindows()     #Cerramos todas las ventanas