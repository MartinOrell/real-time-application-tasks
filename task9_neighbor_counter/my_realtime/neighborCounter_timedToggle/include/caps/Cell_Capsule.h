#pragma once

#include "Capsule.h"

#include "Message.h"
#include <vector>

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
        void connectNeighbor(int neighborId);
    private:
        void sendStatusMessage(int toId, bool isAlive);
        void sendNumNeighborsMessage(int toId, int numNeighbors);
        void receiveToggleMessage();
        void receiveStatusMessage(const mrt::StatusMessage& message);
        
        void deadEntry();
        void aliveEntry();

        int _id;
        int _guiId;
        std::vector<int> _neighborIds;
        mrt::CapsuleRunner* _capsuleRunnerPtr;
        enum State{UNDEFINED, Dead, Alive};
        State _state;

        int _numNeighbors;
};