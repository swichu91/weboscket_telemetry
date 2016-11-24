/*
 * wire_sensors.cpp
 *
 *  Created on: 3 lis 2016
 *      Author: Mateusz
 */

#include "wire_sensors.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

WireSensors::WireSensors()
{
}

WireSensors::~WireSensors() {
	delete instance;
}
WireSensors* WireSensors::Instance()
{
	if (!instance){
		instance = new WireSensors;
	}
	return instance;
}

void WireSensors::ProbeBus() {

    try
    {
        ifstream slaves("/sys/bus/w1/devices/w1_bus_master1/w1_master_slaves");

        if (!slaves.is_open()) {
            throw "Error opening slaves list file. Maybe you forgot load Device Tree Overlay ?!\n";
        }
        string temp;
        string path = "/sys/bus/w1/devices/";


        sensor_cnt = 0;
        sensors_tab.clear();
        while (getline(slaves, temp)) {
            SensorData sensor(sensor_cnt+1,path + temp,temp);
            sensors_tab.push_back(sensor);
            sensor_cnt++;
        }

        slaves.close();
    }
    catch (string& e)
    {
        cout << e;
    }

}

void WireSensors::PerformTempRead() {

    try {
        string file_line;

        sensors_matrix::iterator it = sensors_tab.begin();

        while(it != sensors_tab.end()){

            ifstream file((*it).path + "/w1_slave");
            if (!file.is_open()) {
                throw "error opening file";
            }

            getline(file, file_line);
            getline(file, file_line);
            string delimiter = "t=";
            string temp_token = file_line.substr(file_line.find(delimiter) + delimiter.size(), 100);

            temp_token.insert(2, ".");
            temp_token.resize(temp_token.size() - 1);

            (*it).temp_str = temp_token;
            //(*it).temp = stof(temp_token);

            file.close();


        }

    }
    catch(string& e){
        cout << e;
    }

}


WireSensors *WireSensors::instance = 0;
int WireSensors::sensor_cnt = 0;

