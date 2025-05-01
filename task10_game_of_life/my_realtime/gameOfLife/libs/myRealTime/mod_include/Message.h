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

    struct CellToCellStatusMessage{
        bool isAlive;
        bool sync;
    };

    enum VoidMessage{EndMessage, TogglePlay, ToggleStatus, Tick};

    typedef std::variant<
        TimeoutMessage,
        Timer, //defined in Timer.h
        CancelTimer,
        StatusMessage,
        NumNeighborsMessage,
        CellToCellStatusMessage,
        VoidMessage
        > Message;
        
}