
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

    int numColumns = 100;
    int numRows = 100;

    mrt::MessageManager messageManager;
    int nextCapsuleId = 0;
    
    mrt::CapsuleRunner capsuleRunner(nextCapsuleId++, &messageManager);
    mrt::CapsuleRunner timerRunner(nextCapsuleId++, &messageManager);
    auto main = std::make_unique<Main_Capsule>(nextCapsuleId++, &capsuleRunner, &timerRunner, mainTickPeriod);
    auto gui = std::make_unique<Gui_Capsule>(nextCapsuleId++,&capsuleRunner, &timerRunner, windowWidth, windowHeight, fps, numColumns, numRows);
    
    for(int y = 0; y < numRows; y++){
        for(int x = 0; x < numColumns; x++){
            auto cell = std::make_unique<Cell_Capsule>(nextCapsuleId++, &capsuleRunner);
            main->connectCell(cell->getId());
            gui->connectCell(cell->getId());
            cell->connectGui(gui->getId());
            capsuleRunner.addCapsule(std::move(cell));
        }
    }
    gui->connectMain(main->getId());

    capsuleRunner.addCapsule(std::move(main));
    capsuleRunner.addCapsule(std::move(gui));

    std::jthread timerThread = std::jthread([&timerRunner](){timerRunner.run();});
    capsuleRunner.run();
    timerRunner.stop();
}