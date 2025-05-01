#pragma once

#include "Capsule.h"

#include "Message.h"
#include <vector>
#include <chrono>

namespace mrt{
    class CapsuleRunner;
}

class Main_Capsule: public mrt::Capsule{
    public:
        Main_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, std::chrono::steady_clock::duration tickPeriod);
        int getId() override;
        void start() override;
        void receiveMessage(const mrt::Message& message) override;

        void connectCell(int cellId);
    private:
        void sendToggleStatusMessage(int toId);
        void receiveTimeout(const mrt::TimeoutMessage& message);
        void receiveTogglePlay();

        void pausedEntry();
        void runningEntry();

        int _id;
        std::vector<int> _cellIds;
        mrt::CapsuleRunner* _capsuleRunnerPtr;
        mrt::CapsuleRunner* _timerRunnerPtr;
        enum State{UNDEFINED, Paused, Running};
        State _state;

        int _tickTimerId;
        std::chrono::steady_clock::duration _tickPeriod;
};