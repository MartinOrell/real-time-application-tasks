#include "Main_Capsule.h"

#include "CapsuleRunner.h"

#include <iostream>
#include <stdexcept>
#include <fstream>

Main_Capsule::Main_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, std::chrono::steady_clock::duration waitTime)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}
, _timerRunnerPtr{timerRunnerPtr}
, _waitTime{waitTime}{}

int Main_Capsule::getId(){
    return _id;
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
    if(timeoutMessage.timerId == _waitTimerId){
        if(_state == State::Waiting){
            _state = State::End;
            endEntry();
            return;
        }
        std::string errorMsg = "Received timeout in unknown state";
        throw std::invalid_argument(errorMsg);
    }

    std::string errorMsg = "Received timeout of unknown id";
    throw std::invalid_argument(errorMsg);
}

void Main_Capsule::start(){
    _startTime = std::chrono::steady_clock::now();
    _state = State::Waiting;
    waitingEntry();
}

void Main_Capsule::waitingEntry(){
    _waitTimerId = _timerRunnerPtr->informIn(_id, _waitTime);
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
