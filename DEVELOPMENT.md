 - **11/17/19** Generated project 
   - Still need to get tests working

 - **11/18/19** Got tests working
   - Still need to create seperate configuration for tests to solve multiple main file issue

 - **11/19/19** Got Box2D working
   - Created simple circular planets
   - Created movable ship with arrow button controls

   **11/20/19** Added WASD movement

   **11/21/19**
    - Added gravity to planets
	- Added background music
	- Still need to add scaling with window resize/fullscreen
	- Start adding bullets
	  - Still need to fix collision and spawning

   **11/25/19**
    - Fix bullet spawn position
	- Bullets are now removed when out of bounds
	  - Still need to remove when colliding with planets
	- Make code more readable

   **11/26/19**
    - Make Planet its own subclass of ofxBox2dCircle
	- Make Bullet its own subclass of ofxBox2dRect
	- Create Identifier class for collision detection
	- Make bullets disappear after hitting planets

   **11/28/19**
    - Fix memory leak by preloading bullets

   **11/29/19**
    - Add enemies and preload them

   **11/30/19**
    - Give enemies hitpoints and deal with cancelling multiple collisions
	- Add screen wrapping for player

   **12/1/19**
    - Enemies change colors when hit

   **12/2/19**
    - Create player class

   **12/3/19**
    - Ammo and fuel
	  - Add ammo and fuel tracking
	  - Add ammo and fuel display
	  - Add ammo and fuel refresh if player is on correct planet
	- Add basic enemy spawning

   **12/4/19**
    - Add powerup icons
      - Todo: set collision parameters
      - Implement upgrades

   **12/5/19**
    - Add attack stat
    - Implement collecting powerups
      - Todo: add more powerup types
    - Add collision rules