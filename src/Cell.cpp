#include "Cell.h"

void destroyWalls(Cell* current, Cell* next);

Cell::Cell()
: mIsVisited(false),
  mIsWallDestroyed {false, false, false, false}
{
}

Cell::Cell(int x, int y, int length, int thickness, sf::Color color)
: mIsVisited(false),
  mX(x),
  mY(y),
  mLength(length),
  mThickness(thickness),
  mIsWallDestroyed {false, false, false, false}
{
    mRow = x/length;
    mCol = y/length;
    initWalls(color);
}

void Cell::destroy(Wall direction)
{
    switch(direction)
    {
        case(Wall::TOP):
            mIsWallDestroyed[(int)Wall::TOP] = true;
            break;

        case(Wall::BOTTOM):
            mIsWallDestroyed[(int)Wall::BOTTOM] = true;
            break;

        case(Wall::LEFT):
            mIsWallDestroyed[(int)Wall::LEFT] = true;
            break;

        case(Wall::RIGHT):
            mIsWallDestroyed[(int)Wall::RIGHT] = true;
            break;
    }
}

void Cell::visit(Mark mark)
{
    mIsVisited = true;

    if(mark == START){
        mMarker.setPosition(mX+ mLength/4, mY + mLength/4);
        mMarker.setSize(sf::Vector2f(mLength/2, mLength/2));
        mMarker.setFillColor(sf::Color::Green);
    }
    if(mark == END){
        mMarker.setPosition(mX+ mLength/4, mY + mLength/4);
        mMarker.setSize(sf::Vector2f(mLength/2, mLength/2));
        mMarker.setFillColor(sf::Color::Red);
    }

}

Cell* Cell::checkNeighbors(Cell** grid, int maxRows, int maxCols)
{
    // Returns a vector with unvisited neighbors
    std::vector<Cell*> neighbors;
    srand(mClock.getElapsedTime().asMicroseconds());

    // If column is the uppermost don't check for top
    if(mCol != 0 ) {

        Cell* top = &grid[mRow][mCol-1];
        if(!top->isVisited()) {
            neighbors.push_back(top);
        }
    }

    // If column is the downmost don't check for bottom
    if(mCol != maxCols - 1) {

        Cell* bottom = &grid[mRow][mCol+1];
        if(!bottom->isVisited()){
            neighbors.push_back(bottom);
        }
    }

    // If row is the leftmost don't check for left
    if(mRow != 0){
        Cell* left = &grid[mRow-1][mCol];
        if( !left->isVisited()){
            neighbors.push_back(left);
        }
    }

    // If row is the rightmost don't check for right
    if(mRow != maxRows - 1){
        Cell* right = &grid[mRow+1][mCol];
        if( !right->isVisited()){
            neighbors.push_back(right);
        }
    }

    // If there are unvisited neighbors
    if(!neighbors.empty())
    {

        int i = rand() % neighbors.size();
        return neighbors[i];
    }

    return this;

}

bool Cell::operator==(const Cell& right)
{
    return mRow == right.mRow && mCol == right.mCol;
}

bool Cell::operator!=(const Cell& right)
{
    return !(mRow == right.mRow && mCol == right.mCol);
}

void Cell::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!mIsWallDestroyed[(int)Wall::TOP])
        target.draw(mTopWall, states);

    if(!mIsWallDestroyed[(int)Wall::BOTTOM])
        target.draw(mBottomWall, states);

    if(!mIsWallDestroyed[(int)Wall::LEFT])
        target.draw(mLeftWall, states);

    if(!mIsWallDestroyed[(int)Wall::RIGHT])
        target.draw(mRightWall, states);

    if(mIsVisited)
        target.draw(mMarker, states);
}

void Cell::initWalls(sf::Color color)
{
    mTopWall.setPosition(mX, mY);
    mBottomWall.setPosition(mX + mLength, mY + mLength);
    mLeftWall.setPosition(mX, mY + mLength);
    mRightWall.setPosition(mX + mLength, mY);

    mTopWall.setSize(sf::Vector2f(mLength, mThickness));
    mBottomWall.setSize(sf::Vector2f(-mLength, mThickness));
    mLeftWall.setSize(sf::Vector2f(mThickness, -mLength));
    mRightWall.setSize(sf::Vector2f(mThickness, mLength + mThickness));

    mTopWall.setFillColor(color);
    mBottomWall.setFillColor(color);
    mLeftWall.setFillColor(color);
    mRightWall.setFillColor(color);
}

// Helper function
void destroyWalls(Cell* current, Cell* next)
{
    if(current->getRow() < next->getRow()){
        current->destroy(Cell::Wall::RIGHT);
        next->destroy(Cell::Wall::LEFT);
    }
    if(current->getRow() > next->getRow()) {
        current->destroy(Cell::Wall::LEFT);
        next->destroy(Cell::Wall::RIGHT);
    }

    if(current->getCol() < next->getCol()){
        current->destroy(Cell::Wall::BOTTOM);
        next->destroy(Cell::Wall::TOP);
    }
    if(current->getCol() > next->getCol()){
        current->destroy(Cell::Wall::TOP);
        next->destroy(Cell::Wall::BOTTOM);
    }
}
