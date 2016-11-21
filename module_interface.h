
#include "msg_queue.h"
#include "telemetry_server.h"

#include <iostream>
#include <boost/thread.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/tokenizer.hpp>

class ModuleInterface
{
protected:
    TelemetryServer* inst_;

    /** Incoming message format: 'Cmd1:arg1,Cmd2:arg2 and so on' */
    void ParseMsg(std::string& s){
        std::vector<std::pair<std::string,std::string>> cmdval;
        typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
        boost::char_separator<char> sep{","};
        tokenizer tok{s,sep};

        /* Split input string into cmd&val pairs.*/
         for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it){

             boost::char_separator<char> sep{":"};
             tokenizer tok1{*it,sep};

             int pcnt =1;
             std::pair<std::string,std::string> temp_pair;
             /* Possible input: cmd:val, cmd: or cmd */
             for(tokenizer::iterator it1 = tok1.begin();it1 !=tok1.end();++it1){

                 if(pcnt == 1){
                     temp_pair.first = *it1;
                 }else if(pcnt == 2){
                     temp_pair.second = *it1;
                 }
                 pcnt++;
             }
             cmdval.push_back(temp_pair);
         }

         for(std::vector<std::pair<std::string,std::string>>::iterator it = cmdval.begin(); it!= cmdval.end();++it){

             std::cout << (*it).first << "->" << (*it).second << std::endl;
         }



    }

    /** Message format: 'Module name+arg1:val1,Module name+arg2:val2 and so on */
    virtual void BuildMsg() =0;

    void SendMsg(std::string& msg){
        inst_->SendMsg(msg);
    }

public:
    ModuleInterface(TelemetryServer* inst){
        inst_ =inst;
    }

    virtual ~ModuleInterface(){};

    virtual void run() =0;


};
