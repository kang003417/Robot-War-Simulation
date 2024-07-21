// robots.cpp
#include "robots.h"
#include <limits> 


// Constructor definition
Robot::Robot(std::string &name, char symbol, int posX, int posY, std::ofstream& logFile)
    : name(name), symbol(symbol), logFile(logFile), stunned(false), lives(3), kills(0), upgrade(false), found(false) {
    stunned = false;
    x = posX;
    y = posY;
}

// Destructor definition
Robot::~Robot() {}

void Robot::targetPosition(int targetX, int targetY, Robot* robots[], int& numRobots, char battlefield[M][N], std::ofstream& logFile) {
    for (int i = 0; i < numRobots; i++) {
        if (robots[i]->getX() == targetX && robots[i]->getY() == targetY) {
            robots[i]->setLives(robots[i]->getLives() - 1);
            robots[i]->setStunned(true);

            // Increment kills for the firing robot
            int currentKills = robots[i]->getKills();
            robots[i]->setKills(currentKills + 1);

            battlefield[targetX][targetY] = '.';

            logFile << getSymbol() << " attact (" << targetX << ", " << targetY << "), successfully killed " << robots[i]->getSymbol() << "\n";
            robots[i] -> printStatus() ;

            if(robots[i]->getLives() >0){
                robots[i]->x = rand()%M;
                robots[i]->y = rand()%N;
                logFile << robots[i]-> name << " , " << robots[i]->getSymbol() <<" will be entered at ( " << robots[i]->x << " , " << robots[i]->y << " ) next turn\n" ;
                logFile <<std::endl;
            }else if (robots[i]->getLives() <= 0) {
                logFile << robots[i]->getSymbol() << " was destroyed.\n";
                // Handle robot destruction if necessary
            }

            if (robots[i]->getKills() >= 3 && !robots[i]->isUpgraded()) {
                logFile << getSymbol() << " Upgrade \n";
                robots[i]->setUpgrade(true);
            }
            return;
        }
    }

    logFile << getSymbol() <<" fired (" << targetX << ", " << targetY << "), missed.\n";
}
double Robot:: calculateDistance(int x1, int y1, int x2, int y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

void BlueThunder::printStatus() {
    logFile << name << " , " << symbol <<" has " << lives << "left \n";
}

void BlueThunder::shoot(Robot* robots[], int& numRobots, char battlefield[M][N]) {
    bool fireValid = false;
    int targetX, targetY;
     while (!fireValid) {
        int direction = rand() % 4;
        switch (direction) {
            case 0: targetX = x - 1; break; // Up
            case 1: targetY = y + 1; break; // Right
            case 2: targetX = x + 1; break; // Down
            case 3: targetY = y - 1; break; // Left
        }
        // Check if the target position is within the battlefield boundaries and not the same as the current position
        if (targetX >= 0 && targetX < M && targetY >= 0 && targetY < N && !(targetX == x && targetY == y)) {
            fireValid = true; // Valid firing position found
        }
    }

    targetPosition(targetX, targetY, robots, numRobots, battlefield, logFile);
}
bool BlueThunder::checkUpgrade() const {
    return upgrade;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Madbot::printStatus() {
    logFile << name << " , " << symbol <<" has " << lives << " lives left \n" ;
}
void Madbot::shoot(Robot* robots[], int& numRobots, char battlefield[M][N]) {
    bool fireValid = false;
    int targetX, targetY;

    while (!fireValid) {
        targetX = x + (rand() % 3 - 1); // Randomly choose a new targetX
        targetY = y + (rand() % 3 - 1); // Randomly choose a new targetY
        if (targetX >= 0 && targetX < M && targetY >= 0 && targetY < N && !(targetX == x && targetY == y)) {
            fireValid = true; // Valid firing position found
        }
    }

     targetPosition(targetX, targetY, robots, numRobots, battlefield, logFile);
}

bool Madbot::checkUpgrade() const {
    return upgrade;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RoboTank::printStatus() {
    logFile << name << " , " << symbol <<" has " << lives << " lives left \n" ;
}
void RoboTank::shoot(Robot* robots[], int& numRobots, char battlefield[M][N]) {
    bool fireValid = false;
    int targetX, targetY;

    while (!fireValid) {
        targetX = rand() % M; // Randomly choose a new targetX
        targetY = rand() % N; // Randomly choose a new targetY
        if (targetX >= 0 && targetX < M && targetY >= 0 && targetY < N && !(targetX == x && targetY == y)) {
            fireValid = true; // Valid firing position found
        }
    }
     targetPosition(targetX, targetY, robots, numRobots, battlefield, logFile);
}
bool RoboTank::checkUpgrade() const {
    return upgrade;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RoboCop::printStatus() {
    logFile << name << " , " << symbol <<" has " << lives << " lives left \n" ;
}

void RoboCop::look(Robot* robots[], int& numRobots,char battlefield[M][N], int &targetX, int &targetY) {
    int scanRadius = 1;  // Assuming a scan radius of 1

    for (int dx = -scanRadius; dx <= scanRadius; ++dx) {
        for (int dy = -scanRadius; dy <= scanRadius; ++dy) {
            int lookX = x + dx;
            int lookY = y + dy;

            // Skip the current robot's own position
            if (dx == 0 && dy == 0) continue;

            if (lookX >= 0 && lookX < M && lookY >= 0 && lookY < N ) {
                for (int i = 0; i < numRobots; ++i) {
                    if (robots[i] != this && robots[i]->getX() == lookX && robots[i]->getY() == lookY) {
                        targetX = lookX;
                        targetY = lookY;
                        setfound(true);
                        return;
                    }
                }
            }
        }
    }

}

void RoboCop::move(char battlefield[M][N], Robot* robots[], int numRobots) {
    look(robots, numRobots, battlefield,targetX,targetY);
        // If no enemy robot is found, set targetX and targetY to random values within -1 to 1 around the current position
    if (!isFound()) {
        targetX = x + (rand() % 3 - 1);
        targetY = y + (rand() % 3 - 1);
        logFile << symbol << " did not find an enemy. New target is (" << targetX << " , " << targetY << ")\n";
    }
    else{
        for (int i = 0; i < numRobots; ++i){
        logFile << symbol << " found " << robots[i]->getSymbol() << " at (" << targetX << " , " << targetY << ")\n";
        return;
        }
    }

    if (x < targetX && battlefield[x + 1][y] == '.') {
        ++x;
    } else if (x > targetX && battlefield[x - 1][y] == '.') {
        --x;
    }

    if (y < targetY && battlefield[x][y + 1] == '.') {
        ++y;
    } else if (y > targetY && battlefield[x][y - 1] == '.') {
        --y;
    }

    battlefield[x][y] = symbol;
    logFile << symbol << " moved to (" << x << " , " << y << ")\n";
    setfound(false); // Reset found status
}
void RoboCop::shoot(Robot* robots[], int& numRobots, char battlefield[M][N]) {
    int targetX, targetY;
    bool fireValid = false;
    int numValid = 3;

    for (int i = 0; i < numValid; ++i) {
        fireValid = false;
        while (!fireValid) {
            int offsetX = rand() % 11 - 5;
            int offsetY = rand() % (11 - std::abs(offsetX)) - (10 - std::abs(offsetX));
            targetX = x + offsetX;
            targetY = y + offsetY;
            if (targetX >= 0 && targetX < M && targetY >= 0 && targetY < N && !(targetX == x && targetY == y)) {
                fireValid = true; // Valid firing position found
            }
        }
        targetPosition(targetX, targetY, robots, numRobots, battlefield, logFile);
    }
}

bool RoboCop::checkUpgrade() const {
    return upgrade;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Terminator::printStatus() {
    logFile << name << " , " << symbol <<" has " << lives << " lives left \n" ;
}

void Terminator::look(Robot* robots[], int& numRobots, char battlefield[M][N], int &targetX, int &targetY) {
    int scanRadius = 1;  // Assuming a scan radius of 1

    for (int dx = -scanRadius; dx <= scanRadius; ++dx) {
        for (int dy = -scanRadius; dy <= scanRadius; ++dy) {
            int lookX = x + dx;
            int lookY = y + dy;

            // Skip the current robot's own position
            if (dx == 0 && dy == 0) continue;

            if (lookX >= 0 && lookX < M && lookY >= 0 && lookY < N) {
                for (int i = 0; i < numRobots; ++i) {
                    if ((robots[i]->getX() != x || robots[i]->getY() != y) && robots[i]->getX() == lookX && robots[i]->getY() == lookY) {
                        targetX = lookX;
                        targetY = lookY;
                        setfound(true);
                        return;
                    }
                }
            }
        }
    }
    setfound(false);  // No enemy robot found in the 3x3 neighborhood
}

void Terminator::stepOn(char battlefield[M][N], Robot* robots[], int numRobots) {
    int targetX, targetY;
    look(robots, numRobots, battlefield, targetX, targetY);
    if (isFound()) {
        // Move to the target position and kill the enemy robot
        targetPosition(targetX, targetY, robots, numRobots, battlefield, logFile);
        // Update position
        setX(targetX);
        setY(targetY);
    } else {
        // No enemy found, random move or stay
       int newX, newY;
        do {
            newX = x + (rand() % 3 - 1); // Random move -1, 0, or 1
            newY = y + (rand() % 3 - 1);
        } while ((newX < 0 || newX >= M || newY < 0 || newY >= N || (newX == x && newY == y))); 
            setX(newX);
            setY(newY);
        logFile << symbol << " step  at (" << newX << "," << newY << ")." << std::endl;
        
    }
}


bool Terminator::checkUpgrade() const {
    return upgrade;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TerminatorRoboCop::printStatus() {
    logFile << name << " , " << symbol <<" has " << lives << " lives left \n" ;
}


void TerminatorRoboCop::look(Robot* robots[], int& numRobots,char battlefield[M][N], int &targetX, int &targetY) {
    int scanRadius = 1;  // Assuming a scan radius of 1

    for (int dx = -scanRadius; dx <= scanRadius; ++dx) {
        for (int dy = -scanRadius; dy <= scanRadius; ++dy) {
            int lookX = x + dx;
            int lookY = y + dy;

            // Skip the current robot's own position
            if (dx == 0 && dy == 0) continue;

            if (lookX >= 0 && lookX < M && lookY >= 0 && lookY < N ) {
                for (int i = 0; i < numRobots; ++i) {
                    if (robots[i] != this && robots[i]->getX() == lookX && robots[i]->getY() == lookY) {
                        targetX = lookX;
                        targetY = lookY;
                        setfound(true);
                        return;
                    }
                }
            }
        }
    }

}

void TerminatorRoboCop::shoot(Robot* robots[], int numRobots, char battlefield[M][N]) {
    bool fireValid = false;
    int targetX, targetY;
    int numValid = 3;
    for (int i = 0; i < numValid; ++i) {
        while (!fireValid) {
        int offsetX = rand() % 11 - 5;
        int offsetY = rand() % (11 - std::abs(offsetX)) - (10 - std::abs(offsetX));
        int targetX = x + offsetX;
        int targetY = y + offsetY;
        if (targetX >= 0 && targetX < M && targetY >= 0 && targetY < N && !(targetX == x && targetY == y)) {
            fireValid = true; // Valid firing position found
            numValid++;  
        }
    }
    targetPosition(targetX, targetY, robots, numRobots, battlefield, logFile);
        
    }
}

void TerminatorRoboCop::stepOn(char battlefield[M][N], Robot* robots[], int numRobots) {
    int targetX, targetY;
    look(robots, numRobots, battlefield, targetX, targetY);
    if (isFound()) {
        // Move to the target position and kill the enemy robot
        targetPosition(targetX, targetY, robots, numRobots, battlefield, logFile);
        // Update position
        setX(targetX);
        setY(targetY);
    } else {
        // No enemy found, random move or stay
       int newX, newY;
        do {
            newX = x + (rand() % 3 - 1); // Random move -1, 0, or 1
            newY = y + (rand() % 3 - 1);
        } while ((newX < 0 || newX >= M || newY < 0 || newY >= N || (newX == x && newY == y))); 
            setX(newX);
            setY(newY);
        logFile << symbol << " step  at (" << newX << "," << newY << ")." << std::endl;
        
    }
}

bool TerminatorRoboCop::checkUpgrade() const {
    return upgrade;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UltimateRobot ::printStatus() {
    logFile << name << " , " << symbol <<" has " << lives << " lives left \n" ;
}


void UltimateRobot ::look(Robot* robots[], int& numRobots,char battlefield[M][N], int &targetX, int &targetY) {
    int scanRadius = 1;  // Assuming a scan radius of 1

    for (int dx = -scanRadius; dx <= scanRadius; ++dx) {
        for (int dy = -scanRadius; dy <= scanRadius; ++dy) {
            int lookX = x + dx;
            int lookY = y + dy;

            // Skip the current robot's own position
            if (dx == 0 && dy == 0) continue;

            if (lookX >= 0 && lookX < M && lookY >= 0 && lookY < N ) {
                for (int i = 0; i < numRobots; ++i) {
                    if (robots[i] != this && robots[i]->getX() == lookX && robots[i]->getY() == lookY) {
                        targetX = lookX;
                        targetY = lookY;
                        setfound(true);
                        return;
                    }
                }
            }
        }
    }

}

void UltimateRobot ::shoot(Robot* robots[], int numRobots, char battlefield[M][N]) {
    bool fireValid = false;
    int targetX, targetY;
    int numValid = 3;
    for (int i = 0; i < numValid; ++i) {
        while (!fireValid) {
        int offsetX = rand() % M;
        int offsetY = rand() % N;
        int targetX = x + offsetX;
        int targetY = y + offsetY;
        if (targetX >= 0 && targetX < M && targetY >= 0 && targetY < N && !(targetX == x && targetY == y)) {
            fireValid = true; // Valid firing position found
            numValid++;  
        }
    }
    targetPosition(targetX, targetY, robots, numRobots, battlefield, logFile);
        
    }
}

void UltimateRobot ::stepOn(char battlefield[M][N], Robot* robots[], int numRobots) {
    int targetX, targetY;
    look(robots, numRobots, battlefield, targetX, targetY);
    if (isFound()) {
        // Move to the target position and kill the enemy robot
        targetPosition(targetX, targetY, robots, numRobots, battlefield, logFile);
        // Update position
        setX(targetX);
        setY(targetY);
    } else {
        // No enemy found, random move or stay
       int newX, newY;
        do {
            newX = x + (rand() % 3 - 1); // Random move -1, 0, or 1
            newY = y + (rand() % 3 - 1);
        } while ((newX < 0 || newX >= M || newY < 0 || newY >= N || (newX == x && newY == y))); 
            setX(newX);
            setY(newY);
        logFile << symbol << " step  at (" << newX << "," << newY << ")." << std::endl;
        
    }
}
bool UltimateRobot ::checkUpgrade() const {
    return upgrade;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void shiftRobotToEnd(Robot* robots[], int &numRobots, int index) {
    Robot* temp = robots[index];
    for (int i = index; i < numRobots - 1; ++i) {
        robots[i] = robots[i + 1];
    }
    robots[numRobots - 1] = temp;
}

void initializeBattlefield(char battlefield[M][N], Robot* robots[MAX_ROBOTS], int numRobots) {
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            battlefield[i][j] = '.';
    for (int i = 0; i < numRobots; i++) {
        if (robots[i] -> isStunned()){continue;}
        else{
             battlefield[robots[i]->getX()][robots[i]->getY()] = robots[i]->getSymbol();
        }
    }
}

void printBattlefield(char battlefield[M][N],std::ofstream& logFile) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++)
            logFile << battlefield[i][j] << ' ';
        logFile << std::endl;
    }
}

