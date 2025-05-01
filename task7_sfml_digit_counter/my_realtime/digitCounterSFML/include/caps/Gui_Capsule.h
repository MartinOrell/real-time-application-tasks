#pragma once

#include "Capsule.h"

#include "Message.h"
#include <vector>
#include <chrono>

namespace mrt{
    class CapsuleRunner;
}
class Gui;

class Gui_Capsule: public mrt::Capsule{
    public:
        Gui_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, unsigned int windowWidth, unsigned int windowHeight, unsigned int fps);
        int getId() override;
        void start() override;
        void receiveMessage(const mrt::Message& message) override;

        void connectDigit(int digitId);
        void connectMain(int mainId);

    private:
        void sendRequestDigit(int toId);
        void sendTogglePlayMessage(int toId);
        void sendIncMessage(int toId);
        void receiveTimeout(const mrt::TimeoutMessage& message);
        void receiveDigit(const mrt::RespondDigit& message);
        void receiveConnectDigit(const mrt::ConnectDigit& message);

        void timeoutTransition();
        void requestDigitsTransition();
        void receiveDigitTransition(const mrt::RespondDigit& message);

        void runningEntry();
        void receivingEntry();
        void endEntry();

        int _id;
        int _mainId;
        std::vector<int> _digitIds;
        std::vector<int> _digitValues;
        mrt::CapsuleRunner* _capsuleRunnerPtr;
        mrt::CapsuleRunner* _timerRunnerPtr;
        enum State{UNDEFINED, Running, Receiving, End};
        State _state;

        int _tickTimerId;
        std::chrono::steady_clock::duration _tickPeriod;

        int _numRequests;
        int _numResponses;

        unsigned int _windowWidth;
        unsigned int _windowHeight;
        Gui* _gui;
};