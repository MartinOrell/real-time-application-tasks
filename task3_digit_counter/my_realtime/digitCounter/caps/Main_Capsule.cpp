#include "Main_Capsule.h"

#include "CapsuleRunner.h"

#include <iostream>
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
        switch(_state){
            case Zero:
                _state = State::One;
                oneEntry();
                return;
            case One:
                _state = State::Two;
                twoEntry();
                return;
            case Two:
                _state = State::Three;
                threeEntry();
                return;
            case Three:
                _state = State::Four;
                fourEntry();
                return;
            case Four:
                _state = State::Five;
                fiveEntry();
                return;
            case Five:
                _state = State::Six;
                sixEntry();
                return;
            case Six:
                _state = State::Seven;
                sevenEntry();
                return;
            case Seven:
                _state = State::Eight;
                eightEntry();
                return;
            case Eight:
                _state = State::Nine;
                nineEntry();
                return;
            case Nine:
                _state = State::Zero;
                zeroEntry();
                return;
            default:
                std::string errorMsg = "Received timeout in unknown state";
                throw std::invalid_argument(errorMsg);   
        }
    }

    std::string errorMsg = "Received timeout of unknown id";
    throw std::invalid_argument(errorMsg);
}

void Main_Capsule::start(){
    _tickTimerId = _timerRunnerPtr->informEvery(_id, _tickTime);
    _state = State::Zero;
    zeroEntry();
}

void Main_Capsule::zeroEntry(){
    std::cout << "0";
}

void Main_Capsule::oneEntry(){
    std::cout << "1";
}

void Main_Capsule::twoEntry(){
    std::cout << "2";
}

void Main_Capsule::threeEntry(){
    std::cout << "3";
}

void Main_Capsule::fourEntry(){
    std::cout << "4";
}

void Main_Capsule::fiveEntry(){
    std::cout << "5";
}

void Main_Capsule::sixEntry(){
    std::cout << "6";
}

void Main_Capsule::sevenEntry(){
    std::cout << "7";
}

void Main_Capsule::eightEntry(){
    std::cout << "8";
}

void Main_Capsule::nineEntry(){
    std::cout << "9";
}
