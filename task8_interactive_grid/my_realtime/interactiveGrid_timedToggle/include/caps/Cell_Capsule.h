#pragma once

#include "Capsule.h"

#include "Message.h"

namespace mrt{
    class CapsuleRunner;
}

class Cell_Capsule: public mrt::Capsule{
    public:
        Cell_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr);
        int getId() override;
        void start() override;
        void receiveMessage(const mrt::Message& message) override;

        void connectGui(int guiId);
    private:
        void sendStatusMessage(int toId, bool isAlive);
        void receiveToggleMessage();
        
        void deadEntry();
        void aliveEntry();

        int _id;
        int _guiId;
        mrt::CapsuleRunner* _capsuleRunnerPtr;
        enum State{UNDEFINED, Dead, Alive};
        State _state;
};