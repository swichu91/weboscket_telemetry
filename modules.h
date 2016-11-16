
#include <iostream>
#include <map>
#include <boost/algorithm/string.hpp>
#include "msg_queue.h"

class Modules
{
    typedef std::map<std::string,MsgQueue<std::string>* >  module;
    module modules_;

public:

    typedef enum{
        OK = 0,
        ALREADY_REGISTERED = -1,
        NOT_FOUND = -2
    }mod_ret;

    Modules(){};
    ~Modules(){};

    mod_ret Register(const std::string& s,MsgQueue<std::string>* const msgq){

        module::iterator it;

        it = modules_.find(s);
         if (it != modules_.end()){
             /* Module has been already registered */
             return ALREADY_REGISTERED;
         }else{
             modules_[s] = msgq;
             return OK;
         }

    }
    mod_ret Unregister(const std::string& s){

        module::iterator it;
        it = modules_.find(s);
         if (it != modules_.end()){
             modules_.erase (it);
             return OK;
         }else{
             return NOT_FOUND;
         }
    }

    mod_ret UnregisterAll(){
        modules_.clear();
        return OK;
    }

    mod_ret IsRegistered(const std::string& s){
        module::iterator it;
        it = modules_.find(s);
         if (it != modules_.end()){
             return OK;
         }else{
             return NOT_FOUND;
         }
    }

    void ParseAndRouteMsg(const std::string& s){

    	std::vector<std::string> strs;
    	boost::split(strs,s, boost::is_any_of(":"));


    	module::iterator it;
    	it = modules_.find(strs[0]);
    	/* 0 element contains name od module */
    	if(it != modules_.end()){
    		/* Ok, msg can be passed to module cause it's been already loaded */
    		std::cout << "Msg: " << strs[1] <<" passed to module: " << strs[0] << std::endl;
    		modules_[strs[0]]->wr(strs[1]);
    	}else{
    		//TODO: silently ignore this request
    		std::cout << "Module not found !" << std::endl;
    	}

    }

};
