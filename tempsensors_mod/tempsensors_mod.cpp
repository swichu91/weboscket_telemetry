
#include "tempsensors_mod.h"
#include "wire_sensors.h"

#include <sstream>
#include <thread>




/* Virtual methods below.. */
ret TempTest::Handler_Data_Req(handler_param){

	/* On windows we're generating dummy test data */
#if _WIN32

	/* Send test messages on request */
    cmd_vect cmd_vect;
    cmd_vect.push_back(std::make_pair("T1","25,4C"));
    cmd_vect.push_back(std::make_pair("T2","23,4C"));
    cmd_vect.push_back(std::make_pair("T3","22,4C"));
    cmd_vect.push_back(std::make_pair("T4","21,4C"));
    SendMsg(name_,cmd_vect);
#else



#endif

	return Cmd_Executed;
}
ret TempTest::Handler_Data_Cyclic(handler_param){
	cyclic_data_ = true;
	return Cmd_Executed;
}
ret TempTest::Handler_Stop(handler_param){
	cyclic_data_ = false;
	return Cmd_Executed;
}
ret TempTest::Handler_Mod_Specific(handler_param){
	return Cmd_Executed;
}

void TempTest::MainThread_(TelemetryServer* inst){

	WireSensors* w1 = WireSensors::Instance();

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

        if(cyclic_data_){
            cmd_vect cmd_vect;

#if _WIN32
        	/* Send test message on request */
            cmd_vect.push_back(std::make_pair("T1","30C"));
            cmd_vect.push_back(std::make_pair("T2","31C"));
            cmd_vect.push_back(std::make_pair("T3","32C"));
            cmd_vect.push_back(std::make_pair("T4","33C"));
            SendMsg(name_,cmd_vect);
//#else
        	//w1->ProbeBus();
        	//w1->PerformTempRead();

            for(auto i: w1->GetSensorsData()){
            	cmd_vect.push_back(std::make_pair("T:"+i.nr,i.temp_str + "C"));
            }
            SendMsg(name_,cmd_vect);


#endif
        }

    }

}
