
#include "AIShell.h"
#include <stdlib.h>
#include <iostream>
#include <climits>
#include <sys/time.h>
using namespace std;




AIShell::AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove)
{
    this->deadline=0;
    this->numRows=numRows;
    this->numCols=numCols;
    this->gravityOn=gravityOn;
    this->gameState=gameState;
    this->lastMove=lastMove;
    
}


AIShell::~AIShell()
{
    
    //delete the gameState variable.
    for (int i =0; i<numCols; i++){
        delete [] gameState[i];
    }
    delete [] gameState;
    
}

vector<Move> AIShell::getActions() {
    vector<Move> possibleMoves;
    for (int col = 0; col < numCols; col++) {
        for (int row = 0; row < numRows; row++) {
            if (gameState[col][row] == NO_PIECE){
                Move m(col, row);
                possibleMoves.push_back(m);
                if (gravityOn) {
                    break;
                }
            }
        }
    }
    return possibleMoves;
}

int AIShell::getHeuristic() {
    return getRIGHTUtility() + getUPUtility() + getDOWNRIGHTUtility() + getUPRIGHTUtility();
}


int AIShell::alphaBetaSearchMaxValue(int depth, int currentDepth, int alpha, int beta,
                                     long int originalTime, bool *valid, map<string, vector<Move> > &depthVsMoves) {
    if (depth < 0 || depth == 0) {
        return getHeuristic();
    }
    vector<Move> possibleMoves;
    set<Move, CompareMax> setMoves;
    string currentGameStateStr = "";
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < numRows; j++) {
            currentGameStateStr += gameState[i][j] == HUMAN_PIECE ? '0' : (gameState[i][j] == AI_PIECE ? 'X' : ' ');
        }
    }
    
    if(depthVsMoves.end() == depthVsMoves.find(currentGameStateStr)){
        possibleMoves = getActions();
        
    }else if(depthVsMoves.end() != depthVsMoves.find(currentGameStateStr)){
        possibleMoves = depthVsMoves[currentGameStateStr];    }
             
    if (possibleMoves.size() == 0) {
        return 0;
    }
    
    
    for (int i = 0; i < possibleMoves.size(); i++) {
        Move chosenMove = possibleMoves[i];
        gameState[chosenMove.col][chosenMove.row] = AI_PIECE;
        
        if (getTerminalState(chosenMove) == true) {
            gameState[chosenMove.col][chosenMove.row] = NO_PIECE;
            alpha = INT_MAX;
            break;
        }
        
        int alphaChosenMove = alphaBetaSearchMinValue(depth - 1, currentDepth + 1, alpha, beta, originalTime, valid, depthVsMoves);
        
        if (*valid == false) {
            return INT_MIN;
        }
        
        chosenMove.utility = alphaChosenMove;
        setMoves.insert(chosenMove);
        
        struct timeval timeObject;
        gettimeofday(&timeObject, NULL);
        
        
        long int currTime = (timeObject.tv_usec / 1000) + (timeObject.tv_sec * 1000);
        if (deadline < currTime - originalTime ) {
            *valid = false;
            return alpha;
        }
        if (alpha < alphaChosenMove) {
            alpha = alphaChosenMove;
        }
        gameState[chosenMove.col][chosenMove.row] = NO_PIECE;
        
        if (alpha >= beta) {
            return alpha;
        }
        if (alpha == INT_MAX){
            break;
        }
    }
    return alpha;
}

int AIShell::alphaBetaSearchMinValue(int depth, int currentDepth, int alpha, int beta,
                                     long int originalTime, bool *valid, map<string, vector<Move> > &depthVsMoves) {
    
    if (depth < 0 || depth == 0) {
        return getHeuristic();
    }
    vector<Move> possibleMoves;
     set<Move, CompareMin> setMoves;
    string currentGameStateStr = "";
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < numRows; j++) {
            currentGameStateStr += gameState[i][j] == HUMAN_PIECE ? '0' : (gameState[i][j] == AI_PIECE ? 'X' : ' ');
        }
    }
    if(depthVsMoves.end() == depthVsMoves.find(currentGameStateStr)){
        possibleMoves = getActions();
    }else if(depthVsMoves.end() != depthVsMoves.find(currentGameStateStr)){
        possibleMoves = depthVsMoves[currentGameStateStr];    }
    
    if (possibleMoves.size() == 0) {
        return 0;
    }

    for (int i = 0; i < possibleMoves.size(); i++) {
        
        Move chosenMove = possibleMoves[i];
        gameState[chosenMove.col][chosenMove.row] = HUMAN_PIECE;
        if (getTerminalState(chosenMove) == true) {
            gameState[chosenMove.col][chosenMove.row] = NO_PIECE;
            beta = INT_MIN;
            break;
        }
        
        int betaChosenMove = alphaBetaSearchMaxValue(depth - 1, currentDepth + 1, alpha, beta, originalTime, valid, depthVsMoves);
        if (*valid == false) {
            return INT_MAX;
        }
        
        chosenMove.utility = betaChosenMove;
        setMoves.insert(chosenMove);
        
        struct timeval timeObject;
        gettimeofday(&timeObject, NULL);
        
        long int currentTime = (timeObject.tv_sec * 1000) + (timeObject.tv_usec / 1000);
        if (currentTime - originalTime >= deadline) {
            *valid = false;
            return beta;
        }
        
        
        if (betaChosenMove < beta) {
            beta = betaChosenMove;
        }
        gameState[chosenMove.col][chosenMove.row] = NO_PIECE;
        if (alpha >= beta) {
            return beta;
        }
        if (beta == INT_MIN) {
            break;}
    }
    return beta;
}

