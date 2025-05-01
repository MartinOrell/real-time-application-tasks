#pragma once

#include "Capsule.h"

#include "Message.h"

#include <chrono>

namespace mrt{
    class CapsuleRunner;
}

class Printer_Capsule: public mrt::Capsule{
    public:
        Printer_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, std::chrono::steady_clock::duration tickTime);
        int getId() override;
        void start() override;
        void receiveMessage(const mrt::Message& message) override;

        void connectDigit(int digitId);

    private:
        void sendRequestDigit(int toId);
        void receiveTimeout(const mrt::TimeoutMessage& message);
        void receiveDigit(const mrt::RespondDigit& message);
        void receiveConnectDigit(const mrt::ConnectDigit& message);

        void requestDigitsTransition();
        void receiveDigitTransition(const mrt::RespondDigit& message);

        void runningEntry();
        void receivingEntry();

        int _id;
        std::vector<int> _digitIds;
        std::vector<int> _digitValues;
        
        mrt::CapsuleRunner* _capsuleRunnerPtr;
        mrt::CapsuleRunner* _timerRunnerPtr;
        enum State{UNDEFINED, Running, Receiving};
        State _state;

        int _numRequests;
        int _numResponses;

        int _tickTimerId;
        std::chrono::steady_clock::duration _tickTime;
};