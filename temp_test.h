
#include <iostream>
#include "module_interface.h"
#include "telemetry_server.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/thread.hpp>

class TempTest  : public ModuleInterface {

public:
    TempTest(std::string& name,TelemetryServer* inst){
        inst_=inst;
        queue_ = boost::make_shared<MsgQueue<std::string>>();
        inst_->RegisterModule(const_cast<std::string&>(name),queue_);

        //boost::scoped_thread<> mod1_thread{boost::thread{MainThread_,this,inst_}};
        //inst_->RegisterModule(const_cast<std::string&>(name),queue_);

    }
    ~TempTest();

    void run();

private:
    TelemetryServer* inst_;
    boost::shared_ptr<MsgQueue<std::string>> queue_;
    void MainThread_(TelemetryServer* inst);
};
