
#include "MessageManager.h"
#include "CapsuleRunner.h"
#include <chrono>
#include <memory>
#include <thread>
#include "Main_Capsule.h"

int main(){

    unsigned int windowWidth = 800u;
    unsigned int windowHeight = 500u;
    
    int fps = 30;

    mrt::MessageManager messageManager;

    int nextCapsuleId = 0;

    mrt::CapsuleRunner capsuleRunner(nextCapsuleId++, &messageManager);
    mrt::CapsuleRunner timerRunner(nextCapsuleId++, &messageManager);
    auto main = std::make_unique<Main_Capsule>(nextCapsuleId++, &capsuleRunner, &timerRunner, windowWidth, windowHeight, fps);

    capsuleRunner.addCapsule(std::move(main));

    std::jthread timerThread = std::jthread([&timerRunner](){timerRunner.run();});
    capsuleRunner.run();
    timerRunner.stop();
}