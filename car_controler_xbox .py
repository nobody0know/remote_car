import pygame
import paho.mqtt.client as mqtt
import time
# Define some colors
BLACK    = (   0,   0,   0)
WHITE    = ( 255, 255, 255)
 
# This is a simple class that will help us print to the screen
# It has nothing to do with the joysticks, just outputting the
# information.
class TextPrint:
    def __init__(self):
        self.reset()
        self.font = pygame.font.Font(None, 20)
 
    def print(self, screen, textString):
        textBitmap = self.font.render(textString, True, WHITE)
        screen.blit(textBitmap, [self.x, self.y])
        self.y += self.line_height
        
    def reset(self):
        self.x = 10
        self.y = 10
        self.line_height = 15
        
    def indent(self):
        self.x += 10
        
    def unindent(self):
        self.x -= 10
    
class MqttRoad(object):
 
    def __init__(self, mqtt_host, mqtt_port, mqtt_keepalive):
        super(MqttRoad, self).__init__()
        
        client.on_connect = self.on_connect
        client.on_message = self.on_message
        client.on_publish = self.on_publish
        client.on_disconnect = self.on_disconnect
        client.on_subscribe = self.on_subscribe
        client.connect_async(mqtt_host, mqtt_port, mqtt_keepalive)  # 600为keepalive的时间间隔
        client.loop_start()  # 保持连接
 
    def on_connect(self, client, userdata, flags, rc):
        print("Connected with result code: " + str(rc))
        # 订阅
        client.subscribe("accelerator")
        client.subscribe("turn")
        client.subscribe("brake")
        client.subscribe("gearmode")
 
 
    def on_message(self, client, userdata, msg):
        print("on_message topic:" + msg.topic + " message:" + str(msg.payload.decode('utf-8')))
        
 
    #   订阅回调
    def on_subscribe(self, client, userdata, mid, granted_qos):
        print("On Subscribed: qos = %d" % granted_qos)
        pass
 
    #   取消订阅回调
    def on_unsubscribe(self, client, userdata, mid):
        # print("取消订阅")
        print("On unSubscribed: qos = %d" % mid)
        pass
 
    #   发布消息回调
    def on_publish(self, client, userdata, mid):
        # print("发布消息")
        print("On onPublish: qos = %d" % mid)
        pass
 
    #   断开链接回调
    def on_disconnect(self, client, userdata, rc):
        # print("断开链接")
        print("Unexpected disconnection rc = " + str(rc))
        pass
 
 
pygame.init()
 
# Set the width and height of the screen [width,height]
size = [500, 700]
screen = pygame.display.set_mode(size)
 
pygame.display.set_caption("My Game")
 
#Loop until the user clicks the close button.
done = False
 
# Used to manage how fast the screen updates
clock = pygame.time.Clock()
 
# Initialize the joysticks
pygame.joystick.init()
    
# Get ready to print
textPrint = TextPrint()
 
HOST = "bemfa.com"
PORT = 9501
topic = "test"
 

client = mqtt.Client("4ea6ab40f4f64f0b80fcddf9c92453f7")
MqttRoad(HOST, PORT, 600)

