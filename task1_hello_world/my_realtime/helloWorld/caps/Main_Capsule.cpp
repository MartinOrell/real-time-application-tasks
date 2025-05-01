#include "Main_Capsule.h"

#include "CapsuleRunner.h"

#include <iostream>

Main_Capsule::Main_Capsule(int id, mrt::CapsuleRunner* capsuleRunnerPtr)
: _id{id}
, _state{UNDEFINED}
, _capsuleRunnerPtr{capsuleRunnerPtr}{}

int Main_Capsule::getId(){
    return _id;
}

void Main_Capsule::start(){
    _state = State::Hello;
    helloEntry();
}

void Main_Capsule::helloEntry(){
    std::cout << "Hello World" << std::endl;
    _capsuleRunnerPtr->stop();
}

