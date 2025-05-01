#include "Cell_Capsule.h"

#include "CapsuleRunner.h"

#include "SendMessage.h"
#include <stdexcept>

Cell_Capsule::Cell_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}
, _currentNumNeighbors{0}
, _nextNumNeighbors{0}
, _sync{false}{}

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

void Cell_Capsule::sendToCellStatusMessage(int toId, bool isAlive){
    mrt::CellToCellStatusMessage outMessage;
    outMessage.isAlive = isAlive;
    outMessage.sync = _sync;
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
            case mrt::Tick:
                receiveTickMessage();
                return;
            default:
                std::string errorMsg = "Cell received message of wrong type";
                throw std::invalid_argument(errorMsg);
        }
        return;
    }
    if(std::holds_alternative<mrt::CellToCellStatusMessage>(message)){
        receiveStatusMessage(std::get<mrt::CellToCellStatusMessage>(message));
        return;
    }

    std::string errorMsg = "Cell Received message of wrong type";
    throw std::invalid_argument(errorMsg);
}

void Cell_Capsule::receiveToggleMessage(){
    switch(_state){
        case Dead:
            for(auto nId : _neighborIds){
                sendToCellStatusMessage(nId, true);
            }
            sendStatusMessage(_guiId, true);
            _state = State::Alive;
            aliveEntry();
            return;
        case Alive:
            for(auto nId : _neighborIds){
                sendToCellStatusMessage(nId, false);
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

void Cell_Capsule::receiveTickMessage(){
    _sync = !_sync;
    switch(_state){
        case Dead:

            if(_currentNumNeighbors != 3){
                _currentNumNeighbors = _nextNumNeighbors;
                deadEntry();
                return;
            }

            for(auto nId : _neighborIds){
                sendToCellStatusMessage(nId, true);
            }
            sendStatusMessage(_guiId, true);
            _currentNumNeighbors = _nextNumNeighbors;
            _state = State::Alive;
            aliveEntry();
            return;
        case Alive:

            if(_currentNumNeighbors == 2 or _currentNumNeighbors == 3){
                _currentNumNeighbors = _nextNumNeighbors;
                aliveEntry();
                return;
            }

            for(auto nId : _neighborIds){
                sendToCellStatusMessage(nId, false);
            }
            sendStatusMessage(_guiId, false);
            _currentNumNeighbors = _nextNumNeighbors;
            _state = State::Dead;
            deadEntry();
            return;
        default:
            std::string errorMsg = "Received toggleMessage in unknown state";
            throw std::invalid_argument(errorMsg);   
    }
}

void Cell_Capsule::receiveStatusMessage(const mrt::CellToCellStatusMessage& message){
    if(message.isAlive){
        if(message.sync == _sync){
            _currentNumNeighbors++;
        }
        _nextNumNeighbors++;
    }
    else{
        if(message.sync == _sync){
            _currentNumNeighbors--;
        }
        _nextNumNeighbors--;
    }
    sendNumNeighborsMessage(_guiId, _nextNumNeighbors);
}

void Cell_Capsule::start(){
    _state = State::Dead;
    deadEntry();
}

void Cell_Capsule::deadEntry(){}

void Cell_Capsule::aliveEntry(){}