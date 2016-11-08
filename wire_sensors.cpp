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
/*	ifstream slaves("/sys/bus/w1/devices/w1_bus_master1/w1_master_slaves");

	if (!slaves.is_open()) {
		cout << "Error opening slaves list file. Maybe you forgot load Device Tree Overlay ?!\n";
	}
	string temp;
	string path = "/sys/bus/w1/devices/";
	SensorData sensor;

	sensor_cnt = 0;
	//sensors_tab.clear();
	while (getline(slaves, temp)) {
		sensor.nr = sensor_cnt+1;
		sensor.id = temp;
		sensor.path = path + temp;
		//sensors_tab.push_back(sensor);
		sensor_cnt++;
	}

	slaves.close();*/
}

void WireSensors::PerformTempRead() {

	string file_line;

	sensors_matrix::iterator it = sensors_tab.begin();

	while(it != sensors_tab.end()){

		ifstream file((*it).path + "/w1_slave");
		if (!file.is_open()) {
			cout << "error opening file" << endl;
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


WireSensors *WireSensors::instance = 0;
int WireSensors::sensor_cnt = 0;

