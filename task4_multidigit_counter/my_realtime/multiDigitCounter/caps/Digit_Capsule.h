#pragma once

#include "Capsule.h"

#include "Message.h"

namespace mrt{
    class CapsuleRunner;
}

class Digit_Capsule: public mrt::Capsule{
    public:
        Digit_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr);
        int getId() override;
        void start() override;
        void receiveMessage(const mrt::Message& message) override;

        void connectPrinter(int printerId);
        void connectOverflowSendToId(int digitId);
    private:
        void sendDigitMessage(int toId, int digit);
        void sendIncMessage(int toId);
        void receiveRequestDigitMessage();
        void receiveIncMessage();

        void overflowTransition();
        
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
        int _printerId;
        int _overflowSendToId;
        mrt::CapsuleRunner* _capsuleRunnerPtr;
        enum State{UNDEFINED, One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Zero};
        State _state;
};