#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>

class Cell : public sf::Drawable
{
public:
    enum class Wall: unsigned char
    {
        TOP     =  0,
        BOTTOM  =  1,
        LEFT    =  2,
        RIGHT   =  3
    };

    enum Mark
    {
        START   = 0,
        END     = 1,
        CELL    = 2
    };

public:
    Cell();
    Cell(int x, int y, int length, int thickness, sf::Color color = sf::Color::White);

    // Checkers&Getters
    inline int  getRow()    const { return mRow; }
    inline int  getCol()    const { return mCol; }
    inline bool isVisited() const { return mIsVisited; }
    Cell*       checkNeighbors(Cell** grid, int maxRows, int maxCols);

    // Modifiers
    void        destroy(Wall direction);
    void        visit(Mark mark = CELL);

    // Operator overloads
    bool        operator==(const Cell& right);
    bool        operator!=(const Cell& right);

private:
    void        initWalls(sf::Color color);
    void        draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    int mX, mY;                 // Cell window coordinates
    int mRow, mCol;             // Cell grid coordinates
    int mLength, mThickness;    // Cell wall length and thickness

    bool  mIsWallDestroyed[4];
    bool  mIsVisited;

    sf::RectangleShape mTopWall,
                       mBottomWall,
                       mLeftWall,
                       mRightWall;

    sf::RectangleShape mMarker;
    sf::Clock mClock;           // To make better randomization

};