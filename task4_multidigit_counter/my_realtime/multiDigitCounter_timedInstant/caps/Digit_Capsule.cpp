#include "Digit_Capsule.h"

#include "CapsuleRunner.h"
#include "SendMessage.h"

#include <stdexcept>
#include <fstream>

Digit_Capsule::Digit_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}
, _overflowSendToId{-1}{}

int Digit_Capsule::getId(){
    return _id;
}

void Digit_Capsule::connectPrinter(int printerId){
    _printerId = printerId;
}

void Digit_Capsule::connectOverflowSendToId(int digitId){
    _overflowSendToId = digitId;
}

void Digit_Capsule::sendDigitMessage(int toId, int digit){
    mrt::RespondDigit outMessage;
    outMessage.fromId =_id;
    outMessage.digit = digit;
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = outMessage;
    _capsuleRunnerPtr->sendMessage(sendMessage);
}

void Digit_Capsule::sendIncMessage(int toId){
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = mrt::VoidMessage{mrt::IncMessage};
    _capsuleRunnerPtr->sendMessage(sendMessage);
}

void Digit_Capsule::receiveMessage(const mrt::Message& message){
    if(std::holds_alternative<mrt::VoidMessage>(message)){
        switch(std::get<mrt::VoidMessage>(message)){
            case mrt::IncMessage:
                receiveIncMessage();
                return;
            case mrt::RequestDigit:
                receiveRequestDigitMessage();
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

void Digit_Capsule::receiveRequestDigitMessage(){
    switch(_state){
        case Zero:
            sendDigitMessage(_printerId, 0);
            return;
        case One:
            sendDigitMessage(_printerId, 1);
            return;
        case Two:
            sendDigitMessage(_printerId, 2);
            return;
        case Three:
            sendDigitMessage(_printerId, 3);
            return;
        case Four:
            sendDigitMessage(_printerId, 4);
            return;
        case Five:
            sendDigitMessage(_printerId, 5);
            return;
        case Six:
            sendDigitMessage(_printerId, 6);
            return;
        case Seven:
            sendDigitMessage(_printerId, 7);
            return;
        case Eight:
            sendDigitMessage(_printerId, 8);
            return;
        case Nine:
            sendDigitMessage(_printerId, 9);
            return;
        default:
            return; 
    }
}

void Digit_Capsule::receiveIncMessage(){
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
            overflowTransition();
            return;
        default:
            return;  
    }
}

void Digit_Capsule::overflowTransition(){
    if(_overflowSendToId != -1){
        sendIncMessage(_overflowSendToId);
    }
    else{
        _endTime = std::chrono::steady_clock::now();
        auto nSecDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(_endTime-_startTime).count();

        std::ofstream out;
        out.open("results.txt", std::ios::app);
        out << nSecDuration << "\n";
        out.close();

        _numCycles++;
        if(_numCycles >= 1000){
            _capsuleRunnerPtr->stop();
            _state = State::UNDEFINED;
            return;
        }

        _startTime = std::chrono::steady_clock::now();
    }
    _state = State::Zero;
    zeroEntry();
}

void Digit_Capsule::start(){
    _state = State::Zero;
    _startTime = std::chrono::steady_clock::now();
    zeroEntry();
}

void Digit_Capsule::zeroEntry(){}

void Digit_Capsule::oneEntry(){}

void Digit_Capsule::twoEntry(){}

void Digit_Capsule::threeEntry(){}

void Digit_Capsule::fourEntry(){}

void Digit_Capsule::fiveEntry(){}

void Digit_Capsule::sixEntry(){}

void Digit_Capsule::sevenEntry(){}

void Digit_Capsule::eightEntry(){}

void Digit_Capsule::nineEntry(){}