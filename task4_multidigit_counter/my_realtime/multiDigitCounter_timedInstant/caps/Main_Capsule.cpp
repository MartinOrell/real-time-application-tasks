#include "Main_Capsule.h"

#include "CapsuleRunner.h"
#include "SendMessage.h"

#include <stdexcept>

Main_Capsule::Main_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}
, _timerRunnerPtr{timerRunnerPtr}{}

int Main_Capsule::getId(){
    return _id;
}

void Main_Capsule::connectDigit1(int digit1Id){
    _digit1Id = digit1Id;
}

void Main_Capsule::sendInstantTimeout(int toId){
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = mrt::VoidMessage{mrt::InstantTimeout};
    _capsuleRunnerPtr->mergeOrSendMessage(sendMessage);
}

void Main_Capsule::sendIncMessage(int toId){
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = mrt::VoidMessage{mrt::IncMessage};
    _capsuleRunnerPtr->sendMessage(sendMessage);
}

void Main_Capsule::receiveMessage(const mrt::Message& message){
    if(std::holds_alternative<mrt::VoidMessage>(message)){
        switch(std::get<mrt::VoidMessage>(message)){
            case mrt::InstantTimeout:
                receiveTimeout();
                return;
            default:
                std::string errorMsg = "Received message of wrong type";
                throw std::invalid_argument(errorMsg);
        }
        return;
    }

    std::string errorMsg = "Received message of wrong type";
    throw std::invalid_argument(errorMsg);
}

void Main_Capsule::receiveTimeout(){
    
    if(_state == State::Running){
        sendInstantTimeout(_id);
        runningEntry();
        return;
    }
}

void Main_Capsule::start(){
    sendInstantTimeout(_id);
    _state = State::Running;
    runningEntry();
}

void Main_Capsule::runningEntry(){
    sendIncMessage(_digit1Id);
}