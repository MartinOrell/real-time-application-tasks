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
    switch(_state){
        case Zero:
            _state = State::One;
            oneEntry();
            break;
        case One:
            _state = State::Two;
            twoEntry();
            break;
        case Two:
            _state = State::Three;
            threeEntry();
            break;
        case Three:
            _state = State::Four;
            fourEntry();
            break;
        case Four:
            _state = State::Five;
            fiveEntry();
            break;
        case Five:
            _state = State::Six;
            sixEntry();
            break;
        case Six:
            _state = State::Seven;
            sevenEntry();
            break;
        case Seven:
            _state = State::Eight;
            eightEntry();
            break;
        case Eight:
            _state = State::Nine;
            nineEntry();
            break;
        case Nine:
        {
            _endTime = std::chrono::steady_clock::now();
            auto nSecDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(_endTime-_startTime).count();
            std::cout << " Time: " << nSecDuration << "ns" << std::endl;
        
            std::ofstream out;
            out.open("results.txt", std::ios::app);
            out << nSecDuration << "\n";
            out.close();

            _numCycles++;
            if(_numCycles >= 1001){
                _capsuleRunnerPtr->stop();
                _state = State::UNDEFINED;
                break;
            }
        }
            _state = State::Zero;
            zeroEntry();
            break;
        default:
            return;
    }
    sendInstantTimeout(_id);
}

void Main_Capsule::start(){
    sendInstantTimeout(_id);
    _state = State::Zero;
    zeroEntry();
}

void Main_Capsule::zeroEntry(){
    _startTime = std::chrono::steady_clock::now();
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
