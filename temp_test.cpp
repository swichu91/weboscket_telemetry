
#include "temp_test.h"
/*
#include "module_interface.h"
#include "telemetry_server.h"
#include <iostream>
*/

#include <sstream>

TempTest::~TempTest(){}

void TempTest::run(){
    /*boost::scoped_thread<> mod1_threadboost::thread{MainThread_,this,inst_};*/
    /* only temporary solution. I need more elegant way to finish thread */
    boost::thread mythread{MainThread_,this,inst_};
}


void TempTest::MainThread_(TelemetryServer* inst){

    static int cnt =0;
    while(1)
    {
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        std::stringstream ss;
        ss << cnt++;
        std::string str = "testowy msg" + ss.str();
        inst->SendMsg(str);
    }

}
