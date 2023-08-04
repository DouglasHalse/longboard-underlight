# Longboard Underlight
This is an old project to add dynamic light effects that mount to the underside of a longboard. 
<p float="center">
    <img src="images/animation.gif" width="100%" />
</p>

# The hardware
I use an Arduino Nano Every for the microcontroller powered by a power bank. 
These are stored in an aluminum extruded box mounted to the underside of the longboard.

<p float="center">
  <img src="images/extrudedBox.jpg" width="30%" />
  <img src="images/powerButton.jpg" width="30%" /> 
  <img src="images/chargingPort.jpg" width="30%" />
</p>

Two LED strips are mounted along the underside of the longboard and connected to the microcontroller.

<p float="center">
    <img src="images/ledStrips.jpg" width="50%" />
</p>


Two magnets are mounted to the inside of one of the front wheels and a hall effect sensor senses them passing. 
This is used to estimate the board's current speed and the animation speed is set to be proportional to the estimated speed.

<p float="center">
    <img src="images/magnets.jpg" width="50%" />
</p>
