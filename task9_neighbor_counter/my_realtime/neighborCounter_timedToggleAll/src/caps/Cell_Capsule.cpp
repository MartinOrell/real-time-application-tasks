#include "Cell_Capsule.h"

#include "CapsuleRunner.h"

#include "SendMessage.h"
#include <stdexcept>

Cell_Capsule::Cell_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}
, _numNeighbors{0}{}

int Cell_Capsule::getId(){
    return _id;
}

void Cell_Capsule::connectGui(int guiId){
    _guiId = guiId;
}

void Cell_Capsule::connectNeighbor(int neighborId){
    _neighborIds.push_back(neighborId);
}

void Cell_Capsule::sendStatusMessage(int toId, bool isAlive){
    mrt::StatusMessage outMessage;
    outMessage.fromId =_id;
    outMessage.isAlive = isAlive;
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = outMessage;
    _capsuleRunnerPtr->sendMessage(sendMessage);
}

void Cell_Capsule::sendNumNeighborsMessage(int toId, int numNeighbors){
    mrt::NumNeighborsMessage outMessage;
    outMessage.fromId = _id;
    outMessage.numNeighbors = numNeighbors;
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = outMessage;
    _capsuleRunnerPtr->sendMessage(sendMessage);
}

void Cell_Capsule::receiveMessage(const mrt::Message& message){
    if(std::holds_alternative<mrt::VoidMessage>(message)){
        switch(std::get<mrt::VoidMessage>(message)){
            case mrt::ToggleStatus:
                receiveToggleMessage();
                return;
            default:
                std::string errorMsg = "Cell received message of wrong type";
                throw std::invalid_argument(errorMsg);
        }
        return;
    }
    if(std::holds_alternative<mrt::StatusMessage>(message)){
        receiveStatusMessage(std::get<mrt::StatusMessage>(message));
        return;
    }

    std::string errorMsg = "Received message of wrong type";
    throw std::invalid_argument(errorMsg);
}

void Cell_Capsule::receiveToggleMessage(){
    switch(_state){
        case Dead:
            for(auto nId : _neighborIds){
                sendStatusMessage(nId, true);
            }
            sendStatusMessage(_guiId, true);
            _state = State::Alive;
            aliveEntry();
            return;
        case Alive:
            for(auto nId : _neighborIds){
                sendStatusMessage(nId, false);
            }
            sendStatusMessage(_guiId, false);
            _state = State::Dead;
            deadEntry();
            return;
        default:
            std::string errorMsg = "Received toggleMessage in unknown state";
            throw std::invalid_argument(errorMsg);   
    }
}

void Cell_Capsule::receiveStatusMessage(const mrt::StatusMessage& message){
    if(message.isAlive){
        _numNeighbors++;
    }
    else{
        _numNeighbors--;
    }
    sendNumNeighborsMessage(_guiId, _numNeighbors);
}

void Cell_Capsule::start(){
    _state = State::Dead;
    deadEntry();
}

void Cell_Capsule::deadEntry(){}

void Cell_Capsule::aliveEntry(){}