
#include "MessageManager.h"
#include "CapsuleRunner.h"
#include <chrono>
#include <memory>
#include <thread>
#include "Main_Capsule.h"

int main(){

    mrt::MessageManager messageManager;

    auto waitTime = std::chrono::microseconds(1);

    int nextCapsuleId = 0;

    mrt::CapsuleRunner capsuleRunner(nextCapsuleId++, &messageManager);
    //mrt::CapsuleRunner timerRunner(nextCapsuleId++, &messageManager);
    auto main = std::make_unique<Main_Capsule>(nextCapsuleId++, &capsuleRunner, &capsuleRunner, waitTime);

    capsuleRunner.addCapsule(std::move(main));

    //std::jthread timerThread = std::jthread([&timerRunner](){timerRunner.run();});
    capsuleRunner.run();
    //timerRunner.stop();
}