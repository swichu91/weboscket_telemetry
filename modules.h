
#include <iostream>
#include <map>

class Modules
{
    typedef std::set<std::string,int>  module;
    module modules_;

public:

    typedef enum{
        OK = 0,
        ALREADY_REGISTERED = -1,
        NOT_FOUND = -2
    }mod_ret;

    Modules();
    ~Modules();

    mod_ret Register(const std::string& s){

        module::iterator it;

        it = modules_.find(s);
         if (it != modules_.end()){
             /* Module has been already registered */
             return ALREADY_REGISTERED;
         }else{
             modules_[s] = 0;
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

    mod_ret IsRegistered(const std::string& s){
        module::iterator it;
        it = modules_.find(s);
         if (it != modules_.end()){
             return OK;
         }else{
             return NOT_FOUND;
         }
    }

};
