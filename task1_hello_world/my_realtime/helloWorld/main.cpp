
#include "CapsuleRunner.h"
#include <memory>
#include "Main_Capsule.h"

int main(){

    int nextCapsuleId = 0;

    mrt::CapsuleRunner capsuleRunner(nextCapsuleId++);
    auto hello = std::make_unique<Main_Capsule>(nextCapsuleId++, &capsuleRunner);
    capsuleRunner.addCapsule(std::move(hello));
    capsuleRunner.run();
}