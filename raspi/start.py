import XboxController
import serial
import time
# This is the port the arduino is assigned, I found the last number increased 
# each time I restarted the program.
ser = serial.Serial('/dev/ttyACM0', 9600)
#ser = serial.Serial('/dev/ttyACM4', 9600)
score = 0
targetsOn = [0,0,0,0]

def myCallBack(controlId, value):
  # back button quits program.
  if controlId==12 and value==1:
    xboxCont.stop()
  # put up target if button pressed 
  elif controlId>=6 and controlId <=9 and value == 1:
    #print "controller button "
    # write value to arduino
    controlId = controlId - 6
    #if not targetsOn[controlId] == value:
    targetsOn[controlId] = value
    ser.write(str(controlId))
    ser.write(str(value))

# wait for on/off value from arduino
def waitForValue(targetNum):
  global score
  value = int(ser.read())
  score = score + value * 10 * (targetNum+1)
  print "Score: " + str(score)
  # Turn off the target
  ser.write(str(targetNum))
  ser.write("0")


print "start"
xboxCont = XboxController.XboxController(
    controllerCallBack = myCallBack,
    joystickNo = 0,
    deadzone = 0.1,
    scale = 1,
    invertYAxis = False)

xboxCont.start()

while 1 :
  # read serial in
  targetNum = int(ser.read())
  waitForValue(targetNum)
