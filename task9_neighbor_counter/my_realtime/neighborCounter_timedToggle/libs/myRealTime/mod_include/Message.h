#pragma once

#include <variant>
#include "Timer.h"

namespace mrt{

    struct TimeoutMessage{
        int timerId;
        int timeouts;
    };

    struct CancelTimer{
        int timerId;
    };

    struct StatusMessage{
        int fromId;
        bool isAlive;
    };

    struct NumNeighborsMessage{
        int fromId;
        int numNeighbors;
    };

    enum VoidMessage{EndMessage, TogglePlay, ToggleStatus};

    typedef std::variant<
        TimeoutMessage,
        Timer, //defined in Timer.h
        CancelTimer,
        StatusMessage,
        NumNeighborsMessage,
        VoidMessage
        > Message;
        
}