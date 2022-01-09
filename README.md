# oscLaserShow

Control a laser via OSC using ofxOSC and ofxLaser openframeworks addons. Tested with a [Helios DAC](https://bitlasers.com/helios-laser-dac/) and a Laserworld CS-2000RGB FX. 

![Open Stage Control Template](/doc/openStageControlScreen.png)

Features:
- TCP connection used to ensure that if OSC client is disconnected, lasers are shut off.
- Select connected DAC using drop down
- Preset mask to reduce laser intensity. Mask coverage controlled by vertical value.
- Mask level control (0-100%)
- Global Brightness control
- Animation speed control
- Select from multipe pre-defined patterns

How to use:
Clone this repository, compile and run it. Using [Open Stage Control](http://openstagecontrol.ammd.net/) you can test the functionality using [openStageControlServer.config](/openStageControlServer.config) as server configuration and [openStageControlTemplate.json](/openStageControlTemplate.json) as a OSC layout. The Patterns can be changed in ofApp.cpp.

Addons required:
- ofxOsc
- ofxLaser
- ofxSvg
- ofxNetwork
