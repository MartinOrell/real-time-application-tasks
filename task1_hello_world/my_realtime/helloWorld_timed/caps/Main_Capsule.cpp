#include "Main_Capsule.h"

#include "CapsuleRunner.h"

#include <iostream>
#include <fstream>

Main_Capsule::Main_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}{}

int Main_Capsule::getId(){
    return _id;
}

void Main_Capsule::start(){
    _startTime = std::chrono::steady_clock::now();
    _state = State::Hello;
    helloEntry();
}

void Main_Capsule::helloEntry(){
    std::cout << "Hello World" << std::endl;
    _endTime = std::chrono::steady_clock::now();
    auto nSecDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(_endTime-_startTime).count();
    std::cout << "Time: " << nSecDuration << "ns" << std::endl;

    std::ofstream out;
    out.open("results.txt", std::ios::app);
    out << nSecDuration << "\n";

    _capsuleRunnerPtr->stop();
}

