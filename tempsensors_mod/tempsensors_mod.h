

#ifndef TEMP_TEST_H_
#define TEMP_TEST_H_

#include <iostream>
#include <thread>

#include "../websocketserver/modinterface.h"
#include "../websocketserver/websocketserver.h"

class TempTest  : public ModuleInterface, public Opcodes {

public:

    /* Worker thread will be called as soon as class is created */
    TempTest(std::string& name,TelemetryServer* inst): ModuleInterface(inst) {
        name_ = name;
        inst_=inst;
        queue_ = std::make_shared<MsgQueue<std::string>>();

        inst_->RegisterModule(const_cast<std::string&>(name_),queue_);

        worker_thread_ = std::thread(&TempTest::MainThread_,this,inst_);
    }
    ~TempTest(){
        stop_thread_ = true;
        if(worker_thread_.joinable()){
            worker_thread_.join();
        }
    }

    /* Virtual methods below.. */
    ret Handler_Data_Req(handler_param);
    ret Handler_Data_Cyclic(handler_param);
    ret Handler_Stop(handler_param);
    ret Handler_Mod_Specific(handler_param);

private:
    bool stop_thread_ = false;
    std::thread worker_thread_;
    std::string name_;
    std::shared_ptr<MsgQueue<std::string>> queue_;
    void MainThread_(TelemetryServer* inst);

    bool cyclic_data_ = false;
};

#endif /*TEMP_TEST */
