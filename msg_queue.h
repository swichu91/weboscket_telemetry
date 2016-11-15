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
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

template<typename T>
class MsgQueue
{
	boost::mutex data_mutex;
	std::deque<T> data_queue;
    boost::unique_lock<boost::mutex> lk;
    boost::condition_variable notifier;

public:
	/* This is blocking call*/
	T rd(){
		T temp;
		boost::unique_lock<boost::mutex> lk(data_mutex);

		while(data_queue.empty()){
	        notifier.wait(lk);
		}
		temp=data_queue.back();
		data_queue.pop_back();
		return temp;
	}
	/* Non blocking call. It return void element on timeout */
    T rd(const boost::chrono::milliseconds& timeout){
        T temp;

        boost::lock_guard<boost::mutex> lock(data_mutex);

        while(data_queue.empty()){
            if(notifier.wait_for(lk,timeout) == boost::cv_status::timeout){
                return temp;
            }
        }
        temp=data_queue.back();
        data_queue.pop_back();
        return temp;
    }


	/* Blocking call ? */
	void wr(T& t){
	    boost::lock_guard<boost::mutex> lock(data_mutex);
		data_queue.push_back(t);
		notifier.notify_one();
	}

	bool isEmpty(){
	    return data_queue.empty();
	}
};


#endif /* MSG_QUEUE_H_ */
