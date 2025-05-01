#include "Main_Capsule.h"

#include "CapsuleRunner.h"

#include <iostream>
#include <stdexcept>
#include <fstream>

Main_Capsule::Main_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}
, _timerRunnerPtr{timerRunnerPtr}{}

int Main_Capsule::getId(){
    return _id;
}

void Main_Capsule::sendInstantTimeout(int toId){
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = mrt::VoidMessage{mrt::InstantTimeout};
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
    if(_state == State::Waiting){
        _state = State::End;
        endEntry();
        return;
    }
    std::string errorMsg = "Received timeout in unknown state";
    throw std::invalid_argument(errorMsg);
}

void Main_Capsule::start(){
    _startTime = std::chrono::steady_clock::now();
    _state = State::Waiting;
    waitingEntry();
}

void Main_Capsule::waitingEntry(){
    sendInstantTimeout(_id);
    std::cout << "Hello World!" << std::endl;
}

void Main_Capsule::endEntry(){
    std::cout << "Goodbye World!" << std::endl;

    _endTime = std::chrono::steady_clock::now();
    auto nSecDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(_endTime-_startTime).count();
    std::cout << "Time: " << nSecDuration << "ns" << std::endl;

    std::ofstream out;
    out.open("results.txt", std::ios::app);
    out << nSecDuration << "\n";

    _capsuleRunnerPtr->stop();
}
