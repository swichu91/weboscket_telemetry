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

class Opcodes
{
	private:
	typedef int handler_param;


	/* Opcodes*/
	typedef enum{

		Code_Data_Req = 0,/* Asynchronous data request*/
		Code_Data_Cyclic = 1,/* Synchronous data request*/

		/* TBD */
	} opcodes;


public:

	typedef enum{
		Cmd_Not_Found = 0,
		Cmd_Executed = 1,
		Cmd_Error = 2,
		Cmd_Not_Implemented = 3,
	}ret;

	/* Each of opcode handler needs to be implemented inside specific module. */
	/*virtual */ret Handler_Data_Req(handler_param){return Cmd_Not_Implemented;}
	/*virtual */ret Handler_Data_Cyclic(handler_param){return Cmd_Not_Implemented;}


	virtual ~Opcodes(){};

	ret InvokeHandler(std::pair<std::string,std::string>& cmdval){

		switch(std::stoi(cmdval.first,nullptr,10)){

		/* Code_Data_Req */
		case Code_Data_Req:
			return Handler_Data_Req(std::stoi(cmdval.second,nullptr,10));
		case Code_Data_Cyclic:
			return Handler_Data_Cyclic(std::stoi(cmdval.second,nullptr,10));

		default:
			return Cmd_Not_Found;
		}

	}

};



#endif /* OPCODES_H_ */
