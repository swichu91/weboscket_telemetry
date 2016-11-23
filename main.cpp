/*
 * main.cpp
 *
 *  Created on: 1 lis 2016
 *      Author: Mateusz
 */


#include <iostream>
#include <boost/bind.hpp>
#include "wire_sensors.h"
#include "telemetry_server.h"
/* For getopt */
#include <unistd.h>
#include <boost/thread.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>

#include "temp_test.h"


using namespace std;

void print_usage()
{
    cout << "Usage: p <port number>" << endl;
    cout << "       w <threads number>" << endl;
}

#include "msg_queue.h"

int main(int argc, char** argv) {

    int opt;
    /* Default port number */
    uint16_t port_nr = 55555;
    uint16_t thread_nr = 2;

    while ((opt = getopt(argc,argv,"p:")) != EOF)
        switch(opt)
        {
            case 'p': port_nr = atoi(optarg); break;
            case 'w': thread_nr = atoi(optarg); break;
            case '?': print_usage();exit(EXIT_FAILURE); break;
            default: print_usage(); exit(EXIT_FAILURE);
        }

    if((port_nr <=0) || (thread_nr<2)){
        print_usage();
        exit(EXIT_FAILURE);
    }

    TelemetryServer s;

    std::string module1 = "tempm";
    TempTest temptest(module1,&s);
    //temptest.run();

   // boost::scoped_thread<> mod1_thread{boost::thread{module1,&s}};
    //boost::scoped_thread<> mod2_thread{boost::thread{module2,&s}};

    s.run("", port_nr);

}



