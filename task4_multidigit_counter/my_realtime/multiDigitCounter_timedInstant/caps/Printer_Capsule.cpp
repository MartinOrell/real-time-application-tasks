#include "Printer_Capsule.h"

#include "CapsuleRunner.h"
#include "SendMessage.h"

#include <iostream>
#include <stdexcept>

Printer_Capsule::Printer_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, std::chrono::steady_clock::duration tickTime)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}
, _timerRunnerPtr{timerRunnerPtr}
, _tickTime{tickTime}{}

int Printer_Capsule::getId(){
    return _id;
}

void Printer_Capsule::connectDigit1(int digit1Id){
    _digit1Id = digit1Id;
}

void Printer_Capsule::connectDigit2(int digit2Id){
    _digit2Id = digit2Id;
}

void Printer_Capsule::connectDigit3(int digit3Id){
    _digit3Id = digit3Id;
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

void Printer_Capsule::requestDigitsTransition(){
    sendRequestDigit(_digit1Id);
    sendRequestDigit(_digit2Id);
    sendRequestDigit(_digit3Id);
    _numRequests = 3;
    _numResponses = 0;
    _state = Receiving;
    receivingEntry();
}

void Printer_Capsule::receiveDigitTransition(const mrt::RespondDigit& digitMessage){
    if(digitMessage.fromId == _digit1Id){
        _digit1Value = digitMessage.digit;
    }
    else if(digitMessage.fromId == _digit2Id){
        _digit2Value = digitMessage.digit;
    }
    else if(digitMessage.fromId == _digit3Id){
        _digit3Value = digitMessage.digit;
    }
    _numResponses++;
    if(_numResponses < _numRequests){
        receivingEntry();
    }
    else{
        std::cout << _digit3Value << _digit2Value << _digit1Value << " ";
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