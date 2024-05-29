
#include<iostream>	// Library for input and output stream
#include<conio.h>	// Library for getche() and kbhit() functions
#include<windows.h> // Library for structs such as HANDLE and for functions such as Sleep()
#include<fstream>	// Library for file stream to write scores to a file

#define screenWidth 90
#define screenHeight 26
#define windowWidth 70		// Window width only covers the actual gameplay, screen width covers both gameplay and side menu

using namespace std;

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);		// Main handle assigned to cursor
COORD CursorPosition;		// This is a struct which holds two values which are X and Y coordinates. It is used in the SetConsoleCursorPosition() function.

int numEnemies;
int level = 1;
int timeCounter = 0;
int enemyY[20];		// There are 20 total enemies
int enemyX[20];
int enemyFlag[20];
char player[3][5] = { ' ', ' ', 202, ' ', ' ',		// The player spaceship is 3 rows and 5 columns in size. We made it big to subsitute graphics
					202, 202, 202, 202, 202,
					202, 202, 202, 202, 202 };
int playerPos = windowWidth / 2;		// To center the spaceship on the screen
int score = 0;
int lives = 3;
int bullet[20][4];		// The bullet has 20 rows because the game height used is 20. When it reaches a height of 20 it disappears.
int bulletLife[20];
int bulletIndex = 0;
void setPosition(int x, int y) {		// This is the main function utilised in the game. It gets an X and Y coordinate and sets the cursor at that position
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(hStdOut, CursorPosition);
}
void drawBorder() {			// Function for drawing the borders of the game screen.

	for (int i = 0; i < screenWidth; i++) { // Draws the bottom row
		setPosition(i, screenHeight);
		cout << char(178);
	}

	for (int i = 0; i < screenHeight; i++) { // Draws the leftmost and rightmost columns
		setPosition(0, i);
		cout << char(178);
		setPosition(screenWidth, i);
		cout << char(178);
	}
	for (int i = 0; i < screenHeight; i++) { // Draws the middle column.
		setPosition(windowWidth, i);
		cout << char(178); // 178th character is used as that looked most similar to the one in the question example.
	}
}
void generateEnemy(int ind) {			// Randomizes the X values of enemies to randomly generate them on screen
	enemyX[ind] = 3 + rand() % (windowWidth - 10);
}
void drawEnemy(int ind) {		// Draws the actual enemy using characters on several locations
	SetConsoleTextAttribute(hStdOut, 4);
	if (enemyFlag[ind] == true) {
		setPosition(enemyX[ind], enemyY[ind]);
		for (int i = 0; i < 4; i++)
		{
			cout << char(254); // 254th character is used which is a box
		}
		setPosition(enemyX[ind], enemyY[ind] + 1); // This will draw the next line of the enemy. There are 4 total lines of the enemy spaceship
		for (int i = 0; i < 4; i++)
		{
			cout << char(254);
		}
		setPosition(enemyX[ind], enemyY[ind] + 2);
		for (int i = 0; i < 4; i++)
		{
			cout << char(254);
		}
		setPosition(enemyX[ind], enemyY[ind] + 3);
		cout << "."; // Added a dot at the mouth of the enemy to add characteristic.
		for (int i = 0; i < 2; i++)
		{
			cout << char(254);
		}
		cout << ".";
	}
	SetConsoleTextAttribute(hStdOut, 7);
}
void eraseEnemy(int ind) {		// Erases the enemy using empty spaces when conditions meet
	if (enemyFlag[ind] == true) {
		setPosition(enemyX[ind], enemyY[ind]);
		cout << "    ";
		setPosition(enemyX[ind], enemyY[ind] + 1); // Every line is erased
		cout << "    ";
		setPosition(enemyX[ind], enemyY[ind] + 2);
		cout << "    ";
		setPosition(enemyX[ind], enemyY[ind] + 3);
		cout << "    ";
	}
}
void resetEnemy(int ind) {		// Erases the enemy, and then generates it again at a random X with Y = 4
	eraseEnemy(ind);
	enemyY[ind] = 4;
	generateEnemy(ind);
}
void generateBullet() {			// Used to generate the bullet when the correct key is pressed.
	bullet[bulletIndex][0] = 22;
	bullet[bulletIndex][1] = playerPos + 2; // So that it is generated at the mouth of the player spaceship
	bulletIndex++;
	if (bulletIndex == 20) // This means the bullet reaches the end of screen
		bulletIndex = 0;
}
void moveBullet() {			// Moves the bullet up the screen
	for (int i = 0; i < 20; i++) {
		if (bullet[i][0] > 2)
			bullet[i][0]--; // As we decrease the value of Y, the bullet moves up
		else
			bullet[i][0] = 0;
	}
}
void drawBullet() {		// 15th value of Char is used as bullet. This function draws the bullet
	for (int i = 0; i < 20; i++) {
		if (bullet[i][0] > 1) {
			setPosition(bullet[i][1], bullet[i][0]);
			cout << char(15);
		}
	}
}
void eraseBullet() {		// Similar to erasing enemy, this function erases the bullet by adding empty space, when bullet reaches top of screen
	for (int i = 0; i < 20; i++) {
		if (bullet[i][0] >= 1) {
			setPosition(bullet[i][1], bullet[i][0]);
			cout << " ";
		}
	}
}
void eraseBullet(int i) {		// This function erases when the bullet hits the enemy. That's why it is different than the above function.
	setPosition(bullet[i][1], bullet[i][0]);
	cout << " ";
}
void drawPlayer() {		// This draws the player spaceship at the correct position
	SetConsoleTextAttribute(hStdOut, 2);		// Cursor Handle used to change color of player
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) { // The player spaceship has a 3x5 matrix
			setPosition(j + playerPos, i + 22);
			cout << player[i][j]; // The player array values are already assigned in the start. We just have to output them.
		}
	}
	SetConsoleTextAttribute(hStdOut, 7);
}
void erasePlayer() {		// Similar to bullet and enemy, this function erases the player
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			setPosition(j + playerPos, i + 22);
			cout << " ";
		}
	}
}

