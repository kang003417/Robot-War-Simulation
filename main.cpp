/**********|**********|**********| 
Program: main.cpp / robots.h  / robots,cpp
Course: Data Structures and Algorithms 
Trimester: 2410 
Name: Khor Jea Shen,Karthekeyan, Chin Wei Kang, Low Sam Yee
ID: 1211110871, 1211108235,1211102952,1231303316
Lecture Section: TC4L
Tutorial Section: TT13L 
Email: 1211108235@student.mmu.edu.my
Phone: 011-2852 4521
**********|**********|**********/ 

/*
TO RUN THE PROGRAM:
g++ main.cpp robots.cpp -o my_program
./my_program
*/
#include "robots.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

void executeActions(Robot* robot, char battlefield[M][N], Robot* robots[], int &numRobots, std::ofstream& logFile) {
    int targetX,targetY;
    if (BlueThunder* blueThunder = dynamic_cast<BlueThunder*>(robot)) {
        blueThunder->shoot(robots, numRobots, battlefield);
    } else if (Madbot* madbot = dynamic_cast<Madbot*>(robot)) {
        madbot->shoot(robots, numRobots, battlefield);
    } else if (RoboTank* RTank = dynamic_cast<RoboTank*>(robot)) {
        RTank->shoot(robots, numRobots, battlefield);
    }else if (RoboCop* roboCop = dynamic_cast<RoboCop*>(robot)) {
        roboCop->look(robots , numRobots,battlefield, targetX, targetY);
        roboCop->move(battlefield, robots, numRobots);
        roboCop->shoot(robots, numRobots, battlefield);
    }else if (Terminator* term = dynamic_cast<Terminator*>(robot)) {
        term->look(robots , numRobots,battlefield, targetX, targetY);
        term->stepOn(battlefield, robots, numRobots);
        
    }else if (TerminatorRoboCop* termCop = dynamic_cast<TerminatorRoboCop*>(robot)) {
        termCop->look(robots , numRobots,battlefield, targetX, targetY);
        termCop->stepOn(battlefield, robots, numRobots);
        termCop->shoot(robots, numRobots, battlefield);
    }else {
        // Handle other robot types or unexpected cases
        logFile << "Unknown robot type encountered.\n";
    }

    BlueThunder* blueThunder = dynamic_cast<BlueThunder*>(robot);
    if (blueThunder && blueThunder->checkUpgrade()) {
        char currentSymbol = robot->getSymbol(); // Preserve current symbol
        int posX = blueThunder->getX();
        int posY = blueThunder->getY();
        std::string name = robot->getName();
        delete blueThunder;
        robot = new Madbot(name, currentSymbol, posX, posY, logFile);  
    }

    Madbot* Mdbot = dynamic_cast<Madbot*>(robot);
    if (Mdbot && Mdbot->checkUpgrade()) {
        char currentSymbol = robot->getSymbol(); // Preserve current symbol
        int posX = Mdbot->getX();
        int posY = Mdbot->getY();
        std::string name = robot->getName();
        delete Mdbot;
        robot = new RoboTank(name, currentSymbol, posX, posY, logFile);
       
    }

    RoboTank* RTank = dynamic_cast<RoboTank*>(robot);
    if (RTank && RTank->checkUpgrade()) {
        char currentSymbol = robot->getSymbol(); // Preserve current symbol
        int posX = RTank->getX();
        int posY = RTank->getY();
        std::string name = robot->getName();
        delete RTank;
        robot = new UltimateRobot(name, currentSymbol, posX, posY, logFile);
       
    }

    RoboCop* roboCop = dynamic_cast<RoboCop*>(robot);
    if (roboCop && roboCop->checkUpgrade()) {
        char currentSymbol = robot->getSymbol(); // Preserve current symbol
        int posX = roboCop->getX();
        int posY = roboCop->getY();
        std::string name = robot->getName();
        delete roboCop;
        robot = new TerminatorRoboCop(name, currentSymbol, posX, posY, logFile);
    }
    
    // Check for upgrades to TerminatorRoboCop
    Terminator* term = dynamic_cast<Terminator*>(robot);
    if (term && term->checkUpgrade()) {
        char currentSymbol = robot->getSymbol();
        int posX = term->getX();
        int posY = term->getY();
        std::string name = robot->getName();
        delete term;
        robot = new TerminatorRoboCop(name, currentSymbol, posX, posY, logFile);
    }

    TerminatorRoboCop* trc = dynamic_cast<TerminatorRoboCop*>(robot);
    if (trc && trc->checkUpgrade()) {
        char currentSymbol = robot->getSymbol();
        int posX = trc->getX();
        int posY = trc->getY();
        std::string name = robot->getName();
        delete trc;
        robot = new UltimateRobot(name, currentSymbol, posX, posY, logFile);
    }
}

