#include "Main_Capsule.h"

#include "CapsuleRunner.h"

#include "SendMessage.h"
#include <stdexcept>

Main_Capsule::Main_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, std::chrono::steady_clock::duration tickPeriod)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}
, _timerRunnerPtr{timerRunnerPtr}
, _tickPeriod{tickPeriod}{}

int Main_Capsule::getId(){
    return _id;
}

void Main_Capsule::connectCell(int cellId){
    _cellIds.push_back(cellId);
}

void Main_Capsule::sendTickMessage(int toId){
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = mrt::VoidMessage{mrt::Tick};
    _capsuleRunnerPtr->sendMessage(sendMessage);
}

void Main_Capsule::receiveMessage(const mrt::Message& message){
    if(std::holds_alternative<mrt::VoidMessage>(message)){
        switch(std::get<mrt::VoidMessage>(message)){
            case mrt::TogglePlay:
                receiveTogglePlay();
                return;
            default:
                std::string errorMsg = "Main received voidMessage of wrong type";
                throw std::invalid_argument(errorMsg);
        }
        return;
    }
    if(std::holds_alternative<mrt::TimeoutMessage>(message)){
        receiveTimeout(std::get<mrt::TimeoutMessage>(message));
        return;
    }

    std::string errorMsg = "Main received message of wrong type";
    throw std::invalid_argument(errorMsg);
}

void Main_Capsule::receiveTimeout(const mrt::TimeoutMessage& timeoutMessage){
    if(timeoutMessage.timerId == _tickTimerId){
        if(_state == Running){
            runningEntry();
            return;
        }
    }
}

void Main_Capsule::receiveTogglePlay(){
    switch(_state){
        case State::Running:
            _timerRunnerPtr->cancelTimer(_tickTimerId);
            _state = State::Paused;
            pausedEntry();
            return;
        case State::Paused:
            _tickTimerId = _timerRunnerPtr->informEvery(_id, _tickPeriod);
            _state = State::Running;
            runningEntry();
            return;
        default:
            std::string errorMsg = "Received ToggleMessage in unknown state";
            throw std::invalid_argument(errorMsg);
    }
}

void Main_Capsule::start(){
    _state = State::Paused;
    pausedEntry();
}

void Main_Capsule::pausedEntry(){}

void Main_Capsule::runningEntry(){
    for(auto id : _cellIds){
        sendTickMessage(id);
    }
}