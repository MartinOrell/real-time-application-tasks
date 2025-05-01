#include "Main_Capsule.h"

#include "CapsuleRunner.h"

#include "SendMessage.h"
#include <stdexcept>

Main_Capsule::Main_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, std::chrono::steady_clock::duration tickTime)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}
, _timerRunnerPtr{timerRunnerPtr}
, _tickTime{tickTime}{}

int Main_Capsule::getId(){
    return _id;
}

void Main_Capsule::connectDigit1(int digit1Id){
    _digit1Id = digit1Id;
}

void Main_Capsule::sendIncMessage(int toId){
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = mrt::VoidMessage{mrt::IncMessage};
    _capsuleRunnerPtr->sendMessage(sendMessage);
}

void Main_Capsule::receiveMessage(const mrt::Message& message){
    if(std::holds_alternative<mrt::TimeoutMessage>(message)){
        receiveTimeout(std::get<mrt::TimeoutMessage>(message));
        return;
    }

    std::string errorMsg = "Received message of wrong type";
    throw std::invalid_argument(errorMsg);
}

void Main_Capsule::receiveTimeout(const mrt::TimeoutMessage& timeoutMessage){
    if(timeoutMessage.timerId == _tickTimerId){
        if(_state == Running){
            runningEntry();
            return;
        }
        else{
            std::string errorMsg = "Received timeout in unknown state";
            throw std::invalid_argument(errorMsg);
        }
    }

    std::string errorMsg = "Received timeout of wrong id. Expected: "
    + std::to_string(_tickTimerId)
    + ". Received: "
    + std::to_string(timeoutMessage.timerId);
    throw std::invalid_argument(errorMsg);
}

void Main_Capsule::start(){
    _tickTimerId = _timerRunnerPtr->informEvery(_id, _tickTime);
    _state = State::Running;
    runningEntry();
}

void Main_Capsule::runningEntry(){
    sendIncMessage(_digit1Id);
}