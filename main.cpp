/*
 * main.cpp
 *
 *  Created on: 1 lis 2016
 *      Author: Mateusz
 */


#include <iostream>
#include <unistd.h>

#include "tempsensors_mod/tempsensors_mod.h"
#include "websocketserver/websocketserver.h"


using namespace std;

void print_usage()
{
    cout << "Usage: p <port number>" << endl;
    cout << "       w <threads number>" << endl;
}

#include "websocketserver/msgqueue.h"

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

    s.run("", port_nr);

}



