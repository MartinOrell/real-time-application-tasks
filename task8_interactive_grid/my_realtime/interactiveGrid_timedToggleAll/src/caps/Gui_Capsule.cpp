#include "Gui_Capsule.h"

#include "CapsuleRunner.h"
#include "Gui.h"

#include "SendMessage.h"
#include <stdexcept>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <fstream>

Gui_Capsule::Gui_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, unsigned int windowWidth, unsigned int windowHeight, unsigned int fps, int numColumns, int numRows)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}
, _timerRunnerPtr{timerRunnerPtr}
, _tickPeriod{std::chrono::milliseconds{1000/fps}}
, _windowWidth{windowWidth}
, _windowHeight{windowHeight}
, _numColumns{numColumns}
, _numRows{numRows}{}

int Gui_Capsule::getId(){
    return _id;
}

void Gui_Capsule::connectMain(int mainId){
    _mainId = mainId;
}

void Gui_Capsule::connectCell(int cellId){
    _cellIds.push_back(cellId);
}

void Gui_Capsule::sendTogglePlayMessage(int toId){
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = mrt::VoidMessage{mrt::TogglePlay};
    _capsuleRunnerPtr->sendMessage(sendMessage);
}

void Gui_Capsule::sendToggleStatusMessage(int toId){
    mrt::SendMessage sendMessage;
    sendMessage.toId = toId;
    sendMessage.message = mrt::VoidMessage{mrt::ToggleStatus};
    _capsuleRunnerPtr->sendMessage(sendMessage);
}

void Gui_Capsule::receiveMessage(const mrt::Message& message){
    if(std::holds_alternative<mrt::TimeoutMessage>(message)){
        receiveTimeout(std::get<mrt::TimeoutMessage>(message));
        return;
    }
    if(std::holds_alternative<mrt::StatusMessage>(message)){
        receiveStatusMessage(std::get<mrt::StatusMessage>(message));
        return;
    }
    std::string errorMsg = "Gui_Capsule received message of wrong type";
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

void Gui_Capsule::receiveStatusMessage(const mrt::StatusMessage& message){
    for(int i = 0; i < _cellIds.size(); i++){
        if(_cellIds.at(i) == message.fromId){
            int x = i%_numColumns;
            int y = i/_numColumns;
            if(message.isAlive){
                _gui->setCellColor(x,y,0,0,0);
            }
            else{
                _gui->setCellColor(x,y,255,255,255);
            }

            _numReceived++;
            if(_numReceived >= _cellIds.size()){
                _endTime = std::chrono::steady_clock::now();
                auto nSecDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(_endTime-_startTime).count();
                std::cout << "Gui received " << _numReceived << " cells" << std::endl;
                std::cout << "Time: " << nSecDuration << "ns" << std::endl;

                std::ofstream out;
                out.open("results.txt", std::ios::app);
                out << nSecDuration << "\n";
                out.close();
            }
            
            break;
        }
    }
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
                    _numReceived = 0;
                    _startTime = std::chrono::steady_clock::now();
                    for(auto id : _cellIds){
                        sendToggleStatusMessage(id);
                    }
                }
                break;
            case sf::Event::MouseButtonReleased:
            {
                int x = _gui->getCellColumn(event.mouseButton.x);
                int y = _gui->getCellRow(event.mouseButton.y);
                int cellPos = y*_numColumns + x;
                sendToggleStatusMessage(_cellIds.at(cellPos));
            }
                break;
            default:
                break;
        }
    }
    _gui->draw();
}

void Gui_Capsule::start(){
    _gui = new Gui{_windowWidth, _windowHeight, "Counter"};
    _gui->createCells(_numColumns, _numRows);
    _gui->setCellsOutlineColor(128,128,128);
    _gui->setCellsOutlineThickness(2.f);
    _tickTimerId = _timerRunnerPtr->informEvery(_id, _tickPeriod);
    _state = State::Running;
    runningEntry();
}

void Gui_Capsule::runningEntry(){}

void Gui_Capsule::receivingEntry(){}

void Gui_Capsule::endEntry(){
    _capsuleRunnerPtr->stop();
}
