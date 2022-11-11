#include "AlgorithmMatrix.h"
#include "Coordinates.h"
#include "Tile.h"
#include <vector>
#include <iostream>

namespace ttt
{
    AlgorithmMatrix::AlgorithmMatrix(Board const &board, std::string const& playerName) // The constructor creates the semantic matrix that will be used in the Alpha Beta Algorithm.
    : mWidth(board.width()) , mHeight(board.height()) , mWinCondition(board.winCondition())
    {
        mMatrix.resize(mWidth, std::vector<int>(board.height()));
        for (int i = 0; i < mWidth; i++)
        {
            for (int j = 0; j < mHeight; j++)
            {
                Coordinates c{i, j};
                mMatrix[i][j] = (int) TokenValues::emptyToken; // I Asume that the tile is empty.
                if (board.tile(c)->owner().has_value())
                {
                    if (playerName == board.tile(c)->owner()->get().name()) // Depending on the owner of our tile, the function will put the token asociated.
                    {
                        mMatrix[i][j] = (int) TokenValues::currentPlayerToken;
                    }
                    else
                    {
                        mMatrix[i][j] = (int) TokenValues::rivalToken;
                    }
                }
            }
        }
    }

    int AlgorithmMatrix::getWidth() const
    {
        return mWidth;
    }

    int AlgorithmMatrix::getHeight() const
    {
        return mHeight;
    }

    void AlgorithmMatrix::executeMovement(Coordinates const &c, bool actualPlayer) // Function that execute a movement.
    {
        mMatrix[c.x][c.y] = actualPlayer? (int) TokenValues::currentPlayerToken : (int) TokenValues::rivalToken;
    }

    void AlgorithmMatrix::undoMovement(Coordinates const &c) // Function that undo a movement.
    {
        mMatrix[c.x][c.y] = (int) TokenValues::emptyToken;
    }

    std::vector<Coordinates> AlgorithmMatrix::getMovs() // We get all the posible moves to make in the board.
    {
        std::vector<Coordinates> output;
        output.clear();
        for (int i = 0; i < mWidth; i++)
        {
            for (int j = 0; j < mHeight; j++)
            {
                if (mMatrix[i][j] == (int) TokenValues::emptyToken)
                {
                    output.push_back(Coordinates{i, j});
                }
            }
        }
        return output;
    }

    // Private void that detects if a victory occurred. In addition, it initializes all the variables of trivialGameOver.

