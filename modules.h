
#include <iostream>
#include <map>
#include <thread>
#include <mutex>
#include <boost/algorithm/string.hpp>
#include "msg_queue.h"

typedef std::shared_ptr<MsgQueue<std::string>> module_ptr;

class Modules
{
    typedef std::map<std::string,module_ptr>  module;
    module modules_;
    std::mutex lock;

public:

    typedef enum{
        OK = 0,
        ALREADY_REGISTERED = -1,
        NOT_FOUND = -2
    }mod_ret;

    Modules(){};
    ~Modules(){};

    mod_ret Register(const std::string& s,module_ptr msgq){

        module::iterator it;

        std::unique_lock<std::mutex> lk(lock);
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
        std::unique_lock<std::mutex> lk(lock);
        it = modules_.find(s);
         if (it != modules_.end()){
             modules_.erase (it);
             return OK;
         }else{
             return NOT_FOUND;
         }
    }

    mod_ret UnregisterAll(){
    	std::unique_lock<std::mutex> lk(lock);
        modules_.clear();
        return OK;
    }

    mod_ret IsRegistered(const std::string& s){
        module::iterator it;

        std::unique_lock<std::mutex> lk(lock);
        it = modules_.find(s);
         if (it != modules_.end()){
             return OK;
         }else{
             return NOT_FOUND;
         }
    }

    void RouteMsg(const std::string& s){

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
    		std::cout << "Module:" << strs[0] << " not found !" << std::endl;
    	}

    }

};
