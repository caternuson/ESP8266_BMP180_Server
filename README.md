# ESP8266 BMP180 Server
![thumbnail](http://caternuson.github.io/oat-thumb.jpg)<br/>
An [Adafruit HUZZAH ESP8266](https://www.adafruit.com/products/2471)
based server for an
[Adafruit BMP180](https://www.adafruit.com/product/1603)
pressure/temperature/altitude sensor.

# Hardware
* [Adafruit HUZZAH ESP8266](https://www.adafruit.com/products/2471)
* [Adafruit BMP180](https://www.adafruit.com/product/1603)
* empty [nuun](http://nuun.com/) tube
* hookup wires with optional connectors
* 3/4" copper plumbing mount
* screws

# Software
You will need a [USB console cable](https://www.adafruit.com/products/954)
to program the HUZZAH. Follow
[this](https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/using-arduino-ide)
guide for information on how to setup the Arduino IDE.

# Dependencies
* [ESP8266 Board Package](https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/using-arduino-ide#install-the-esp8266-board-package)
* [Adafruit BMP085 library](https://github.com/adafruit/Adafruit-BMP085-Library)

# Configure
You will need to create a file called ```network_config.h``` that will include
information for connecting to your wifi network. It also includes information
for setting up the IP address and port configuration for the ESP8266.
It should have the following contents.
```C++
#define MY_SSID "your_ssid"
#define MY_PASSWORD "your_password"
#define MY_PORT 65050
#define MY_IP_ADDRESS 192, 168, 1, 100
#define MY_IP_GATEWAY 192, 168, 1, 1
#define MY_IP_SUBNET 255, 255, 255, 0
```
Replace ```your_ssid``` and ```your_password``` with your wifi network
specifics. The remaining values configure the ESP8266 once it is connected.
Some nominal values are shown, which should be changed depending on your
network setup.

# Getting Data
Once the ESP8266 HUZZAH has been programmed, powered, and successfully
connected to your network, you should be able to request temperature
and pressure from it. A python code sniper below shows how to
request the current temperature.
```python
def get_OAT():
    """Get outside air temperature in Fahrenheit."""
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(5.0)
        s.connect(('192.168.1.100',65050))
        s.send('GCT')
        data = s.recv(1024)
        s.close()
        temp = float(data)
        return temp
    except (socket.error, ValueError):
        return -999.0
    except:
        return -999.0
```