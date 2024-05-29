# SpaceShooter

# Introduction
A simple console-based "Space Shooter" game written in C++. The game features a player spaceship that can move left and right and shoot bullets upwards to destroy enemies. The game progresses through different levels, increasing the number and speed of enemies. The game uses various functionalities, including drawing characters to represent objects, handling user inputs for movement and shooting, collision detection, and scorekeeping.

## Constants and Variables
- **screenWidth** and **screenHeight** define the dimensions of the game screen.
- **windowWidth** is the width of the actual gameplay area.
- **playerPos** holds the horizontal position of the player spaceship.
- **score, lives, level, and timeCounter** keep track of the game's state.
- Arrays like **enemyY, enemyX, and enemyFlag** store the positions and states of the enemies.
- **bullet and bulletLife** manage the bullets fired by the player.

# Functions
## Drawing and Positioning
- **setPosition(int x, int y):** Sets the cursor position on the console to draw elements.
- **drawBorder():** Draws the borders of the gameplay area.
- **drawPlayer():** Draws the player's spaceship at the current position.
- **drawEnemy(int ind):** Draws an enemy at the specified index.
- **drawBullet():** Draws bullets that have been fired by the player.

## Game Mechanics
- **generateEnemy(int ind):** Randomly positions an enemy on the screen.
- **generateBullet():** Generates a bullet at the player's position.
- **moveBullet():** Moves bullets up the screen.
- **eraseEnemy(int ind):** Erases an enemy from the screen.
- **eraseBullet():** Erases bullets from the screen.
- **collision():** Checks for collisions between the player and enemies.
- **bulletHit():** Checks if bullets have hit enemies and handles the collision.

## Game States and Updates
- **updateScore():** Updates the displayed score on the screen.
- **updateLives():** Updates the displayed lives on the screen.
- **updateLevel():** Updates the displayed level on the screen.
- **storeScore():** Saves the player's score to a file.
- **gameover():** Displays the game over message and stores the score.
- **victory():** Displays the victory message and stores the score.

# Screenshots

![image](https://github.com/farhanj21/SpaceShooter/assets/102750756/34d6ad82-890e-47a7-9c48-3b9bf9f731a6)

![image](https://github.com/farhanj21/SpaceShooter/assets/102750756/34ed9079-51b8-4040-82c8-e1a2e9841f91)

![image](https://github.com/farhanj21/SpaceShooter/assets/102750756/910d1c4e-05f2-4dc4-a912-9c9ead5524c5)

![image](https://github.com/farhanj21/SpaceShooter/assets/102750756/d3c86ef5-c4eb-4378-bd18-73b2f775935f)


