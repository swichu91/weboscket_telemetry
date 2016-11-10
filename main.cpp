/*
 * main.cpp
 *
 *  Created on: 1 lis 2016
 *      Author: Mateusz
 */


#include <iostream>
#include "wire_sensors.h"
#include "telemetry_server.h"
/* For getopt */
#include <unistd.h>
#include <boost/thread.hpp>


using namespace std;

void print_usage()
{
    cout << "Usage: p <port number>" << endl;
    cout << "       w <threads number>" << endl;
}

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

    boost::thread_group worker_threads;

    TelemetryServer s;
    s.run("", port_nr);


}



