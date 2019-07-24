# mrsidx-interactive-candle
Interactive Candle via Node-Red Dashboard UI,  Sound board with Light Effects (ESP8266 Huzzah Feather Neopixel Shield)


This flow is the UI and audio player.


Please setup the global variables Node to enter
your server address information.


## Web Controll Interface:
visit:   <server address>:<node-red port>/ui
ie:  http://myPiserver.local:1880/ui
or:  http://192.168.0.10:1880/ui

SUMMARY:
Client loads UI controls page (above), page loads
audio sounds from server.
UI events send HTTP request to microcontroller,
processes and changes states to do lighting 
animation or effect.

Sound switches turn on and off the sounds.

UI sends WebSocket signal to client, client then plays
loaded audio file.

Multiple clients can all share the same UI
and all play the audio effects from their 
endpoints.  One node can trigger audio switch,
changes on all other connected endpoints.


FURTHER:

Buttons will change the lighting effect according
to the button label  ie:  strobe, rainbow, etc.

The node-red UI interface will send a HTTP request
to the Arduino Wifi unit (should be sharing the same
wifi network)

Arduino code can specify a mDNS local name to call
to, but this could be faulty if you are using
a client browser that doesn't support DNS local
name resolusions.  IP numerical addressing would
work better in this case, on the local network.

Switches will turn on and off the sounds played
through the client's browser endpoint, through 
the controll UI.  If no sounds are coming through,
check your server address settings in the Global Variables
function node, and check that your browser can
resolve an address name like  myPiServer.local,
if not, try using the numerical IP address instead
of a name.  [ie   myPiServer.local  VS  192.168.0.10]

ARDUINO CONTROLLER:
You will need to reflash the microcontroller to your
specific network,  change the WiFi settings to
match your router Wifi SSID Name and Password,
The light module used to develop this is the 
Adafruit Huzzah Feather with Neopixel shield,
You may need to adjust some neopixel settings
to match your neopixel count and settings, if 
you are using a different configuration.

If connected correctly, you will be able to change 
the lighting of the controller's neopixels using
the UI Web interface.


You can add more effects etc..
But you will need to hard code this for now.






-
