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


using namespace std;

void print_usage()
{
    cout << "Usage: p <port number>" << endl;
}

int main(int argc, char** argv) {


    int opt;
    uint16_t port_nr = 55555;;

    while ((opt = getopt(argc,argv,"p:")) != EOF)
        switch(opt)
        {
            case 'p': port_nr = atoi(optarg); break;
            case '?': print_usage();exit(EXIT_FAILURE); break;
            default: print_usage(); exit(EXIT_FAILURE);
        }

    if(port_nr <=0){
        print_usage();
        exit(EXIT_FAILURE);
    }


    TelemetryServer s;
    s.run("", port_nr);


}