# -------- Main Program Loop -----------
while done==False:
    # EVENT PROCESSING STEP
    for event in pygame.event.get(): # User did something
        if event.type == pygame.QUIT: # If user clicked close
            done=True # Flag that we are done so we exit this loop
        
        # Possible joystick actions: JOYAXISMOTION JOYBALLMOTION JOYBUTTONDOWN JOYBUTTONUP JOYHATMOTION
       # if event.type == pygame.JOYBUTTONDOWN:
        #    print("Joystick button pressed.")
       # if event.type == pygame.JOYBUTTONUP:
        #    print("Joystick button released.")
            
 
    # DRAWING STEP
    # First, clear the screen to white. Don't put other drawing commands
    # above this, or they will be erased with this command.
    screen.fill(BLACK)
    textPrint.reset()
 
    # Get count of joysticks
    joystick_count = pygame.joystick.get_count()
 
    #textPrint.print(screen, "Number of joysticks: {}".format(joystick_count) )
    textPrint.indent()
    
    # For each joystick:
    for i in range(joystick_count):
        joystick = pygame.joystick.Joystick(i)
        joystick.init()
    
       # textPrint.print(screen, "Joystick {}".format(i) )
        #textPrint.indent()
    
        # Get the name from the OS for the controller/joystick
        #name = joystick.get_name()
        #textPrint.print(screen, "Joystick name: {}".format(name) )
        
        # Usually axis run in pairs, up/down for one, and left/right for
        # the other.
        axes = joystick.get_numaxes()
        #textPrint.print(screen, "Number of axes: {}".format(axes) )
        #textPrint.indent()
        
        for i in range( axes ):
            axis = joystick.get_axis( i )
            textPrint.print(screen, "Axis {} value: {:>6.3f}".format(i, axis) )
            #if i==1 and (axis<-0.1 or axis>0.1):
                #print("Left ↑ ↓",-axis)
                #client.publish(topic="vx",payload=axis,qos=0,retain=False)
            if i==0:
                print("Left ← →",-axis)
                client.publish(topic="turn",payload=axis,qos=0,retain=False)
            if i==3 and axis < -0.1:
                print("Right up",axis)
            if i==3 and axis > 0.1:
                print("Right down",axis)
            if i==2 and axis < -0.1:
                print("Right left",axis)
            if i==2 and axis > 0.1:
                print("Right right",axis)
            if i==4 :
                axis+=1
                print("brake",axis)
                client.publish(topic="brake",payload=-axis*1.3,qos=0,retain=False)
            if i==5 :
                axis+=1
                print("accelerator",axis)
                client.publish(topic="accelerator",payload=-axis*1.3,qos=0,retain=False)
       
        textPrint.unindent()
            
        buttons = joystick.get_numbuttons()
        textPrint.print(screen, "Number of buttons: {}".format(buttons) )
        textPrint.indent()
 
        for i in range( buttons ):
            button = joystick.get_button( i )
            textPrint.print(screen, "Button {:>2} value: {}".format(i,button) )
            if i==0 and button ==1:
                print("A")
            if i==1 and button ==1:
                print("B")
            if i==2 and button ==1:
                print("X")
            if i==3 and button ==1:
                print("Y")
            if i==4 and button ==1:
                print("LB")
            if i==5 and button ==1:
                print("RB")
            if i==6 and button ==1:
                print("BACK")
            if i==7 and button ==1:
                print("START")
            if i==8 and button ==1:
                print("Logitech")
            if i==9 and button ==1:
                print("Left GA")
            if i==10 and button ==1:
                print("Right GA")
 
        textPrint.unindent()
            
        # Hat switch. All or nothing for direction, not like joysticks.
        # Value comes back in an array.
        hats = joystick.get_numhats()
        textPrint.print(screen, "Number of hats: {}".format(hats) )
        textPrint.indent()
 
        for i in range( hats ):
            hat = joystick.get_hat( i )
            textPrint.print(screen, "Hat {} value: {}".format(i, str(hat)) )
            if hat==(1,0) :
                print("FX right")
            if hat==(-1,0) :
                print("FX left")
            if hat==(0,1):
                print("FX up")
                client.publish(topic="gearmode",payload=-1,qos=2,retain=False)
            if hat==(0,-1):
                print("FX down")
                client.publish(topic="gearmode",payload=1,qos=2,retain=False)

        textPrint.unindent()
        
        textPrint.unindent()
        
        
    # ALL CODE TO DRAW SHOULD GO ABOVE THIS COMMENT
    
    # Go ahead and update the screen with what we've drawn.
    pygame.display.flip()
    
    # Limit to 20 frames per second
    clock.tick(20)

    
# Close the window and quit.
# If you forget this line, the program will 'hang'
# on exit if running from IDLE.
pygame.quit ()