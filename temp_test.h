
#include <iostream>
#include "module_interface.h"
#include "telemetry_server.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/thread.hpp>

class TempTest  : public ModuleInterface {

public:
    TempTest(std::string& name,TelemetryServer* inst): ModuleInterface(inst) {
        inst_=inst;
        queue_ = boost::make_shared<MsgQueue<std::string>>();
        inst_->RegisterModule(const_cast<std::string&>(name),queue_);
    }
    ~TempTest();

    /* Methods below are pure virtual and need to be implemented */
    void run();
    //void ParseMsg(std::string&){};
    void BuildMsg(){};

private:
    boost::shared_ptr<MsgQueue<std::string>> queue_;
    void MainThread_(TelemetryServer* inst);
};
