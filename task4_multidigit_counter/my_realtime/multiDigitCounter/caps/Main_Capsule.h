#pragma once

#include "Capsule.h"

#include "Message.h"

#include <chrono>

namespace mrt{
    class CapsuleRunner;
}

class Main_Capsule: public mrt::Capsule{
    public:
        Main_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, std::chrono::steady_clock::duration tickTime);
        int getId() override;
        void start() override;
        void receiveMessage(const mrt::Message& message) override;

        void connectDigit1(int digit1Id);
    private:
        void sendIncMessage(int toId);
        void receiveTimeout(const mrt::TimeoutMessage& message);

        void runningEntry();

        int _id;
        int _digit1Id;
        mrt::CapsuleRunner* _capsuleRunnerPtr;
        mrt::CapsuleRunner* _timerRunnerPtr;
        enum State{UNDEFINED, Running};
        State _state;

        int _tickTimerId;
        std::chrono::steady_clock::duration _tickTime;
};