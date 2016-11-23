

#ifndef TEMP_TEST_H_
#define TEMP_TEST_H_

#include <iostream>
#include "module_interface.h"
#include "telemetry_server.h"
#include <thread>

class TempTest  : public ModuleInterface, public Opcodes {

public:
    TempTest(std::string& name,TelemetryServer* inst): ModuleInterface(inst) {
        name_ = name;
        inst_=inst;
        queue_ = std::make_shared<MsgQueue<std::string>>();
        inst_->RegisterModule(const_cast<std::string&>(name_),queue_);
    }
    ~TempTest();

    /* Methods below are pure virtual and need to be implemented */
    void run();

    ret Handler_Data_Req(handler_param){return Cmd_Executed;}
    ret Handler_Data_Cyclic(handler_param){return Cmd_Executed;}
    ret Handler_Stop(handler_param){return Cmd_Executed;}

private:
    std::string name_;
    std::shared_ptr<MsgQueue<std::string>> queue_;
    void MainThread_(TelemetryServer* inst);
};

#endif /*TEMP_TEST */
