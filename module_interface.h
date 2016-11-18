
#include "msg_queue.h"
#include "telemetry_server.h"

#include <iostream>
#include <boost/thread.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/bind.hpp>

class ModuleInterface
{


public:
/*    ModuleInterface(std::string& name,TelemetryServer* inst,module_ptr mptr){
        inst->RegisterModule(name,mptr);
    }*/

    virtual ~ModuleInterface(){};

    virtual void run() =0;


};
