
#include "msg_queue.h"
#include "telemetry_server.h"

#include <iostream>
#include <boost/thread.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/bind.hpp>

class ModuleInterface
{
protected:
    TelemetryServer* inst_;

    /** Incoming message format: 'Cmd1:arg1,Cmd2:arg2 and so on' */
    virtual void ParseMsg(std::string&) =0;

    /** Message format: 'Module name+arg1:val1,Module name+arg2:val2 and so on */
    virtual void SendMsg() =0;

public:
    ModuleInterface(TelemetryServer* inst){
        inst_ =inst;
    }

    virtual ~ModuleInterface(){};

    virtual void run() =0;


};
