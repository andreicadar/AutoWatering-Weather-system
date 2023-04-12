# AutoWatering-Weather-system

A project made for school which displays weather from the internet and auto waters a plant

## Components

<ul>
    <li>Raspberry Pi 3B+</li>
    <li>Arduino Uno board</li>
    <li>20x4 LCD display</li>
    <li>Tower Pro Micro Servo 9g</li>
    <li>Capacitive humidity sensor</li>
    <li>Arduino Uno board</li>
    <li>TMP35 temperature sensors</li>
    <li>Resistors and capacitors</li>
    <li>0 -> 10k Ohm potentiometers</li>
</ul>

## Usage

After the Raspberry PI is powered on it also powers the Arduino which is connected through USB.

The [**weather script**](RaspberryGetWeather.py) is executed on the RPI by a cronjob every two minutes.
<br>
When it is executed is sends through serial communication all the weather information to the the Arduino.