Move AIShell::alphaBetaSearch(int maxDepth, map<string, vector<Move> > &depthVsMoves, long int originalTime) {
    vector<Move> possibleMoves;
    string currentGameStateStr = "";
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < numRows; j++) {
            currentGameStateStr += gameState[i][j] == HUMAN_PIECE ? '0' : (gameState[i][j] == AI_PIECE ? 'X' : ' ');
        }
    }
    if(depthVsMoves.end() == depthVsMoves.find(currentGameStateStr)){
        possibleMoves = getActions();
        cout << "Created " << possibleMoves.size() << " moves for depth 1\n";

    }else if(depthVsMoves.end() != depthVsMoves.find(currentGameStateStr)){
        possibleMoves = depthVsMoves[currentGameStateStr];
        cout << "Found " << possibleMoves.size() << " moves for depth 1\n";
    }

    Move maxMove = possibleMoves[0];

    bool valid = true;
    set<Move, CompareMax> orderedMoves;
        int alpha = INT_MIN;
        int beta = INT_MAX;
        
    for (int i = 0; i < possibleMoves.size(); i++) {
        Move chosenMove = possibleMoves[i];
        gameState[chosenMove.col][chosenMove.row] = AI_PIECE;
        if (getTerminalState(chosenMove) == true) {
            gameState[chosenMove.col][chosenMove.row] = NO_PIECE;
            maxMove.row = chosenMove.row;
            maxMove.col = chosenMove.col;
            break;
        }
        
        int alphaChosenMove = alphaBetaSearchMinValue(maxDepth - 1, 1, alpha, beta, originalTime, &valid, depthVsMoves);
        if (valid != true) {
            Move noMove(-1, -1);
            return noMove;
        }
        
        chosenMove.utility = alphaChosenMove;
        orderedMoves.insert(chosenMove);
        
        struct timeval timeObject;
        gettimeofday(&timeObject, NULL);
        
        
        long int currentTime = (timeObject.tv_sec * 1000) + (timeObject.tv_usec / 1000);
        if (currentTime - originalTime >= deadline) {
            return Move(-1, -1);
        }
        
        
        if (alphaChosenMove > alpha) {
            alpha = alphaChosenMove;
            maxMove.col = chosenMove.col;
            maxMove.row = chosenMove.row;
        }
        
        gameState[chosenMove.col][chosenMove.row] = NO_PIECE;
        if (alpha == INT_MAX) {
            break;}
    }
    return maxMove;
}

int AIShell::last_IDS_Depth = 2;
Move AIShell::IDS() {
    struct timeval timeObject;
    gettimeofday(&timeObject, NULL);
    long int originalTime = (timeObject.tv_sec * 1000) + (timeObject.tv_usec / 1000);
    
    int depthToTry = last_IDS_Depth - 1;
    Move m;
    bool deadlinePassed = false;
    deadline = deadline - 300;
    
    
    map<string, vector<Move> > depthVsMoves;
    
    do {
        cout << "Current depth: " << depthToTry << "\n";
        Move currentMove = alphaBetaSearch(depthToTry, depthVsMoves, originalTime);
        if (currentMove.row == -1) {
            cout << "Returning from error state\n";
            return lastIDSMove;
        }
        
        gettimeofday(&timeObject, NULL);
        long int currentTime = (timeObject.tv_sec * 1000) + (timeObject.tv_usec / 1000);
        int timeElapsed = (currentTime - originalTime);
        
        if (timeElapsed > deadline) {
            break;
        }
        AIShell::last_IDS_Depth = depthToTry;

        
        m.col = currentMove.col;
        m.row = currentMove.row;
        
        lastIDSMove.col = m.col;
        lastIDSMove.row = m.row;
        

        depthToTry++;
    } while ((deadlinePassed != true) && (depthToTry <= 10));
    return m;
}