int collision() {			// This function is used to detect the collision between player and enemy. (not to be confused with bullet and enemy)
	for (int i = 0; i < numEnemies; i++)
	{
		if (enemyY[i] + 4 >= 23) {
			if (enemyX[i] + 4 - playerPos >= 0 && enemyX[i] + 4 - playerPos < 8) { // Since both enemy and player are big in size, +4 is used when detecting.
				return 1;
			}
		}
	}
	return 0;
}
int bulletHit() {		// This function checks if the bullet is hitting the enemy
	for (int i = 0; i < numEnemies; i++) // It is run for the number of times equal to the number of enemies currently in the game depending on level
	{
		for (int j = 0; j < 20; j++) { // Since the bullet travels 20 spaces
			for (int k = 0; k < 4; k += 2) { // Since the bullet array has hitbox of 4 because enemies are 4x4 in size
				if (bullet[j][k] != 0) {
					if (bullet[j][k] >= enemyY[i] && bullet[j][k] <= enemyY[i] + 4) { // To check if the bullet is in between the enemy
						if (bullet[j][k + 1] >= enemyX[i] && bullet[j][k + 1] <= enemyX[i] + 4) { // To check that if when the bullet moves up it still hits the enemy
							eraseBullet(j); // Erases the bullet
							bullet[j][k] = 0; // Resets the value of that bullet
							resetEnemy(i); // Generates a new enemy
							return 1; // Used later on
						}
					}
				}
			}
		}
	}
	return 0;
}
void storeScore() // Function to store final scores of player into a file
{
	char name[100];
	ofstream outFile;
	outFile.open("scores.txt", ios::app); // So that file is not overwritten but rather appended
	cin.getline(name, 100);
	outFile << name << " " << score << endl;
	outFile.close();
}
void gameover() { // If lives run out and the player loses, this function runs.
	system("cls");
	cout << endl;
	cout << "\t\t--------------------------" << endl;
	cout << "\t\t-------- Game Over -------" << endl; // Message display
	cout << "\t\t--------------------------" << endl << endl;
	cout << "\t\tEnter your name, final score = " << score << endl << "\t\t"; // \t is used to give the equal amount of space as when the tab key is pressed in IDE
	Sleep(500); // So that if keys were being spammed this message isn't skipped
	storeScore(); // Function to store the final score and name of player into the file
	cout << "\t\tPress any key to go back to the menu." << endl;
	_getch();
}
void victory() // If the user beats all 5 levels, this message is displayed
{
	system("cls");
	cout << endl << endl;
	cout << "\t\t           YOU WON!!!       " << endl;
	cout << "\t\tThe universe is safe thanks to you" << endl << endl;
	cout << "\t\tEnter your name, final score = " << score << endl << "\t\t";
	Sleep(500); // Again, to not skip this message due to key spamming.
	storeScore();
	cout << "\t\tPress any key to go back to the menu." << endl;
	_getch();
}

