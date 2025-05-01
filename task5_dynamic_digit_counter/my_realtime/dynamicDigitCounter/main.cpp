
#include "MessageManager.h"
#include "CapsuleRunner.h"
#include <chrono>
#include <memory>
#include <thread>
#include "Main_Capsule.h"
#include "Digit_Capsule.h"
#include "Printer_Capsule.h"

int main(){

    auto incInterval = std::chrono::milliseconds(7);
    auto printInterval = std::chrono::milliseconds(1000);

    mrt::MessageManager messageManager;
    int nextCapsuleId = 0;
    
    mrt::CapsuleRunner capsuleRunner(nextCapsuleId++, &messageManager);
    mrt::CapsuleRunner timerRunner(nextCapsuleId++, &messageManager);
    auto main = std::make_unique<Main_Capsule>(nextCapsuleId++, &capsuleRunner, &timerRunner, incInterval);
    auto printer = std::make_unique<Printer_Capsule>(nextCapsuleId++, &capsuleRunner, &timerRunner, printInterval);
    auto digit1 = std::make_unique<Digit_Capsule>(nextCapsuleId++, &capsuleRunner);
    
    main->connectDigit1(digit1->getId());
    printer->connectDigit(digit1->getId());
    digit1->connectPrinter(printer->getId());

    capsuleRunner.addCapsule(std::move(main));
    capsuleRunner.addCapsule(std::move(digit1));
    capsuleRunner.addCapsule(std::move(printer));

    std::jthread timerThread = std::jthread([&timerRunner](){timerRunner.run();});
    capsuleRunner.run();
    timerRunner.stop();
}