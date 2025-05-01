#include "Main_Capsule.h"

#include "CapsuleRunner.h"
#include "Gui.h"

#include <stdexcept>
#include <SFML/Window/Event.hpp>
#include <fstream>

Main_Capsule::Main_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr, mrt::CapsuleRunner* timerRunnerPtr, unsigned int windowWidth, unsigned int windowHeight, unsigned int fps)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}
, _timerRunnerPtr{timerRunnerPtr}
, _tickPeriod{std::chrono::milliseconds{1000/fps}}
, _windowWidth{windowWidth}
, _windowHeight{windowHeight}{}

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
        if(_state = State::Running){
            timeoutTransition();
        }
        return;
    }

    std::string errorMsg = "Received timeout of unknown id";
    throw std::invalid_argument(errorMsg);
}

void Main_Capsule::timeoutTransition(){
    _startTime = std::chrono::steady_clock::now();
    sf::Event event;
    while(_gui->pollEvent(event)){
        if(event.type == sf::Event::Closed){
            _state = State::End;
            endEntry();
            return;
        }
    }
    _gui->draw();

    _endTime = std::chrono::steady_clock::now();
    auto nSecDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(_endTime-_startTime).count();

    std::ofstream out;
    out.open("results.txt", std::ios::app);
    out << nSecDuration << "\n";

    _numCycles++;
    if(_numCycles >= 1001){
        _state = State::End;
        endEntry();
        return;
    }

    _state = State::Running;
    runningEntry();
}

void Main_Capsule::start(){
    _gui = new Gui{_windowWidth, _windowHeight, "Hello SFML"};
    _gui->createMiddleText();
    _gui->setMiddleText("Hello SFML");
    _tickTimerId = _timerRunnerPtr->informEvery(_id, _tickPeriod);
    _state = State::Running;
    runningEntry();
}

void Main_Capsule::runningEntry(){
}


void Main_Capsule::endEntry(){
    _capsuleRunnerPtr->stop();
}
