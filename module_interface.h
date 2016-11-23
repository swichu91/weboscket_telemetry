
#ifndef MODULE_INTERFACE_H_
#define MODULE_INTERFACE_H_

#include "msg_queue.h"
#include "telemetry_server.h"
#include "opcodes.h"
#include <iostream>
#include <boost/tokenizer.hpp>

class ModuleInterface
{
    /* TODO: support for bounding 2 args into one cmd*/
	enum{TOKEN_CMD=1,TOKEN_VAL1=2,TOKEN_VAL2=3};

protected:
    TelemetryServer* inst_;

    /** Incoming message format: 'Cmd1=arg1,Cmd2=arg2 and so on' */
    cmd_vect ParseMsg(std::string& s){

        typedef boost::tokenizer<boost::char_separator<char>> tokenizer;

        cmd_vect cmdval;
        boost::char_separator<char> sep{","};
        tokenizer tok{s,sep};

        /* Split input string into cmd&val pairs.*/
         for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it){

             boost::char_separator<char> sep{"="};
             tokenizer tok1{*it,sep};

             int pcnt = TOKEN_CMD;
             std::pair<std::string,std::string> temp_pair;
             /* Possible input: cmd=val, cmd= or cmd */
             for(tokenizer::iterator it1 = tok1.begin();it1 !=tok1.end();++it1){

                 if(pcnt == TOKEN_CMD){
                     temp_pair.first = *it1;
                 }else if(pcnt == TOKEN_VAL1){
                     temp_pair.second = *it1;
                 }
                 ++pcnt;
             }
             cmdval.push_back(temp_pair);
         }
         /* Only for debug purposes */
         for(cmd_vect::iterator it = cmdval.begin(); it!= cmdval.end();++it){
             std::cout << (*it).first << "->" << (*it).second << std::endl;
         }

         return cmdval;

    }

    ModuleInterface(TelemetryServer* inst){
        inst_ =inst;
    }

    /** Message format: 'Module name_arg1:val1,Module name_arg2:val2 and so on */
    void SendMsg(const std::string& name,cmd_vect& vect){
        std::string cmd_string;

        for(auto i: vect){
            cmd_string += name + "_" + i.first + ":" + i.second + ",";
        }

        inst_->SendMsg(cmd_string);
    }

    /** Start specific module thread */
    virtual void run() =0;

    /** Virtual destructor */
    virtual ~ModuleInterface(){};

};

#endif
