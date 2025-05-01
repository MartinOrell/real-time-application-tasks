
#include "MessageManager.h"
#include "CapsuleRunner.h"
#include <chrono>
#include <memory>
#include <thread>
#include "Main_Capsule.h"
#include "Digit_Capsule.h"
#include "Gui_Capsule.h"

int main(){
    
    unsigned int windowWidth = 800u;
    unsigned int windowHeight = 500u;

    unsigned int fps = 30;
    auto incPeriod = std::chrono::milliseconds(7);

    mrt::MessageManager messageManager;
    int nextCapsuleId = 0;
    
    mrt::CapsuleRunner capsuleRunner(nextCapsuleId++, &messageManager);
    mrt::CapsuleRunner timerRunner(nextCapsuleId++, &messageManager);
    auto main = std::make_unique<Main_Capsule>(nextCapsuleId++, &capsuleRunner, &timerRunner, incPeriod);
    auto gui = std::make_unique<Gui_Capsule>(nextCapsuleId++,&capsuleRunner, &timerRunner, windowWidth, windowHeight, fps);
    auto digit1 = std::make_unique<Digit_Capsule>(nextCapsuleId++, &capsuleRunner);
    
    main->connectDigit1(digit1->getId());
    gui->connectDigit(digit1->getId());
    gui->connectMain(main->getId());
    digit1->connectGui(gui->getId());

    capsuleRunner.addCapsule(std::move(main));
    capsuleRunner.addCapsule(std::move(digit1));
    capsuleRunner.addCapsule(std::move(gui));

    std::jthread timerThread = std::jthread([&timerRunner](){timerRunner.run();});
    capsuleRunner.run();
    timerRunner.stop();
}