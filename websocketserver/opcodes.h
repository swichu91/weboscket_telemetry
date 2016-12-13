/*
 * opcodes.h
 *
 *  Created on: 21 lis 2016
 *      Author: Mateusz
 */

#ifndef OPCODES_H_
#define OPCODES_H_

#include <iostream>
#include <string>
#include <vector>

typedef std::vector<std::pair<std::string,std::string>> cmd_vect;


/* Opcodes*/
typedef enum{

	Code_Data_Req = 0,/* Asynchronous data request*/
	Code_Data_Cyclic = 1,/* Synchronous data request*/
	Code_Stop = 2, /* Put module into silent mode(it won't send any data to webserver)*/

	/* .. */
	Code_Mod_Specific =66, /* Pass module specific commands into corresponding module */

	/* TBD */
} opcodes;

typedef enum{
	Cmd_Not_Found = 0,
	Cmd_Executed = 1,
	Cmd_Error = 2,
	Cmd_Not_Implemented = 3,
}ret;

class Opcodes
{
public:

	typedef int handler_param;


private:
    std::string RetToStr(std::string& name,ret r){
        switch(r){
            case Cmd_Not_Found:
                return name + ":Cmd not found\r\n";
            case Cmd_Executed:
                return name + ":Cmd executed\r\n";
            case Cmd_Error:
                return name + ":Cmd error\r\n";
            case Cmd_Not_Implemented:
                return name + ":Body of handler not implemented";
            default:
                return name + ":Error";
        }
    }

public:

	/* Each of opcode handler needs to be implemented inside specific module. */
	virtual ret Handler_Data_Req(handler_param){return Cmd_Not_Implemented;}
	virtual ret Handler_Data_Cyclic(handler_param){return Cmd_Not_Implemented;}
	virtual ret Handler_Stop(handler_param){return Cmd_Not_Implemented;}
	virtual ret Handler_Mod_Specific(handler_param){return Cmd_Not_Implemented;}

	virtual ~Opcodes(){};

	/* Invoke all commands that cmd vector contains. */
	void InvokeHandlers(std::string& name,cmd_vect& cmdval){

	    for(auto i: cmdval){

            switch(std::stoi(i.first,nullptr,10)){

            case Code_Data_Req:
                std::cout << RetToStr(name,Handler_Data_Req(std::stoi(i.second,nullptr,10)));
                break;
            case Code_Data_Cyclic:
                std::cout << RetToStr(name,Handler_Data_Cyclic(std::stoi(i.second,nullptr,10)));
                break;
            case Code_Mod_Specific:
                std::cout << RetToStr(name,Handler_Mod_Specific(std::stoi(i.second,nullptr,10)));
                break;
            case Code_Stop:
                std::cout << RetToStr(name,Handler_Stop(std::stoi(i.second,nullptr,10)));
                break;
            default:
                std::cout << RetToStr(name,Cmd_Not_Found);
            }

	    }
	}

};



#endif /* OPCODES_H_ */
