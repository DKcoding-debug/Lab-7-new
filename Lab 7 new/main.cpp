/*
	Delvis Kouete
	Computer Science Fall 2024
	Due: Dec. 17, 2024
	Lab Name: Lab 7 Battleship
	For this lab you will make a simple battleship game.
	*/

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
// Constants defining grid size and number of ships
const int GRID_SIZE = 10;
const int NUM_SHIPS = 5;
// Constants representing different grid elements 
const char WATER = '~';
const char SHIP = 'S';
const char HIT = 'X';
const char MISS = 'O';

// Function declarations
void initializeGrid(char grid[GRID_SIZE][GRID_SIZE]);
void printGrid(const char grid[GRID_SIZE][GRID_SIZE]);
void placeComputerShips(char grid[GRID_SIZE][GRID_SIZE]);
bool placeUserShip(char grid[GRID_SIZE][GRID_SIZE], int x, int y, int size, bool vertical);
bool fire(char grid[GRID_SIZE][GRID_SIZE], int x, int y);
bool checkVictory(char grid[GRID_SIZE][GRID_SIZE]);
bool isValid(int x, int y);
void displayGameStatus(char unserGrid[GRID_SIZE][GRID_SIZE], char compGrid[GRID_SIZE][GRID_SIZE], int userHits, int compHits);


int main()
{
	srand(time(0)); // Initialize random seed based on the system time

	// Initialize the user's and computer's grids
	char userGrid[GRID_SIZE][GRID_SIZE], compGrid[GRID_SIZE][GRID_SIZE];
	initializeGrid(userGrid);
	initializeGrid(compGrid);

	// Place the computer's ships on the grid
	placeComputerShips(compGrid);

	// Prompt the user to place their ships
	cout << "Place your ships on the grid. \n";
	for (int i = 0; i < NUM_SHIPS; ++i)
	{
		int x, y, size = 3;
		char direction;
		bool vertical;

		printGrid(userGrid); // Display the current user's grid
		cout << "Enter coordiantes (x,y) to place ship #" << i + 1 << " (Size: " << size << "): ";
		cin >> x >> y;
		cout << "Enter direction (v for vertical, h for horizontal): ";
		cin >> direction;
		vertical = (direction == 'v' || direction == 'V');

		// Keep prompting the user until a valid ship placement is made
		while (!placeUserShip(userGrid, x, y, size, vertical))
		{
			cout << "Invalid placement! Try again. \n";
			cout << "Enter coordinates (x,y) to place ship #" << i + 1 << " (Size: " << size << "): ";
			cin >> x >> y;
			cout << "Enter direction (v for vertical, h for horizontal): ";
			cin >> direction;
			vertical = (direction == 'v' || direction == 'V');
		}
	}

	// Initialize hit counters and the game running flag
	int userHits = 0, compHits = 0;
	bool gameRunning = true;

	// Main game loop
	while (gameRunning)
	{
		int x, y;

		// Computer's turn to fire
		x = rand() % GRID_SIZE;
		y = rand() % GRID_SIZE;
		// Keep generating random coordinates until a valid target is found
		while (compGrid[y][x] == HIT || compGrid[y][x] == MISS)
		{
			x = rand() % GRID_SIZE;
			y = rand() % GRID_SIZE;
		}
		// Fire at the selected coordinates
		if (fire(userGrid, x, y))
		{
			cout << "Computer fired at (" << x << ", " << y << ") and HIT! \n";
			compHits++;
		}
		else
		{
			cout << "Computer fired at (" << x << ", " << y << ") and MISSED.\n";
		}

		// Check if the user has lost all ships
		if (checkVictory(userGrid))
		{
			cout << "The computer wins! All your ships are destroyed.\n";
			gameRunning = false;
			break;
		}

		// User's turn to fire
		cout << "Your turn! Enter coordinates to fire (x,y): ";
		cin >> x >> y;
		// Keep promting until the user enters valid coordinates that haven't been fired at before
		while (!isValid(x, y) || userGrid[y][x] == HIT || userGrid[y][x] == MISS)
		{
			cout << "Invalid or already fired coordinates. Try again: ";
			cin >> x >> y;
		}
		// Fire at the selected coordinates
		if (fire(compGrid, x, y))
		{
			cout << "You fired at (" << x << ", " << y << ") and HIT!\n";
			userHits++;
		}
		else
		{
			cout << "You fired at (" << x << ", " << y << ") and MISSED.\n";
		}

		// Check if the computer has lost all ships
		if (checkVictory(compGrid))
		{
			cout << "You win! All computer ships are destroyed.\n";
			gameRunning = false;
			break;
		}

		// Display the current game status
		displayGameStatus(userGrid, compGrid, userHits, compHits);
	}
	return 0;
}

