/*
 * msg_queue.h
 *
 *  Created on: 10 lis 2016
 *      Author: Mateusz
 */

#ifndef MSG_QUEUE_H_
#define MSG_QUEUE_H_

#include <condition_variable>
#include <thread>
#include <deque>
#include <iostream>
#include <mutex>

template<typename T>
class MsgQueue
{
	std::mutex data_mutex;
	std::deque<T> data_queue;
    std::condition_variable notifier;

public:
	/* This is blocking call*/
	T rd(){
		T temp;
		std::unique_lock<std::mutex> lk(data_mutex);

		while(data_queue.empty()){
	        notifier.wait(lk);
		}
		temp=data_queue.back();
		data_queue.pop_back();
		return temp;
	}
	/* Non blocking call. It return void element on timeout */
    T rd(const std::chrono::milliseconds& timeout){
        T temp;

        std::unique_lock<std::mutex> lk(data_mutex);

        while(data_queue.empty()){
            if(notifier.wait_for(lk,timeout) == std::cv_status::timeout){
                return temp;
            }
        }
        temp=data_queue.back();
        data_queue.pop_back();
        return temp;
    }


	/* Blocking call ? */
	void wr(T& t){
	    std::lock_guard<std::mutex> lock(data_mutex);
		data_queue.push_back(t);
		notifier.notify_one();
	}

	bool isEmpty(){
	    return data_queue.empty();
	}
};


#endif /* MSG_QUEUE_H_ */
