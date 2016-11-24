/*
 * wire_sensors.h
 *
 *  Created on: 3 lis 2016
 *      Author: Mateusz
 */

#ifndef WIRE_SENSORS_H_
#define WIRE_SENSORS_H_

#include <iostream>
#include <string>
#include <vector>

struct SensorData{
public:
	int nr;
	std::string path;
	std::string id;
	std::string temp_str;
	float	temp;

	SensorData(int _nr,std::string _path,std::string _id,std::string _temp_str=0,float _temp=0){
	    nr = _nr;
	    path = _path;
	    id = _id;
	    temp_str = _temp_str;
	    temp = _temp;
	};
	SensorData() : nr(0),temp(0){};
};

typedef std::vector<SensorData> sensors_matrix;

class WireSensors
{
	static WireSensors* instance;
	static int sensor_cnt;

	sensors_matrix	sensors_tab;

	WireSensors();
	~WireSensors();


public:

	static WireSensors* Instance();


	void PerformTempRead();
	void ProbeBus();

	/* Sensors numbers start from 0 */
	sensors_matrix GetSensorsData(){
		return sensors_tab;
	}

};



#endif /* WIRE_SENSORS_H_ */
