# Space Turret

## What is it?
Space Turret is a game I made for the CS 126 final project at the University of Illinois. 

## How to build it

1. **Downloads**
	1. [Space Turret](https://github.com/CS126FA19/fantastic-finale-grenadilla)\
    The game itself.

	1. [Visual Studio](https://visualstudio.microsoft.com/downloads/)\
	Space Turret was made on Visual Studio 2019 and has not been tested for any other version of Visual Studio.

	1. [openFrameworks](https://openframeworks.cc/download/)\
    Graphics engine and more.

	1. [ofxBox2D](https://github.com/vanderlin/ofxBox2d)\
	ofxBox2D is an openFrameworks wrapper for the physics engine Box2D.

	1. [Images and sound](https://uofi.box.com/s/5jpn9rv4qjyl5s19regpyf2uiw9xpn1a)\
    Contains music and images for the game.

1. **Convert to openFrameworks project**
	1. Move ofxBox2D into the addons folder in openFrameworks
    2. Use the openFrameworks project generator located in /projectGenerator-vs to convert the game into an openFrameworks project. Make sure to include ofxBox2D as an addon.
	3. Move the downloaded music and images data folder into /bin/data

1. **Build using Visual Studio!**

## How to play

### Controls
<kbd>W</kbd> or <kbd>↑</kbd> to apply a forward force\
<kbd>S</kbd> or <kbd>↓</kbd> to apply a backwards force\
<kbd>A</kbd> or <kbd>←</kbd> to turn counterclockwise\
<kbd>D</kbd> or <kbd>→</kbd> to turn clockwise\
<kbd>Space</kbd> to shoot

### Objective
Your objective is to defend the two planets from the evil triangular spaceships. 
Be careful - the planets have strong gravity fields and you have limited fuel and ammo capacity that can only be replenished on the correct planet.
Collect powerups to defeat enemies more easily, and make sure they don't hit you or any planet!

## Credits

### Images
[Blast](https://game-icons.net/1x1/sbed/blast.html) - sbed\
[Ion Cannon Blast](https://game-icons.net/1x1/lorc/ion-cannon-blast.html) - Lorc\
[Heart Plus](https://game-icons.net/1x1/zeromancer/heart-plus.html) - Zeromancer\
[Fuel Tank](https://game-icons.net/1x1/delapouite/fuel-tank.html) - Delapouite\
[Oil Drum](https://game-icons.net/1x1/skoll/oil-drum.html) - Skoll\
[Heavy Bullets](https://game-icons.net/1x1/delapouite/heavy-bullets.html) - Delapouite\
[Reload Gun Barrel](https://game-icons.net/1x1/delapouite/reload-gun-barrel.html) - Delapouite

### Sound
[Tilt](https://soundcloud.com/avaren/tilt?in=avaren/sets/precipice-ep) - Avaren\
[Retro Punch](https://freesound.org/people/damnsatinist/sounds/493915/) - damnsatinist\
[Retro Explosion](https://freesound.org/people/stumpbutt/sounds/381686/) - stumpbutt\
[Powerup](https://freesound.org/people/JustInvoke/sounds/138485/) - JustInvoke\
[Laser Shot Silenced](https://freesound.org/people/bubaproducer/sounds/151022/) - bubaproducer