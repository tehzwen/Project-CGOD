import time
import socket
import ctypes
import struct
import pygame
import random
from player import *



RESOLUTION = (800, 600)

def getRandomStartPos():
    randomX = random.randint(0, RESOLUTION[0])
    randomY = random.randint(0, RESOLUTION[1])

    return (randomX, randomY)

myX = RESOLUTION[0]//2
myY = RESOLUTION[1]//2

speedMultiplier = 1

startPos = getRandomStartPos()

movementX = startPos[0]
movementY = startPos[1]

pressed_down = False
pressed_up = False
pressed_left = False
pressed_right = False

objectList = []
playerList = []

rockObject = (10, 50, "&")
objectList.append(rockObject)

userInput = input("enter your username: ")

pygame.init()
pygame.font.init()
myfont = pygame.font.SysFont('Ubuntu Mono', 30)
stickImg = pygame.image.load('./resources/stickman.png')

crashed = False

gameDisplay = pygame.display.set_mode(RESOLUTION)
pygame.display.set_caption('Multiplayer C Game!')
clock = pygame.time.Clock()

#def checkIfPlayerInBounds(player):


def checkIfPlayerAlreadyExists(player):
    for i in range(len(playerList)):
        print ()
        if (player.getUserName() == playerList[i].getUserName()):
            return True

    

    return False

def updatePlayerInformation(player):
    
    for i in range(len(playerList)):
        if (player.getUserName() == playerList[i].getUserName()):
            print("UPDATING")
            playerList[i].Active = player.Active
            playerList[i].X = player.X
            playerList[i].Y = player.Y

    


def sendPacketToServer(xMovement, yMovement):
    proto = Packet()

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.settimeout(1.0)
    proto.active = 1
    proto.x = myX + xMovement
    proto.y = myY + yMovement
    myUserName = userInput
    proto.userName = myUserName.encode('utf-8')

    addr = ("127.0.0.1", 3000)

    start = time.time()
    client_socket.sendto(proto, addr)
    try:
        data, server = client_socket.recvfrom(1024)
        end = time.time()
        elapsed = end - start

        #print(data[2:15])
        if (int.from_bytes(data[2:3], byteorder='big') == 66):
            print("NEW LOGIN")

        elif (int.from_bytes(data[0:1], byteorder='big') == 0):
            print("Empty Array!")

        else:

            packetArrayLen = int.from_bytes(data[0:1], byteorder='big')

            #print(packetArrayLen)
            print (data)
            print (data[5:7])
            print (int.from_bytes(data[5:7], byteorder='big'))

            startIndex = 1
            # loop through the buffer and populate playerList
            for i in range(packetArrayLen):
                #print("I: " + str(i))

                receivedPack = Packet()

                activeBytes = data[startIndex:startIndex + 2]
                activeInt = int.from_bytes(activeBytes, byteorder='big')

                yBytes = data[startIndex + 3:startIndex + 5]
                yInt = int.from_bytes(yBytes, byteorder='big')

                xBytes = data[startIndex + 6: startIndex + 8]
                xInt = int.from_bytes(xBytes, byteorder='big')

                userNameBytes = data[startIndex + 9:startIndex + 15]
                startIndex = 0
                userNameString = userNameBytes.decode("utf-8").rstrip('\0')

                #print("active: " + str(activeInt) + "\nY: " + str(yInt) +
                 #   "\nX: " + str(xInt) + "\nUserName: " + userNameString)

                temp = Player(activeInt, xInt, yInt, userNameString)

                if (not checkIfPlayerAlreadyExists(temp)):
                    playerList.append(temp)

                else:
                    updatePlayerInformation(temp)


    except socket.timeout:
        print('REQUEST TIMED OUT')


def drawPlayer(x, y):
    gameDisplay.blit(stickImg, (x, y))


def drawGameObjects(objectList, x, y):
    for i in range(len(objectList)):
        tempText = myfont.render(objectList[i][2], True, (0, 255, 0))
        gameDisplay.blit(
            tempText, (objectList[i][0] + x, objectList[i][1] + y))


class Packet(ctypes.Structure):
    _fields_ = [
        ("active", ctypes.c_short),
        ("x", ctypes.c_short),
        ("y", ctypes.c_short),
        ("userName", ctypes.c_char * 10)
    ]

sendPacketToServer(movementX, movementY)


while(not crashed):

    myPlayerText = myfont.render("*", True, (0, 255, 0))

    for event in pygame.event.get():

        if event.type == pygame.QUIT:
            crashed = True

        elif event.type == pygame.KEYDOWN:          # check for key presses
            print("here")
            if event.key == pygame.K_LEFT:        # left arrow turns left
                pressed_left = True
            elif event.key == pygame.K_RIGHT:     # right arrow turns right
                pressed_right = True
            elif event.key == pygame.K_UP:        # up arrow goes up
                pressed_up = True
            elif event.key == pygame.K_DOWN:     # down arrow goes down
                pressed_down = True
        elif event.type == pygame.KEYUP:            # check for key releases
            print("up here")
            if event.key == pygame.K_LEFT:        # left arrow turns left
                pressed_left = False
            elif event.key == pygame.K_RIGHT:     # right arrow turns right
                pressed_right = False
            elif event.key == pygame.K_UP:        # up arrow goes up
                pressed_up = False
            elif event.key == pygame.K_DOWN:     # down arrow goes down
                pressed_down = False

        if pressed_left:
            movementX += 10
        if pressed_right:
            movementX -= 10
        if pressed_up:
            movementY += 10
        if pressed_down:
            movementY -= 10

    sendPacketToServer(-movementY, -movementX)

    gameDisplay.fill((0, 0, 0))

    gameDisplay.blit(myPlayerText, (myX, myY))
    drawGameObjects(objectList, (movementX * speedMultiplier), (movementY * speedMultiplier))

    #print("HERE")
    #print(len(playerList))
    # draw the other players on the screen
    for i in range(len(playerList)):

        otherPlayerText = myfont.render(
            playerList[i].getUserName() + str(playerList[i].X) + " " + str(playerList[i].Y), True, (0, 255, 0))
        tempx, tempy = playerList[i].getCoords()
        gameDisplay.blit(
            otherPlayerText, (tempx + movementX, tempy + movementY))

    pygame.display.update()
    clock.tick(60)


pygame.quit()
quit()
