"""
:author: TPS
"""

# IMPORTS

# importation de serial
import serial
from serial.tools import list_ports
import time
import csv

# FIN

# VARIABLES GLOBALES
s = ""
ser = serial.Serial()  # création d'un objet Serial

fname = "events.csv"
file = open(fname, 'a',newline='')

liste = ["date","temp"]
# FIN


# FONCTIONS

def listedesportscom():  # liste l'ensemble des ports com relier au pc
    """Permet de lister les ports de communication connectés au PC
    Une fois la liste établie, stockage dans une listedesportscom
    :returns: listeports la liste des ports
    :rtype: list
    """
    # print(list_ports.comports())
    listeports=list_ports.comports()
    for i, elt in enumerate(listeports):
        print("À l'indice {} se trouve {}.".format(i, elt))# liste l'ensemble des com relier au pc
        return listeports


def configdefautport(ser):  # configure le port
    """Configure le port série
    modifie les attribut de l'objet ser
    :param serial ser: Objet port série
    """
    ser.baudrate = 9600  # Vitesse en baud : 115200
    ser.port = 'COM21'  # Port série COM7 par défaut
    ser.timeout = 1  # timeout de communication

def receivedata(ser):  # recoit des infos du modem
    """recois une chaine du modem
    reçois une chaine du modem jusqu'au caractère de fin de chaine (max 500 char)
    et affiche le resultat
    :param serial ser: Objet port série
    :returns: data.decode("utf-8") le resultat
    :rtype: str
    :returns: "null" si il y un probleme de communication renvoi null
    :rtype: str
    """

    try:
        # data = ser.readline(8)
        data = ser.read_until(";",10)  # attend le caractère nul max 500char
        chaine = str(data.decode(encoding='UTF-8'))
        return chaine
    except:
        print("ERR : Impossible d'utiliser un port non ouvert")
        return "null"


def inicom(ser):  # initialisation de la communication avec le modem
    """Initialise la liaison série
    Passe le modem en mode commande
    :param serial ser: Objet port série
    """
    try:
        ser.open()  # ouvre le port
    except:
        print("ERR : Impossible d'ouvrir le port de communication")

def destroycom(ser):
    """Ferme le port de com
    Quite le mode commande du modem et ferme le port de communication
    :param serial ser: Objet port série
    :returns: 0
    :rtype: int
    """
    try:
        ser.close()  # fin de communication
    except:
        print("ERR : Impossible de fermer un port non ouvert")
    finally:
        return 0

def delay(d):
    """fonction delais
    :param int d: temps d'attente en seconde
    """
    time.sleep(d)

def getdate():
    t = time.localtime()
    t = str(t.tm_mday)+"/"+str(t.tm_mon)+"/"+str(t.tm_year)+"/"+" "+str(t.tm_hour)+":"+str(t.tm_min) +":"+str(t.tm_sec)
    # print(t)
    return t


def readallcsvfile():
    try:
        reader = csv.reader(file)
        # print("ici")
        for row in reader:
            print(row)
    finally:
        file.close()

def writecsvline():
    try:
        with open('events.csv', 'a', newline='') as f:
            writer = csv.writer(f)
            writer.writerow(liste)
    finally:
        file.close()

def savetocsv(temp):
    liste[0] = getdate()
    liste[1] = temp+"°C"
    print(liste)
    writecsvline()
# FIN










print("                                 *=========================================*")
print("                                 *             Events                      *")
print("                                 *=========================================*")
print("-----------------------------------------------------------------------------------------------------------------")




listedesportscom()
configdefautport(ser)
inicom(ser)
#savetocsv("25")
#savetocsv("40")
#savetocsv("2")



while 1==1:

    a = receivedata(ser)
    #print(len(a))
    if len(a) == 5:
        # print((a[2:4]))
        savetocsv(a[2:4])
