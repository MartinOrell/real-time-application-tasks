
#include "MessageManager.h"
#include "CapsuleRunner.h"
#include <chrono>
#include <memory>
#include <thread>
#include "Main_Capsule.h"
#include "Digit_Capsule.h"
#include "Printer_Capsule.h"

int main(){

    auto incInterval = std::chrono::microseconds(1);
    auto printInterval = std::chrono::milliseconds(1000);

    mrt::MessageManager messageManager;
    int nextCapsuleId = 0;
    
    mrt::CapsuleRunner capsuleRunner(nextCapsuleId++, &messageManager);
    mrt::CapsuleRunner timerRunner(nextCapsuleId++, &messageManager);
    auto main = std::make_unique<Main_Capsule>(nextCapsuleId++, &capsuleRunner, &timerRunner, incInterval);
    auto digit1 = std::make_unique<Digit_Capsule>(nextCapsuleId++, &capsuleRunner);
    auto digit2 = std::make_unique<Digit_Capsule>(nextCapsuleId++, &capsuleRunner);
    auto digit3 = std::make_unique<Digit_Capsule>(nextCapsuleId++, &capsuleRunner);
    auto printer = std::make_unique<Printer_Capsule>(nextCapsuleId++, &capsuleRunner, &timerRunner, printInterval);

    main->connectDigit1(digit1->getId());
    printer->connectDigit1(digit1->getId());
    printer->connectDigit2(digit2->getId());
    printer->connectDigit3(digit3->getId());
    digit1->connectPrinter(printer->getId());
    digit2->connectPrinter(printer->getId());
    digit3->connectPrinter(printer->getId());

    digit1->connectOverflowSendToId(digit2->getId());
    digit2->connectOverflowSendToId(digit3->getId());

    capsuleRunner.addCapsule(std::move(main));
    capsuleRunner.addCapsule(std::move(digit1));
    capsuleRunner.addCapsule(std::move(digit2));
    capsuleRunner.addCapsule(std::move(digit3));
    capsuleRunner.addCapsule(std::move(printer));

    std::jthread timerThread = std::jthread([&timerRunner](){timerRunner.run();});
    capsuleRunner.run();
    timerRunner.stop();
}