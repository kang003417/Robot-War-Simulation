// robots.h
#ifndef ROBOTS_H
#define ROBOTS_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <cmath>

const int M = 10; // Rows
const int N = 10; // Columns
const int MAX_ROBOTS = 100; // Maximum number of robots

class Robot {
protected:
    int x, y;
    std::string name;
    std::ofstream& logFile;
    char symbol;
    bool stunned;
    int lives;
    int kills;
    bool upgrade;
    friend int main();
    bool found;

public:

    Robot(std::string &name, char symbol, int posX, int posY, std::ofstream& logFile);

    virtual ~Robot();   

    virtual void printStatus() = 0;   

    std::string getName() { return name; }
    int getX() const { return x; }
    int getY() const { return y; }
    bool isStunned() const { return stunned; }
    bool isFound() const { return found; }
    int getLives() const { return lives; }
    int getKills() const { return kills; }
    bool isUpgraded() const { return upgrade; }
    char getSymbol() const { return symbol; }

    void setStunned(bool status) { stunned = status; }
    bool setfound(bool valid)  {found = valid; }
    void setLives(int life) { lives = life; }
    void setKills(int killCount) { kills = killCount; }
    void setUpgrade(bool status) { upgrade = status; }
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }

    void targetPosition(int targetX, int targetY, Robot* robots[], int& numRobots, char battlefield[M][N], std::ofstream& logFile);
    double calculateDistance(int x1, int y1, int x2, int y2);
};

class MovingRobot : public virtual Robot {
public:
    MovingRobot( std::string& name, char symbol, int x, int y, std::ofstream& logFile)
        : Robot(name, symbol, x, y, logFile) {}
    virtual void move(char battlefield[M][N], Robot* robots[], int numRobots) = 0; // Pure virtual function for movement
};

class ShootingRobot : public virtual Robot {
public:
    ShootingRobot( std::string& name, char symbol, int x, int y, std::ofstream& logFile)
        : Robot(name, symbol, x, y, logFile) {}
    virtual void shoot(Robot* robots[], int numRobots, char battlefield[M][N]) = 0; // Pure virtual function for shooting
};

class SeeingRobot : public virtual Robot {
public:
    SeeingRobot( std::string& name, char symbol, int x, int y, std::ofstream& logFile)
        : Robot(name, symbol, x, y, logFile) {}
    virtual void look(char battlefield[M][N], int &targetX, int &targetY)  = 0; // Pure virtual function for seeing
};

class SteppingRobot : public virtual Robot {
public:
    SteppingRobot( std::string& name, char symbol, int x, int y, std::ofstream& logFile)
        : Robot(name, symbol, x, y, logFile) {}
    virtual void stepOn(char battlefield[M][N], Robot* robots[], int numRobots) = 0; // Pure virtual function for movement
};


class BlueThunder : public Robot {
public:
    BlueThunder(std::string &name, char symbol, int posX, int posY, std::ofstream& logFile)
        : Robot(name, symbol, posX, posY, logFile) {}

    virtual void printStatus() override;
    void shoot(Robot* robots[], int& numRobots, char battlefield[M][N]);
    bool checkUpgrade() const;
};

class Madbot : public Robot {
public:
    Madbot(std::string &name, char symbol, int posX, int posY, std::ofstream& logFile)
        : Robot(name, symbol, posX, posY, logFile) {}
    virtual void printStatus() override;
    void shoot(Robot* robots[], int& numRobots, char battlefield[M][N]);
    bool checkUpgrade() const;
};

class RoboTank : public Robot {
public:
    RoboTank(std::string &name, char symbol, int posX, int posY, std::ofstream& logFile)
        : Robot(name, symbol, posX, posY, logFile) {}
    virtual void printStatus() override;
    void shoot(Robot* robots[], int& numRobots, char battlefield[M][N]);
    bool checkUpgrade() const;
};


class RoboCop : public Robot {
private:
bool found;
int targetX, targetY; 
public:
    RoboCop(std::string &name, char symbol, int posX, int posY, std::ofstream& logFile)
        : Robot(name, symbol, posX, posY, logFile) {}
    virtual void printStatus() override;
    void look(Robot* robots[], int& numRobots,char battlefield[M][N], int &targetX, int &targetY);
    void move(char battlefield[M][N], Robot* robots[], int numRobots);
    void shoot(Robot* robots[], int& numRobots, char battlefield[M][N]);
    bool checkUpgrade() const;
};

class Terminator : public Robot {
private:
bool found;
int targetX, targetY; 
public:
    Terminator(std::string &name, char symbol, int posX, int posY, std::ofstream& logFile)
        : Robot(name, symbol, posX, posY, logFile) {}
    virtual void printStatus() override;
    void look(Robot* robots[], int& numRobots,char battlefield[M][N], int &targetX, int &targetY);
    void stepOn(char battlefield[M][N], Robot* robots[], int numRobots);
    bool checkUpgrade() const;
};

class TerminatorRoboCop : public Robot {
public:
    TerminatorRoboCop(std::string &name, char symbol, int posX, int posY, std::ofstream& logFile)
        : Robot(name, symbol, posX, posY, logFile) {}
    int targetX, targetY; 
    bool found;
    void printStatus() override;
    void look(Robot* robots[], int& numRobots,char battlefield[M][N], int &targetX, int &targetY);
    void shoot(Robot* robots[], int numRobots, char battlefield[M][N]) ;
    void stepOn(char battlefield[M][N], Robot* robots[], int numRobots) ;
    bool checkUpgrade() const ;
};

class UltimateRobot  : public Robot {
public:
    UltimateRobot (std::string &name, char symbol, int posX, int posY, std::ofstream& logFile)
        : Robot(name, symbol, posX, posY, logFile) {}
    int targetX, targetY; 
    bool found;
    void printStatus() override;
    void look(Robot* robots[], int& numRobots,char battlefield[M][N], int &targetX, int &targetY);
    void shoot(Robot* robots[], int numRobots, char battlefield[M][N]) ;
    void stepOn(char battlefield[M][N], Robot* robots[], int numRobots) ;

    bool checkUpgrade() const ;
};
void initializeBattlefield(char battlefield[M][N], Robot* robots[MAX_ROBOTS], int numRobots);
void printBattlefield(char battlefield[M][N],std::ofstream& logFile);
void executeActions(Robot* robot, char battlefield[M][N], Robot* robots[], int &numRobots,std::ofstream& logFile);
void shiftRobotToEnd(Robot* robots[], int &numRobots, int index);

#endif // ROBOTS_H