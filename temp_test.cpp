
#include "temp_test.h"
/*
#include "module_interface.h"
#include "telemetry_server.h"
#include <iostream>
*/

TempTest::~TempTest(){}

void TempTest::run(){
    /*boost::scoped_thread<> mod1_threadboost::thread{MainThread_,this,inst_};*/
    /* only temporary solution. We need more elegant way to finish thread */
    boost::thread mythread{MainThread_,this,inst_};
}


void TempTest::MainThread_(TelemetryServer* inst){

    while(1)
    {
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        std::string str = "testowy msg1 !\r\n";
        inst->SendMsg(str);
    }

}
