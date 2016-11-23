
#include "temp_test.h"
/*
#include "module_interface.h"
#include "telemetry_server.h"
#include <iostream>
*/

#include <sstream>
#include <thread>

TempTest::~TempTest(){}

void TempTest::run(){
    /*boost::scoped_thread<> mod1_threadboost::thread{MainThread_,this,inst_};*/
    /* only temporary solution. I need more elegant way to finish thread */
    std::thread mythread{MainThread_,this,inst_};
}


void TempTest::MainThread_(TelemetryServer* inst){

    static int cnt =0;
    while(1)
    {
    	std::string test = queue_->rd(std::chrono::milliseconds(1));

    	if(test != ""){
    	    cmd_vect ret = ParseMsg(test);
    		InvokeHandlers(ret);
    	}

        /*std::this_thread::sleep_for(std::chrono::milliseconds(1));

        std::string str = "testowy msg" + std::to_string(cnt++);

        std::pair<std::string,std::string> temp;

        temp.first = "T1";
        temp.second = "25,4C";

        cmd_vect cmd_vect;
        cmd_vect.push_back(temp);

        SendMsg(name_,cmd_vect);*/
    }

}
