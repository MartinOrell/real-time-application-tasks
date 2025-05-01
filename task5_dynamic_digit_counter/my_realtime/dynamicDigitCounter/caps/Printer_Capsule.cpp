#include "Printer_Capsule.h"

#include "CapsuleRunner.h"

#include "SendMessage.h"
#include <stdexcept>
#include <iostream>

Printer_Capsule::Printer_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, std::chrono::steady_clock::duration tickTime)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}
, _timerRunnerPtr{timerRunnerPtr}
, _tickTime{tickTime}{}

int Printer_Capsule::getId(){
    return _id;
}

void Printer_Capsule::connectDigit(int digitId){
    _digitIds.push_back(digitId);
    _digitValues.push_back(0);
}

void Printer_Capsule::sendRequestDigit(int toId){
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = mrt::VoidMessage{mrt::RequestDigit};
    _capsuleRunnerPtr->sendMessage(sendMessage);
}

void Printer_Capsule::receiveMessage(const mrt::Message& message){
    if(std::holds_alternative<mrt::TimeoutMessage>(message)){
        receiveTimeout(std::get<mrt::TimeoutMessage>(message));
        return;
    }
    if(std::holds_alternative<mrt::RespondDigit>(message)){
        receiveDigit(std::get<mrt::RespondDigit>(message));
        return;
    }
    if(std::holds_alternative<mrt::ConnectDigit>(message)){
        receiveConnectDigit(std::get<mrt::ConnectDigit>(message));
        return;
    }
    std::string errorMsg = "Received message of wrong type";
    throw std::invalid_argument(errorMsg);
}

void Printer_Capsule::receiveTimeout(const mrt::TimeoutMessage& timeoutMessage){
    if(timeoutMessage.timerId == _tickTimerId){
        if(_state == Running){
            requestDigitsTransition();
        }
        return;
    }

    std::string errorMsg = "Received timeout of unknown id";
    throw std::invalid_argument(errorMsg);
}

void Printer_Capsule::receiveDigit(const mrt::RespondDigit& digitMessage){
    if(_state == Receiving){
        receiveDigitTransition(digitMessage);
    }
}

void Printer_Capsule::receiveConnectDigit(const mrt::ConnectDigit& message){
    connectDigit(message.digitId);
}

void Printer_Capsule::requestDigitsTransition(){
    for(int digitId : _digitIds){
        sendRequestDigit(digitId);
    }
    _numRequests = _digitIds.size();
    _numResponses = 0;
    _state = Receiving;
    receivingEntry();
}

void Printer_Capsule::receiveDigitTransition(const mrt::RespondDigit& digitMessage){
    for(int i = 0; i < _digitIds.size(); i++){
        if(digitMessage.fromId == _digitIds.at(i)){
            _digitValues.at(i) = digitMessage.digit;
            break;
        }
    }
    _numResponses++;
    if(_numResponses < _numRequests){
        receivingEntry();
    }
    else{
        for(int i = _digitIds.size()-1; i >= 0; i--){
            std::cout << _digitValues.at(i);
        }
        std::cout << " ";
        _state = Running;
        runningEntry();
    }
}

void Printer_Capsule::start(){
    _tickTimerId = _timerRunnerPtr->informEvery(_id, _tickTime);
    _state = State::Running;
    runningEntry();
}

void Printer_Capsule::runningEntry(){}

void Printer_Capsule::receivingEntry(){}