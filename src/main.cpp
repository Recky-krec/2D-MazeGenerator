#include <iostream>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "Cell.h"

/*
 * 2D Maze Generator created by Krec:
 * Green square represents the start and red square represents the end
 * It is recommend to let AUTOMATIC set to 1. This way you can linearly control the maze by just inputing a number [1-100]
 * If AUTOMATIC is set to 0, you can configure the maze in the variables bellow (read the comments)
 * Be aware that it is possible for the maze creator to fail to create a maze and stop in the process (very rare)
 */

#define AUTOMATIC 1

// Function prototypes
void destroyWalls(Cell* current, Cell* next);

int main()
{
    // Manually Configuring Window and Grid (AUTOMATIC must be 0)
    // ---------------------------
    int difficulty            = 100;                               // Defines how close the end is from the start
    int windowWidth           = 600;                               // It is recommend to make the windowWidth...
    int windowHeight          = 600;                               //...the same as the window height
    float wallLength          = 30;                                // The lower this value, the bigger the maze
    int wallThickness         = 2;                                 // The thickness of the wall
    sf::Color wallColor       = sf::Color::White;                  // The color of the wall
    sf::Color backgroundColor = sf::Color(55, 55, 55, 255);        // The background color (RGBA)
    int highestRow            = (int)(windowWidth / wallLength);   // wallLength and windowWidth have to be divisible...
    int highestCol            = (int)(windowHeight / wallLength);  //...by otherwise it segfaults
    const int FPS             = 300;
    bool isPaused             = false;

#if AUTOMATIC
    // Get difficulty input
    // --------------------
    std::cout << "Difficulty [1-100]: " << std::endl;
    std::cin >> difficulty;
    if(difficulty<1 || difficulty>100) throw std::runtime_error("Out of range Difficulty value");
    if(difficulty<15)
        difficulty = 15;


    // Configure Grid and window attributes
    // -----------------------------------.
    wallLength     = 1000 / difficulty;

    if( int(std::ceil(wallLength)) % 2 == 0)                       // If the ceil of wallLength is divisible by two, assign it
        wallLength = std::ceil(wallLength);

    if( int(std::floor(wallLength)) % 2 == 0)                      // If the floor of wallLength is divisible by two, assign it
        wallLength = std::floor(wallLength);                       // In other words, we wan't to make sure wallLength is divisible by two

    windowWidth     = 10 * (difficulty+15);                        // Being the maze bigger, we want...
    windowHeight    = 10 * (difficulty+15);                        //... the window to be bigger
    highestRow      = (int)std::ceil(windowWidth / wallLength);
    highestCol      = (int)std::ceil(windowHeight / wallLength);
#endif

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "2D-MazeGenerator");
    window.setFramerateLimit(FPS);

    // Creating Grid
    // ---------------------------
    Cell** grid = new Cell* [highestRow];
    for(int i = 0; i < highestRow; i++)
    {
        grid[i] = new Cell[highestCol];
    }

    for(int x = 0; x < windowWidth; x += wallLength) {
        for (int y = 0; y < windowHeight; y += wallLength) {
            int row = x / wallLength;
            int column = y / wallLength;

            grid[row][column] = (Cell(x, y, wallLength, wallThickness, wallColor));
        }
    }


    // Variables used in game loop
    // ---------------------------
    Cell* currentCell = &grid[0][0];                // Make the initial cell the current cell...
    currentCell->visit(Cell::START);                //...and mark it as visited
    Cell* nextCell;
    std::stack<Cell*> backtrackCells;


    difficulty = difficulty < 30 ? 30 : difficulty; // Used to...
    difficulty = difficulty > 97 ? 95 : difficulty; //...prevent bugs
    float multiplier = (difficulty/100.0f);         // The higher the difficulty, the further is the end
    // ---------------------------

    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case (sf::Event::Closed):
                    window.close();
                    break;
            }

            switch(event.key.code) {
                case (sf::Keyboard::P):
                    isPaused = true;
                    break;
                case (sf::Keyboard::O):
                    isPaused = false;
                    break;
            }
        }

        // Update
        // ------
        if(!isPaused)
        {

            //1. If the current cell has any neighbours which have not been visited, choose one randomly
            nextCell = currentCell->checkNeighbors(grid, highestRow, highestCol);

            // If i didn't get stuck
            if(nextCell != currentCell){
                //2. Push the current cell to the stack
                backtrackCells.push(currentCell);

                //3. Remove the wall between the current cell and the chosen cell
                destroyWalls(currentCell, nextCell);

                //4. Make the chosen cell the current cell and mark it as visited
                // Choose end based on the difficulty
                currentCell = nextCell;
                if(currentCell->getRow() == std::floor( (highestRow-1) * multiplier) && currentCell->getCol() == std::floor( (highestCol-1) * multiplier))
                    currentCell->visit(Cell::END);

                currentCell->visit();

            // If i got stuck and my stack isn't empty, backtrack until a cell with unvisited neighbors
            } else if(!backtrackCells.empty()) {
                //1. Pop a cell from the stack and make it the current cell
                currentCell = backtrackCells.top();
                backtrackCells.pop();

            // Stop processing when maze is done
            } else {
                isPaused = true;
            }

        }

        // Render
        // ------
        window.clear(backgroundColor);

        for(int row = 0; row < highestRow; row++)
        {
            for(int col = 0; col < highestCol; col++)
            {
                window.draw(grid[row][col]);
            }
        }
        window.display();
    }
    return 0;
}