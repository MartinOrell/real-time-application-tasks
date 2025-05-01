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

    struct RespondDigit{
        int fromId;
        int digit;
    };

    struct ConnectDigit{
        int digitId;
    };

    enum VoidMessage{EndMessage, IncMessage, RequestDigit};

    typedef std::variant<
        TimeoutMessage,
        Timer, //defined in Timer.h
        CancelTimer,
        RespondDigit,
        ConnectDigit,
        VoidMessage
        > Message;
        
}