void updateScore() { // Function to display the score at the correct position on screen.
	setPosition(windowWidth + 6, 17); // Position of the display
	cout << "SCORE = ";
	SetConsoleTextAttribute(hStdOut, 2); // To change its color
	cout << score << endl;
	SetConsoleTextAttribute(hStdOut, 7);
}
void updateLives() { // Function to display the lives at the correct position on screen.
	setPosition(windowWidth + 6, 19); // Position of display
	cout << "LIVES = ";
	SetConsoleTextAttribute(hStdOut, 2); // To change its color
	cout << lives;
	SetConsoleTextAttribute(hStdOut, 7);
}
void updateLevel() { // Function to display the current level at the correct position on screen.
	setPosition(windowWidth + 7, 8); // Position of display
	cout << "Level ";
	SetConsoleTextAttribute(hStdOut, 2); // To change its color
	cout << level;
	SetConsoleTextAttribute(hStdOut, 7);
}

void instructions() { // Function to display instructions. This function is called if the starting key pressed is 2.

	system("cls");
	cout << "I N S T R U C T I O N S\n\n";
	cout << "Use A and D to move around\n";
	cout << "Press spacebar to shoot\n";
	cout << "Press esc to go back to the menu at any time\n";
	cout << "You have three lives\n";
	cout << "There are five total levels\n";
	cout << "Don't get hit by the enemies\n";
	cout << "Goodluck!\n";
	cout << "\n\nPress any key to go back to menu";
	_getch();
}

