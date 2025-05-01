#pragma once

#include "Capsule.h"

#include "Message.h"
#include <chrono>

namespace mrt{
    class CapsuleRunner;
}

class Main_Capsule: public mrt::Capsule{
    public:
        Main_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, std::chrono::steady_clock::duration waitTime);
        int getId() override;
        void start() override;
        void receiveMessage(const mrt::Message& message) override;

    private:
        void receiveTimeout(const mrt::TimeoutMessage& message);

        void waitingEntry();
        void endEntry();

        int _id;
        mrt::CapsuleRunner* _capsuleRunnerPtr;
        mrt::CapsuleRunner* _timerRunnerPtr;
        enum State{UNDEFINED, Waiting, End};
        State _state;

        int _waitTimerId;
        std::chrono::steady_clock::duration _waitTime;

        std::chrono::steady_clock::time_point _startTime;
        std::chrono::steady_clock::time_point _endTime;
};