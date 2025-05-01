#pragma once

#include "Capsule.h"
#include <chrono>

namespace mrt{
    class CapsuleRunner;
}

class Main_Capsule: public mrt::Capsule{
    public:
        Main_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr);
        int getId() override;
        void start() override;

    private:
        void helloEntry();

        int _id;
        mrt::CapsuleRunner* _capsuleRunnerPtr;
        enum State{UNDEFINED, Hello};
        State _state;

        std::chrono::steady_clock::time_point _startTime;
        std::chrono::steady_clock::time_point _endTime;
};