void play() { // The actual game function. All processes run in this.

	playerPos = -1 + windowWidth / 2; // The player is positioned in the center of the screen.
	score = 0;
	lives = 3;
	timeCounter = 0; // This variable is used to control the speed of the enemies
	level = 1;
	numEnemies = 2; // Initially, only 2 enemies are displayed in level 1.

	for (int i = 0; i < 20; i++)
	{
		enemyFlag[i] = 1; // All the enemies are activated
		enemyY[i] = 4; // All the enemies are located at Y coordinate 4. Their X values are randomized.
	}
	for (int i = 0; i < 20; i++) {
		bullet[i][0] = bullet[i][1] = 0; // So that the bullets are not displayed
	}

	system("cls");
	drawBorder(); // Draws the border of the game window
	for (int i = 0; i < 20; i++)
	{
		generateEnemy(i); // Generates the actual enemies. Note that this function runs 20 times but only 2 enemies are displayed at first
	}
	updateScore(); // The next three functions are used to display score, lives, and level on screen.
	updateLives(); // Seperate functions were made for these three because they need to be constantly updated.
	updateLevel();

	setPosition(windowWidth + 6, 2);
	cout << "S P " << char(202) << " C E"; // To write some information on the rightside of the screen.
	setPosition(windowWidth + 4, 4);
	cout << "S H O O T E R";
	setPosition(windowWidth, 6);
	for (int i = 0; i < 20; i++)
	{
		cout << char(178); // To add partitions in the information panel on the right side of the screen.
	}
	setPosition(windowWidth, 10);
	for (int i = 0; i < 20; i++)
	{
		cout << char(178);
	}

	setPosition(10, 5);
	cout << "Press any key to start"; // Initial pause before the game begins
	_getch();
	setPosition(10, 5);
	cout << "                      ";

	while (1) { // This is the loop that controls the game. While this loop runs, the game runs
		if (_kbhit()) {
			char ch = _getch();
			if (ch == 'a' || ch == 'A') { // If A is pressed, the player moves 2 places to the left.
				if (playerPos > 2)
					playerPos -= 2;
			}
			if (ch == 'd' || ch == 'D') { // If D is pressed, the player moves 2 places to the right.
				if (playerPos < windowWidth - 7)
					playerPos += 2;
			}
			if (ch == 32) { // If spacebar is pressed, the player shoots a bullet.
				generateBullet();
			}
			if (ch == 27) { // If esc is pressed, the player goes back to menu.
				break;
			}
		}

		if (score == 10) // If score is 10, game progresses to level 2, number of enemies increases to 6, and they have 2 speed.
		{
			level = 2;
			numEnemies = 6;
			timeCounter++;
		}
		if (score == 20) // If score is 20, game progresses to level 3, number of enemies increases to 10, and they have 4 speed.
		{
			level = 3;
			numEnemies = 10;
			timeCounter += 3;
		}
		if (score == 30) // If score is 30, game progresses to level 4, number of enemies increases to 15, and they have 6 speed.
		{
			level = 4;
			numEnemies = 15;
			timeCounter += 5;
		}
		if (score == 40) // If score is 40, game progresses to level 5, number of enemies increases to 20, and they have 8 speed.
		{
			level = 5;
			numEnemies = 20;
			timeCounter += 7;
		}
		if (score == 50) // If score is 50, the user wins and the victory message is displayed. Note that these level functions were put in loop so they are constantly checked.
		{
			victory();
			return;
		}

		updateLevel(); // To constantly display the updated level of the game on the screen.
		drawPlayer(); // To constantly draw the player on the correct locations whenever the spaceship is moved
		for (int i = 0; i < numEnemies; i++) // To constantly draw the enemies as they move. This loop runs till number of enemies. That way, only 6 enemies display on level 2.
		{
			drawEnemy(i);
		}
		drawBullet();
		if (collision() == 1) { // If enemy spaceship collides with player spaceship.
			lives--; // Lives decrease by 1 and the updated lives are displayed
			updateLives();
			for (int i = 0; i < numEnemies; i++) // All the enemies are reset back to the top of screen
			{
				resetEnemy(i);
			}
			if (lives == 0) // If the player runs out of their 3 lives, the game ends in a loss.
			{
				gameover();
				return;
			}
		}
		if (bulletHit() == 1) { // If the bullet hits the enemy
			score++; // Score increased by 1 and the updated score is displayed
			updateScore();
		}
		Sleep(200); // To add some delay so that the game does not lag and work smoothly
		erasePlayer(); // To erase the player. Because the player is redrawn whenever it moves, the previous player must be erased.
		for (int i = 0; i < numEnemies; i++) // Following the same logic, the enemies must also be constantly erased
		{
			eraseEnemy(i);
		}
		eraseBullet(); // Following same logic, the bullets must also be erased otherwise there would be a line of bullets from single press.
		moveBullet(); // To move the bullet up the screen

		timeCounter++; // To control the speed of the enemies. Speed is 1 at level 1, 2 at level 2, 3 at level 3, 4 at level 4, and 6 at level 5.

		for (int i = 0; i < numEnemies; i++)
		{
			if (enemyFlag[i] == 1 && timeCounter % 5 == 0) // For example, at level 1, the player can move 5 places before the enemies move once.
			{
				enemyY[i] += 1;
			}
		}

		for (int i = 0; i < numEnemies; i++)
		{
			if (enemyY[i] > screenHeight - 5) { // If the enemy reaches the bottom of the screen, it is reset to the top.
				resetEnemy(i);
			}
		}
	}
}

