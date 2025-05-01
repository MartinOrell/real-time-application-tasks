#pragma once

#include "Capsule.h"

#include "Message.h"
#include <chrono>

namespace mrt{
    class CapsuleRunner;
}
class Gui;

class Main_Capsule: public mrt::Capsule{
    public:
        Main_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, unsigned int windowWidth, unsigned int windowHeight, unsigned int fps);
        int getId() override;
        void start() override;
        void receiveMessage(const mrt::Message& message) override;

    private:
        void receiveTimeout(const mrt::TimeoutMessage& message);

        void timeoutTransition();

        void runningEntry();
        void endEntry();

        int _id;
        mrt::CapsuleRunner* _capsuleRunnerPtr;
        mrt::CapsuleRunner* _timerRunnerPtr;
        enum State{UNDEFINED, Running, End};
        State _state;

        int _tickTimerId;
        std::chrono::steady_clock::duration _tickPeriod;

        unsigned int _windowWidth;
        unsigned int _windowHeight;
        Gui* _gui;
};