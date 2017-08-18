#include <iostream>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>

#include "Cell.h"

void destroyWalls(Cell* current, Cell* next);

int main()
{
    //TODO: Command line arguments for width/height of window

    // Configuring Window and Grid
    // ---------------------------
    const int difficulty      = 90;
    const int windowWidth     = 900;
    const int windowHeight    = 900;
    const int wallLength      = 10;
    const int wallThickness   = 2;
    sf::Color wallColor       = sf::Color::White;
    sf::Color backgroundColor = sf::Color(55, 55, 55, 255);
    const int highestRow      = windowWidth / wallLength; // Both the constants have to...
    const int highestCol      = windowHeight / wallLength; //... be divisible by eachother otherwise it segfaults
    const int FPS             = 300;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "MazePrototype");
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

    // Make the initial cell the current cell and mark it as visited
    // ------------------------------------
    Cell* currentCell = &grid[0][0];
    currentCell->visit(Cell::START);        // Starting Cell
    Cell* nextCell;
    std::stack<Cell*> backtrackCells;
    bool isPaused = false;

    float multiplier = (difficulty/100.0f); // Based on the difficulty
    // ------------------------------------

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