int main()
{
	// The following code displays an initial animation when the game is started. It was taken from the project support file.
	SetConsoleTextAttribute(hStdOut, 2);
	system("cls");
	cout << endl << endl << endl;
	cout << "           .................................................." << endl;
	cout << "           .................................................." << endl;
	cout << "           . .                                 " << char(254) << "          . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                Space Shooter               . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                 " << char(15) << "          . ." << endl;
	cout << "           . .                                 " << char(202) << "          . ." << endl;
	cout << "           .................................................." << endl;
	cout << "           .................................................." << endl;
	cout << endl << endl;
	Sleep(200);

	SetConsoleTextAttribute(hStdOut, 3);
	system("cls");
	cout << endl << endl << endl;
	cout << "           .................................................." << endl;
	cout << "           .................................................." << endl;
	cout << "           . .                                 " << char(254) << "          . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                Space Shooter               . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                 " << char(15) << "          . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                 " << char(202) << "          . ." << endl;
	cout << "           .................................................." << endl;
	cout << "           .................................................." << endl;
	cout << endl << endl;
	Sleep(200);

	SetConsoleTextAttribute(hStdOut, 4);
	system("cls");
	cout << endl << endl << endl;
	cout << "           .................................................." << endl;
	cout << "           .................................................." << endl;
	cout << "           . .                                 " << char(254) << "          . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                Space Shooter               . ." << endl;
	cout << "           . .                                 " << char(15) << "          . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                 " << char(202) << "          . ." << endl;
	cout << "           .................................................." << endl;
	cout << "           .................................................." << endl;
	cout << endl << endl;
	Sleep(200);

	SetConsoleTextAttribute(hStdOut, 2);
	system("cls");
	cout << endl << endl << endl;
	cout << "           .................................................." << endl;
	cout << "           .................................................." << endl;
	cout << "           . .                                 " << char(254) << "          . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                Space Shooter    " << char(15) << "          . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                 " << char(202) << "          . ." << endl;
	cout << "           .................................................." << endl;
	cout << "           .................................................." << endl;
	cout << endl << endl;
	Sleep(200);

	SetConsoleTextAttribute(hStdOut, 3);
	system("cls");
	cout << endl << endl << endl;
	cout << "           .................................................." << endl;
	cout << "           .................................................." << endl;
	cout << "           . .                                 " << char(254) << "          . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                 " << char(15) << "          . ." << endl;
	cout << "           . .                Space Shooter               . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                 " << char(202) << "          . ." << endl;
	cout << "           .................................................." << endl;
	cout << "           .................................................." << endl;
	cout << endl << endl;
	Sleep(200);

	SetConsoleTextAttribute(hStdOut, 4);
	system("cls");
	cout << endl << endl << endl;
	cout << "           .................................................." << endl;
	cout << "           .................................................." << endl;
	cout << "           . .                                 " << char(254) << "          . ." << endl;
	cout << "           . .                                 " << char(15) << "          . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                Space Shooter               . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                            . ." << endl;
	cout << "           . .                                 " << char(202) << "          . ." << endl;
	cout << "           .................................................." << endl;
	cout << "           .................................................." << endl;
	cout << endl << endl;
	Sleep(500);

	SetConsoleTextAttribute(hStdOut, 7);
	system("cls");

	do { // After the animation, the game starting menu is displayed
		system("cls");
		setPosition(10, 5); cout << " -------------------------- ";
		setPosition(10, 6); cout << "|  S P " << char(202) << " C E S H O O T E R | ";
		setPosition(10, 7); cout << " --------------------------";
		setPosition(10, 9); cout << "1. Start Game";
		setPosition(10, 10); cout << "2. Instructions";
		setPosition(10, 11); cout << "3. Quit";
		setPosition(10, 13); cout << "Select option: ";
		char option = _getche();

		if (option == '1') play(); // If 1 is pressed, the game starts
		else if (option == '2') instructions(); // If 2 is pressed, the instructions are shown
		else if (option == '3') exit(0); // If 3 is pressed, the game exits

	} while (1);

	return 0;
}