// Function to initialize the game grid with water
void initializeGrid(char grid[GRID_SIZE][GRID_SIZE])
{
	for (int i = 0; i < GRID_SIZE; ++i)
	{
		for (int j = 0; j < GRID_SIZE; ++j)
		{
			grid[i][j] = WATER;
		}
	}
}

// function to print the grid to the console
void printGrid(const char grid[GRID_SIZE][GRID_SIZE])
{
	cout << " ";
	for (int i = 0; i < GRID_SIZE; ++i)
	{
		cout << i << " ";
	}
	cout << endl;

	// print each row of the grid
	for (int i = 0; i < GRID_SIZE; ++i)
	{
		cout << i << " "; // Print row number
		for (int j = 0; j < GRID_SIZE; ++j)
		{
			cout << grid[i][j] << " "; // Print cell content
		}
		cout << endl;
	}
}

// Function to place the computer's ships randomly
void placeComputerShips(char grid[GRID_SIZE][GRID_SIZE])
{
	int shipsPlaced = 0;
	while (shipsPlaced < NUM_SHIPS)
	{
		int x = rand() % GRID_SIZE;
		int y = rand() % GRID_SIZE;
		bool vertical = rand() % 2;

		// Attempt to place a ship of a size 3 at a random location
		if (placeUserShip(grid, x, y, 3, vertical))
		{
			shipsPlaced++; // Increment the number of ships placed if successful
		}
	}
}

// Function to place a ship on the user's grid
bool placeUserShip(char grid[GRID_SIZE][GRID_SIZE], int x, int y, int size, bool vertical)
{
	if (vertical)
	{
		if (y + size > GRID_SIZE) return false; // Check id the ship fits vertically
		for (int i = 0; i < size; ++i)
		{
			if (grid[y + i][x] == SHIP) return false; // Check if the position is already occupied
		}
		for (int i = 0; i < size; ++i)
		{
			grid[y + i][x] = SHIP; // Place the ship vertically
		}
	}
	else
	{
		if (x + size > GRID_SIZE) return false; // Check if the ship fits horizontally
		for (int i = 0; i < size; ++i)
		{
			if (grid[y][x + i] == SHIP) return false; // Check if the postion is already occupied
		}
		for (int i = 0; i < size; ++i)
		{
			grid[y][x + i] = SHIP; // Place the ship horizontally
		}
	}
	return true; // Return true if the shi[ was placed successfully
}

// Function to fire at a specific location on the grid
bool fire(char grid[GRID_SIZE][GRID_SIZE], int x, int y)
{
	if (grid[y][x] == SHIP)
	{
		grid[y][x] = HIT; // Mark the cell as hit if it contain a ship
		return true;
	}
	else
	{
		grid[y][x] = MISS; // Mark the cell as missed if it doesn't contain a ship
		return false;
	}
}

// Fucntion to check if all ships are destroyed on the grid
bool checkVictory(char grid[GRID_SIZE][GRID_SIZE])
{
	for (int i = 0; i < GRID_SIZE; ++i)
	{
		for (int j = 0; j < GRID_SIZE; ++j)
		{
			if (grid[i][j] == SHIP) return false; // If any ship remains, no victory yet
		}
	}
	return true; // All ships are destroyed
}

//Function to validate the coordinates entered by the user
bool isValid(int x, int y)
{
	return (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE); // Ensure coordinates are within grid bounds
}

// Function display the game status, showing both grids and the number of hits
void displayGameStatus(char userGrid[GRID_SIZE][GRID_SIZE], char compGrid[GRID_SIZE][GRID_SIZE], int userHits, int compHits)
{
	cout << "\nUser's grid:\n";
	printGrid(userGrid);
	cout << "\nComputer's grid:\n";
	printGrid(compGrid);
	cout << "User Hits: " << userHits << " | Computer Hits: " << compHits << endl;
}