int AIShell::getUPRIGHTUtility() {
    int aiScore = 0, humScore = 0;
    for (int col = 0; col <= numCols - k; col++) {
        for (int row = 0; row <= numRows - k; row++) {
            bool aiWin = true, humanWin = true;
            int i = 0;
            for (; i < k; i++) {
                if (gameState[col + i][row + i] == HUMAN_PIECE) {
                    aiWin = false;
                }
                else if (gameState[col + i][row + i] == AI_PIECE) {
                    humanWin = false;
                }
                if ((aiWin && humanWin) != true) {
                    break;
                }
            }
            if (i == k) {
                if (aiWin == true){
                    aiScore ++;}
                if (humanWin == true){
                    humScore ++;}
            }
        }
    }
    return aiScore - humScore;
}

int AIShell::getDOWNRIGHTUtility() {
    int AIscore = 0, humanScore = 0;
    for (int col = 0; col <= numCols - k; col++) {
        for (int row = k - 1; row < numRows; row++) {
            
            bool aiPossible = true, humanPossible = true;
            int i = 0;
            
            for (; i < k; i++) {
                if (gameState[col + i][row - i] == HUMAN_PIECE) {
                    aiPossible = false;
                } else if (gameState[col + i][row - i] == AI_PIECE) {
                    humanPossible = false;
                }
                if (!aiPossible && !humanPossible) {
                    break;
                }
            }
            if (i == k) {
                if (aiPossible == true){
                    AIscore++;}
                if (humanPossible == true){
                    humanScore ++;}
            }
        }
    }
    return AIscore - humanScore;
}

int AIShell::getUPUtility() {
    int AIscore = 0, humanScore = 0;
    for (int col = 0; col < numCols; col++) {
        for (int row = 0; row <= numRows - k; row++) {
            int i = 0;
            for (; i < k; i++) {
                if (gameState[col][row + i] == HUMAN_PIECE) {
                    break;
                }

            }
            if (i == k) {
                AIscore ++;
            } else {
                row = row + i;
            }
        }
        for (int row = 0; row <= numRows - k; row++) {
            int i = 0;
            for (; i < k; i++) {
                if (gameState[col][row + i] == AI_PIECE) {
                    break;
                }
            }
            if (i == k) {
                humanScore ++;
            } else {
                row = row + i;
            }
        }
    }
    return AIscore - humanScore;
}

int AIShell::getRIGHTUtility() {
    int AIscore = 0, humanScore = 0;
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col <= numCols - k; col++) {
            int i = 0;
            for (; i < k; i++) {
                if (gameState[col + i][row] == HUMAN_PIECE) {
                    break;
                }

            }
            if (i == k) {
                AIscore++;
            } else {
                col = col + i;
            }
        }
        for (int col = 0; col <= numCols - k; col++) {
            int i = 0;
            for (; i < k; i++) {
                if (gameState[col + i][row] == AI_PIECE) {
                    break;
                }
            }
            if (i == k) {
                humanScore++;
            } else {
                col = col + i;
            }
        }
    }
    return AIscore - humanScore;
}

bool AIShell::getTerminalState(Move m) {
    int player = gameState[m.col][m.row];
    // check horizontal
    int count = 0, i = m.col, j = m.row;
    while (count < k && i >= 0 && gameState[i--][m.row] == player) {
        count++;
    }
    i = m.col + 1;
    while (count < k && i < numCols && gameState[i++][m.row] == player) {
        count++;
    }
    if (count == k) {
        return true;
    }
    // check SE/NW diagonal
    count = 0, i = m.col, j = m.row;
    while (count < k && i < numCols && j >= 0 && gameState[i++][j--] == player) {
        count++;
    }
    i = m.col - 1; j = m.row + 1;
    while (count < k && i >= 0 && j < numRows && gameState[i--][j++] == player) {
        count++;
    }
    if (count == k){
        return true;}
    // check SW/NE diagonal
    count = 0, i = m.col, j = m.row;
    while (count < k && i >= 0 && j >= 0 && gameState[i--][j--] == player) {
        count++;
    }
    i = m.col + 1; j = m.row + 1;
    while (count < k && i < numCols && j < numRows && gameState[i++][j++] == player) {
        count++;
    }
    if (count == k){
        return true;}
    
    // check vertical
    count = 0, i = m.col, j = m.row;
    while (count < k && i >= 0 && gameState[m.col][j--] == player) {
        count++;
    }
    j = m.row + 1;
    while (count < k && j < numRows && gameState[m.col][j++] == player) {
        count++;
    }
    if (count == k) {
        return true;}
    
    
    
    
    return false;
}


Move AIShell::makeMove(){
    //this part should be filled in by the student to implement the AI
    //Example of a move could be: Move move(1, 2); //this will make a move at col 1, row 2
    
    return IDS();
}