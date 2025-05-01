
#include "MessageManager.h"
#include "CapsuleRunner.h"
#include <chrono>
#include <memory>
#include <thread>
#include "Main_Capsule.h"
#include "Gui_Capsule.h"
#include "Cell_Capsule.h"

int main(){    

    unsigned int windowWidth = 800u;
    unsigned int windowHeight = 500u;

    unsigned int fps = 30;
    auto mainTickPeriod = std::chrono::milliseconds(1000);

    const int numColumns = 50;
    const int numRows = 20;

    mrt::MessageManager messageManager;
    int nextCapsuleId = 0;
    
    mrt::CapsuleRunner capsuleRunner(nextCapsuleId++, &messageManager);
    mrt::CapsuleRunner timerRunner(nextCapsuleId++, &messageManager);
    auto main = std::make_unique<Main_Capsule>(nextCapsuleId++, &capsuleRunner, &timerRunner, mainTickPeriod);
    auto gui = std::make_unique<Gui_Capsule>(nextCapsuleId++,&capsuleRunner, &timerRunner, windowWidth, windowHeight, fps, numColumns, numRows);
    
    std::unique_ptr<Cell_Capsule> cellCapsules[numColumns][numRows];

    for(int y = 0; y < numRows; y++){
        for(int x = 0; x < numColumns; x++){
            cellCapsules[x][y] = std::make_unique<Cell_Capsule>(nextCapsuleId++, &capsuleRunner);
        }
    }

    for(int y = 0; y < numRows; y++){
        for(int x = 0; x < numColumns; x++){
            main->connectCell(cellCapsules[x][y]->getId());
            gui->connectCell(cellCapsules[x][y]->getId());
            cellCapsules[x][y]->connectGui(gui->getId());

            if(x > 0){
                cellCapsules[x][y]->connectNeighbor(cellCapsules[x-1][y]->getId());
            }
            if(x < numColumns-1){
                cellCapsules[x][y]->connectNeighbor(cellCapsules[x+1][y]->getId());
            }
            if(y > 0){
                cellCapsules[x][y]->connectNeighbor(cellCapsules[x][y-1]->getId());
            }
            if(y < numRows-1){
                cellCapsules[x][y]->connectNeighbor(cellCapsules[x][y+1]->getId());
            }
            if(x > 0 && y > 0){
                cellCapsules[x][y]->connectNeighbor(cellCapsules[x-1][y-1]->getId());
            }
            if(x < numColumns-1 && y > 0){
                cellCapsules[x][y]->connectNeighbor(cellCapsules[x+1][y-1]->getId());
            }
            if(x > 0 && y < numRows-1){
                cellCapsules[x][y]->connectNeighbor(cellCapsules[x-1][y+1]->getId());
            }
            if(x < numColumns-1 && y < numRows-1){
                cellCapsules[x][y]->connectNeighbor(cellCapsules[x+1][y+1]->getId());
            }
        }
    }

    gui->connectMain(main->getId());

    capsuleRunner.addCapsule(std::move(main));
    capsuleRunner.addCapsule(std::move(gui));
    for(int y = 0; y < numRows; y++){
        for(int x = 0; x < numColumns; x++){
            capsuleRunner.addCapsule(std::move(cellCapsules[x][y]));
        }
    }

    std::jthread timerThread = std::jthread([&timerRunner](){timerRunner.run();});
    capsuleRunner.run();
    timerRunner.stop();
}