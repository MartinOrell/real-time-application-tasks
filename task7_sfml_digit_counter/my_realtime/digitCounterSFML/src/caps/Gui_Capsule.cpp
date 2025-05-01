#include "Gui_Capsule.h"

#include "CapsuleRunner.h"
#include "Gui.h"

#include "SendMessage.h"
#include <stdexcept>
#include <SFML/Window/Event.hpp>

Gui_Capsule::Gui_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, unsigned int windowWidth, unsigned int windowHeight, unsigned int fps)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}
, _timerRunnerPtr{timerRunnerPtr}
, _tickPeriod{std::chrono::milliseconds{1000/fps}}
, _windowWidth{windowWidth}
, _windowHeight{windowHeight}{}

int Gui_Capsule::getId(){
    return _id;
}

void Gui_Capsule::connectDigit(int digitId){
    _digitIds.push_back(digitId);
    _digitValues.push_back(0);
}

void Gui_Capsule::connectMain(int mainId){
    _mainId = mainId;
}

void Gui_Capsule::sendRequestDigit(int toId){
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = mrt::VoidMessage{mrt::RequestDigit};
    _capsuleRunnerPtr->sendMessage(sendMessage);
}

void Gui_Capsule::sendTogglePlayMessage(int toId){
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = mrt::VoidMessage{mrt::TogglePlay};
    _capsuleRunnerPtr->sendMessage(sendMessage);
}

void Gui_Capsule::sendIncMessage(int toId){
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = mrt::VoidMessage{mrt::IncMessage};
    _capsuleRunnerPtr->sendMessage(sendMessage);
}

void Gui_Capsule::receiveMessage(const mrt::Message& message){
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

void Gui_Capsule::receiveTimeout(const mrt::TimeoutMessage& timeoutMessage){
    if(timeoutMessage.timerId == _tickTimerId){
        if(_state = State::Running){
            timeoutTransition();
        }
        return;
    }

    std::string errorMsg = "Received timeout of unknown id";
    throw std::invalid_argument(errorMsg);
}

void Gui_Capsule::receiveDigit(const mrt::RespondDigit& digitMessage){
    if(_state == Receiving){
        receiveDigitTransition(digitMessage);
    }
}

void Gui_Capsule::receiveConnectDigit(const mrt::ConnectDigit& message){
    connectDigit(message.digitId);
}

void Gui_Capsule::timeoutTransition(){
    sf::Event event;
    while(_gui->pollEvent(event)){
        switch(event.type){
            case sf::Event::Closed:
                _state = State::End;
                endEntry();
                return;
            case sf::Event::KeyPressed:
                if(event.key.scancode == sf::Keyboard::Scan::P){
                    sendTogglePlayMessage(_mainId);
                }
                else if(event.key.scancode == sf::Keyboard::Scan::Space){
                    sendIncMessage(_digitIds.at(0));
                }
                break;
            default:
                break;
        }
    }
    requestDigitsTransition();
}

void Gui_Capsule::requestDigitsTransition(){
    for(int digitId : _digitIds){
        sendRequestDigit(digitId);
    }
    _numRequests = _digitIds.size();
    _numResponses = 0;
    _state = Receiving;
    receivingEntry();
}

void Gui_Capsule::receiveDigitTransition(const mrt::RespondDigit& digitMessage){
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
        std::string text;
        for(int i = _digitIds.size()-1; i >= 0; i--){
            text.push_back('0'+_digitValues.at(i));
        }    
        _gui->setMiddleText(text);
        _gui->draw();
        _state = Running;
        runningEntry();
    }
}

void Gui_Capsule::start(){
    _gui = new Gui{_windowWidth, _windowHeight, "Counter"};
    _gui->createMiddleText();
    _gui->setMiddleText("0");
    _tickTimerId = _timerRunnerPtr->informEvery(_id, _tickPeriod);
    _state = State::Running;
    runningEntry();
}

void Gui_Capsule::runningEntry(){}

void Gui_Capsule::receivingEntry(){}

void Gui_Capsule::endEntry(){
    _capsuleRunnerPtr->stop();
}