    void AlgorithmMatrix::checkVictory(int &actualGameState, int &i, int &j, int &iCapturated, int &jCapturated, int &amountOfEqualToken, bool &moveToNextWinControl)
    {
        if (amountOfEqualToken == mWinCondition)
        {
            if (mMatrix[i][j] == (int) TokenValues::currentPlayerToken)
            {
                actualGameState =(int)  GameStatesValues::currentPlayerWins;
            }
            else
            {
                actualGameState = (int) GameStatesValues::currentPlayerDefeated;
            }
        }
        moveToNextWinControl = false;
        amountOfEqualToken = 1;
        i = iCapturated;
        j = jCapturated;
    }

/*
    int AlgorithmMatrix::trivialGameOver() // Function that seeks a victory.
    {
        bool fullBoard = true; // We asume the Tie Game Over.
        bool moveToNextWinControl = false;
        int actualGameState = (int) GameStatesValues::theGameIsNotOverYet; // If the variable gets a value that is different of the partial state value, a victory has cocurred!
        for (int i = 0; i < mWidth; i++)
        {
            for (int j = 0; j < mHeight; j++)
            {
                int iCapturated = i;
                int jCapturated = j;
                int amountOfEqualToken = 1;
                if (i + mWinCondition <= mWidth) // If we have the minimun amount of tiles to check, then the algorithm will proceed to  check the actual row.
                {
                    while (!moveToNextWinControl && amountOfEqualToken < mWinCondition && i + 1 < mWidth)
                    {
                        if (mMatrix[i][j] == mMatrix[i + 1][j] && mMatrix[i][j] != (int) TokenValues::emptyToken)
                        {
                            amountOfEqualToken++;
                            i++;
                        }
                        else
                        {
                            moveToNextWinControl = true; // If this is true, then on this path there is no longer a possible victory. We must continue to the next control.
                        }
                    }
                    checkVictory(actualGameState, i, j, iCapturated, jCapturated, amountOfEqualToken, moveToNextWinControl);
                }

                if (j + mWinCondition <= mHeight && actualGameState == (int) GameStatesValues::theGameIsNotOverYet) // For now one,  we also ask that if there is no victory yet. Column check
                {
                    while (!moveToNextWinControl && amountOfEqualToken < mWinCondition && j + 1 < mHeight)
                    {
                        if (mMatrix[i][j] == mMatrix[i][j + 1] && mMatrix[i][j] != (int) TokenValues::emptyToken)
                        {
                            amountOfEqualToken++;
                            j++;
                        }
                        else
                        {
                            moveToNextWinControl = true;
                        }
                    }
                    checkVictory(actualGameState, i, j, iCapturated, jCapturated, amountOfEqualToken, moveToNextWinControl);
                }
                if (j + mWinCondition <= mHeight && i + mWinCondition <= mWidth && actualGameState == (int) GameStatesValues::theGameIsNotOverYet) // We check the 2 possible diagonals wins.
                {
                    j = j + mWinCondition - 1;
                    while (!moveToNextWinControl && amountOfEqualToken < mWinCondition && j > 0 && i + 1 < mWidth) // Increasing Diagonal {Q1  to  Q4.}
                    {
                        if (mMatrix[i][j] == mMatrix[i + 1][j - 1] && mMatrix[i][j] != (int)  TokenValues::emptyToken)
                        {
                            amountOfEqualToken++;
                            j--;
                            i++;
                        }
                        else
                        {
                            moveToNextWinControl = true;
                        }
                    }
                    checkVictory(actualGameState, i, j, iCapturated, jCapturated, amountOfEqualToken, moveToNextWinControl);

                    while (!moveToNextWinControl && amountOfEqualToken < mWinCondition && j + 1 < mHeight && i + 1 < mWidth && actualGameState == (int) GameStatesValues::theGameIsNotOverYet) // Decreasing Diagonal {Q2 to Q4}.
                    {
                        if (mMatrix[i][j] == mMatrix[i + 1][j + 1] && mMatrix[i][j] != (int) TokenValues::emptyToken)
                        {
                            amountOfEqualToken++;
                            j++;
                            i++;
                        }
                        else
                        {
                            moveToNextWinControl = true;
                        }
                    }
                    checkVictory(actualGameState, i, j, iCapturated, jCapturated, amountOfEqualToken, moveToNextWinControl);
                }
                if ((mMatrix[i][j]) == (int) TokenValues::emptyToken) // If we found at least one tile that isn't empty, then there is no tie yet!.
                {
                    fullBoard = false;
                }
                if (actualGameState != (int) GameStatesValues::theGameIsNotOverYet) // We found a victory!
                {
                    return actualGameState;
                }
            }
        }
        if (fullBoard)
        {
            return (int) GameStatesValues::playersTie; // In this case, we get a Tie Game Over.
        }
        return (int) GameStatesValues::theGameIsNotOverYet; // If there is, at least, one tile empty, the game will be not over yet.
    }

  */ 

bool AlgorithmMatrix::inRange(int i, int j , int iIncrement , int jIncrement)
{
    if (i+iIncrement < mWidth && j+jIncrement < mHeight)
    {
        if (jIncrement<0)
        {
            if (j>0 && j<mHeight)
            {
                return true;
            }
            return false;
        }
        return true;
    }
    return false;
}


int  AlgorithmMatrix::checkAxisVictory(int i , int y, int iIncrement, int jIncrement, int actualGameState)
{  
    int amountOfEqualToken = 1;
    bool moveToNextWinControl = false;
    while (!moveToNextWinControl && amountOfEqualToken < mWinCondition && inRange(i,y,iIncrement,jIncrement))
    {
        if (mMatrix[i][y] == mMatrix[i + iIncrement] [y + jIncrement] && mMatrix[i][y] != (int) TokenValues::emptyToken)
        {
            amountOfEqualToken++;
            i=i+iIncrement;
            y=y+jIncrement;
        }
        else
        {
            moveToNextWinControl = true; // If this is true, then on this path there is no longer a possible victory. We must continue to the next control.
        }
    }
    if (amountOfEqualToken == mWinCondition)
        {
            if (mMatrix[i][y] == (int) TokenValues::currentPlayerToken)
            {
                return actualGameState =(int)  GameStatesValues::currentPlayerWins;
            }
            else
            {
                return actualGameState = (int) GameStatesValues::currentPlayerDefeated;
            }
        }
    return (int) GameStatesValues::theGameIsNotOverYet;
}




    int AlgorithmMatrix::trivialGameOver() // Function that seeks a victory.
    {
        bool fullBoard = true; // We asume the Tie Game Over.
        bool moveToNextWinControl = false;
        int actualGameState = (int) GameStatesValues::theGameIsNotOverYet; // If the variable gets a value that is different of the partial state value, a victory has cocurred!
        for (int i = 0; i < mWidth; i++)
        {

        //j + 0 ; i + 1 Row (actual tile)
        //j + 1 ; i + 0 Collumn (actual tile)
        //j - 1 ; i + 1 Increasing Diagonal {Q1  to  Q4.} With initialization:   j = j + mWinCondition - 1;
        //j + 1 ; i + 1 Decreasing Diagonal {Q2 to Q4} (Actual tile)

            for (int j = 0; j < mHeight; j++)
            {

                actualGameState = checkAxisVictory(i , j , 1 , 0 , actualGameState);    //Row (actual tile)

                if (actualGameState == (int) GameStatesValues::theGameIsNotOverYet)    //Collumn (actual tile)
                {
                    actualGameState = checkAxisVictory(i , j , 0 , 1 , actualGameState);
                }

                if (actualGameState == (int) GameStatesValues::theGameIsNotOverYet)    //Increasing Diagonal {Q1  to  Q4.}
                {
                    int indexCorrection = j + mWinCondition; indexCorrection--;
                    actualGameState = checkAxisVictory(i , j + mWinCondition -1 , 1, -1 , actualGameState);
                }

                if (actualGameState== (int) GameStatesValues::theGameIsNotOverYet)     //Decreasing Diagonal {Q2 to Q4}
                {
                    actualGameState = checkAxisVictory(i , j , 1, 1 , actualGameState);
                }

                if ((mMatrix[i][j]) == (int) TokenValues::emptyToken) // If we found at least one tile that isn't empty, then there is no tie yet!.
                {
                    fullBoard = false;
                }
                if (actualGameState != (int) GameStatesValues::theGameIsNotOverYet) // We found a victory!
                {
                    return actualGameState;
                }
            }
        }
        if (fullBoard)
        {
            return (int) GameStatesValues::playersTie; // In this case, we get a Tie Game Over.
        }
        return (int) GameStatesValues::theGameIsNotOverYet; // If there is, at least, one tile empty, the game will be not over yet.
    }









} // namespace ttt