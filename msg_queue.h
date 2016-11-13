/*
 * msg_queue.h
 *
 *  Created on: 10 lis 2016
 *      Author: Mateusz
 */

#ifndef MSG_QUEUE_H_
#define MSG_QUEUE_H_

#include <boost/thread.hpp>
#include <deque>
#include <iostream>

template<typename T>
class MsgQueue
{
	boost::mutex data_mutex;
	std::deque<T> data_queue;

	/* Blocking call ? */
	T& rd(){
		T temp;
		data_mutex.lock();
		temp=data_queue.back();
		data_queue.pop_back();
		data_mutex.unlock();
		return temp;
	}

	/* Blocking call ? */
	void wr(T& t){
		data_mutex.lock();
		data_queue.push_back(t);
		data_mutex.unlock();
	}
};


#endif /* MSG_QUEUE_H_ */