int countAliveRobots(Robot* robots[], int numRobots) {
    int count = 0;
    for (int i = 0; i < numRobots; i++) {
        if (robots[i]->getLives() > 0) {
            count++;
        }
    }
    return count;
}


int main() {

    std::ofstream logFile("simulation.txt");
    if(!logFile){
        std::cerr <<"Error oprning stimulation log file" << std:: endl;
        return 1;
    }

    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }
    std::string line;
    int steps = 0, numRobots = 0;


    // Read steps
    std::getline(inputFile, line);
    if (line.find("Steps:") == 0) {
        std::istringstream iss(line.substr(6));
        iss >> steps;
    } else {
        std::cerr << "Invalid format for steps." << std::endl;
        return 1;
    }

    // Read number of robots
    std::getline(inputFile, line);
    if (line.find("NumOfRobots:") == 0) {
        std::istringstream iss(line.substr(12));
        iss >> numRobots;
    } else {
        std::cerr << "Invalid format for number of robots." << std::endl;
        return 1;
    }


    if (numRobots > MAX_ROBOTS) {
        std::cerr << "Too many robots specified." << std::endl;
        return 1;
    }
    Robot* robots[MAX_ROBOTS];
    char type[20], name[20];
    int posX, posY;


    std::srand(std::time(0));
    try {
        for (int i = 0; i < numRobots; i++) {
            inputFile >> type >> name >> posX >> posY;
            if (posX == -1 && posY == -1) {
                posX = std::rand() % M;
                posY = std::rand() % N;
            }
            if (strcmp(type, "Madbot") != 0 && strcmp(type, "RoboCop") != 0  && strcmp(type, "BlueThunder") != 0 && strcmp(type, "RoboTank") != 0&& strcmp(type, "Terminator") != 0) {
                throw std::invalid_argument("Type of robot implemented wrong");
            }
            if (posX > M) {
                throw std::out_of_range("Implement of " + std::string(name) + " position x out of bound");
            }
            if (posY > N) {
                throw std::out_of_range("Implement of " + std::string(name) + " position y out of bound");
            }
            if (posX < 0 || posY < 0) {
                throw std::out_of_range("position cannot be negative");
            }
            std::string robotName(name);

            if (strcmp(type, "Madbot") == 0) {
                robots[i] = new Madbot(robotName,name[0], posX, posY,logFile);
            } else if (strcmp(type, "BlueThunder") == 0) {
                robots[i] = new BlueThunder(robotName,name[0], posX, posY,logFile);
            }else if (strcmp(type, "RoboCop") == 0) {
                robots[i] = new RoboCop(robotName,name[0], posX, posY,logFile);
            }else if (strcmp(type, "RoboTank") == 0) {
                robots[i] = new RoboTank(robotName,name[0], posX, posY,logFile);
            } else if (strcmp(type, "Terminator") == 0) {
                robots[i] = new Terminator(robotName,name[0], posX, posY,logFile);
            }
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    char battlefield[M][N];
    initializeBattlefield(battlefield, robots, numRobots);
    logFile<< "Initial Battlefield:" << std::endl;
    printBattlefield(battlefield, logFile);

     for (int step = 0; step < steps; step++) {
        logFile <<std::endl;
        logFile<< "Step " << step + 1 << ":" << std::endl;
        for (int i = 0; i < numRobots; i++) {
            if (robots[i] -> lives == 0)
            { delete robots[i];
              numRobots--;;
              continue; }
            else if (robots[i]->lives >= 0 && robots[i]->stunned) {
                continue;
            } else {
                logFile <<std::endl;
                executeActions(robots[i], battlefield, robots, numRobots,logFile);
            }
        }
        initializeBattlefield(battlefield, robots, numRobots);
        printBattlefield(battlefield,logFile);
        for (int i = 0; i < numRobots; i++){
            if (robots[i]->lives >= 0 && robots[i]->stunned) {
                robots[i]->setStunned(false);
                shiftRobotToEnd(robots, numRobots, i);
            }
        }
        int aliveRobots = countAliveRobots(robots, numRobots);
        if (aliveRobots <= 1) {
            logFile << "Only one robot left. Terminating program." << std::endl;
            for(int i; i<numRobots;i++){
                logFile << robots[i]->name <<" , " << robots[i] -> symbol <<" Win!!!" << std::endl;
                break;
            }
            break;
        }
    }

    for (int i = 0; i < numRobots; i++) {
        delete robots[i];
    }
    return 0;
}
