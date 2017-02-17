Title:		Super Mario Clone

Programmer:	Eric Truitt

Controls
	Movement: Left & Right Arrow Keys
	Jump:	Spacebar
	Shoot: Z (must have flower power)

Win Condition: Win the game by reaching the castle before the timer runs out without dying

Lose Condition: Lose by running into a Goomba, falling off the ground, or running out of time

Enemies: Goomba - defeat by jumping on top of them or shooting them with a fireball.

Features:
	Animations - all mario variations are animated using Anchor Point Animation.
		   - goombas and mystery boxes are animated using Cell Animation.

	Credits state scrolls up the screen.

	Game uses camera(world) coordinates to keep track of where everything in the world "lives"

	Game World is drawn and loaded using TileMapping.

	Game Features multiple platforms and obstactles to slow the player down.

	All entities use their own state machines to determine how they behave

	Game featues 2 different power ups (Mushrooms, Flowers).