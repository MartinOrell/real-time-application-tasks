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

    private:
        void receiveTimeout(const mrt::TimeoutMessage& message);

        void zeroEntry();
        void oneEntry();
        void twoEntry();
        void threeEntry();
        void fourEntry();
        void fiveEntry();
        void sixEntry();
        void sevenEntry();
        void eightEntry();
        void nineEntry();

        int _id;
        mrt::CapsuleRunner* _capsuleRunnerPtr;
        mrt::CapsuleRunner* _timerRunnerPtr;
        enum State{UNDEFINED, One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Zero};
        State _state;

        int _tickTimerId;
        std::chrono::steady_clock::duration _tickTime;

        int _numCycles{0};
        std::chrono::steady_clock::time_point _startTime;
        std::chrono::steady_clock::time_point _endTime;
};