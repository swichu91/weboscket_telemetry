
#include "tempsensors_mod.h"

#include <sstream>
#include <thread>

void TempTest::MainThread_(TelemetryServer* inst){

    static int cnt =0;
    while(stop_thread_ == false)
    {
    	std::string test = queue_->rd(std::chrono::milliseconds(1));

    	if(test != ""){
    	    cmd_vect ret = ParseMsg(test);
    		InvokeHandlers(name_,ret);
    	}

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::string str = "testowy msg" + std::to_string(cnt++);

        cmd_vect cmd_vect;
        cmd_vect.push_back(std::make_pair("T1","25,4C"));
        cmd_vect.push_back(std::make_pair("T2","23,4C"));
        cmd_vect.push_back(std::make_pair("T3","22,4C"));
        cmd_vect.push_back(std::make_pair("T4","21,4C"));

        SendMsg(name_,cmd_vect);
    }

}
