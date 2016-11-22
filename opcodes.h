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

typedef std::vector<std::pair<std::string,std::string>> cmd_vect;

class Opcodes
{
public:

	typedef int handler_param;


	/* Opcodes*/
	typedef enum{

		Code_Data_Req = 0,/* Asynchronous data request*/
		Code_Data_Cyclic = 1,/* Synchronous data request*/
		Code_Stop = 2, /* Put module into silent mode(it won't send any data do webserver)*/

		/* TBD */
	} opcodes;

	typedef enum{
		Cmd_Not_Found = 0,
		Cmd_Executed = 1,
		Cmd_Error = 2,
		Cmd_Not_Implemented = 3,
	}ret;

	/* Each of opcode handler needs to be implemented inside specific module. */
	virtual ret Handler_Data_Req(handler_param){return Cmd_Not_Implemented;}
	virtual ret Handler_Data_Cyclic(handler_param){return Cmd_Not_Implemented;}
	virtual ret Handler_Stop(handler_param){return Cmd_Not_Implemented;}

	virtual ~Opcodes(){};

	void InvokeHandlers(cmd_vect& cmdval){

	    for(auto i: cmdval){

            switch(std::stoi(i.first,nullptr,10)){

            /* Code_Data_Req */
            case Code_Data_Req:
                std::cout << Handler_Data_Req(std::stoi(i.second,nullptr,10)) << std::endl;
                break;
            case Code_Data_Cyclic:
                std::cout << Handler_Data_Cyclic(std::stoi(i.second,nullptr,10)) << std::endl;
                break;
            default:
                std::cout << Cmd_Not_Found << std::endl;
            }

	    }
	}

};



#endif /* OPCODES_H_ */
