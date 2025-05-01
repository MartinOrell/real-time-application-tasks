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
        Gui_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, unsigned int windowWidth, unsigned int windowHeight, unsigned int fps, int numColumns, int numRows);
        int getId() override;
        void start() override;
        void receiveMessage(const mrt::Message& message) override;

        void connectMain(int mainId);
        void connectCell(int cellId);

    private:
        void sendTogglePlayMessage(int toId);
        void sendToggleStatusMessage(int toId);
        void sendTickMessage(int toId);
        void receiveTimeout(const mrt::TimeoutMessage& message);
        void receiveStatusMessage(const mrt::StatusMessage& message);
        void receiveNumNeighborsMessage(const mrt::NumNeighborsMessage& message);

        void timeoutTransition();

        void runningEntry();
        void receivingEntry();
        void endEntry();

        int _id;
        int _mainId;
        std::vector<int> _cellIds;
        mrt::CapsuleRunner* _capsuleRunnerPtr;
        mrt::CapsuleRunner* _timerRunnerPtr;
        enum State{UNDEFINED, Running, Receiving, End};
        State _state;

        int _tickTimerId;
        std::chrono::steady_clock::duration _tickPeriod;

        unsigned int _windowWidth;
        unsigned int _windowHeight;
        int _numColumns;
        int _numRows;
        Gui* _gui;

        bool _displayNeighborCounters;
};