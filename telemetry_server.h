/*
 * telemetry_server.h
 *
 *  Created on: 9 lis 2016
 *      Author: Mateusz
 */

#ifndef TELEMETRY_SERVER_H_
#define TELEMETRY_SERVER_H_


#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <fstream>
#include <iostream>
#include <set>
#include <streambuf>
#include <string>
#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/scoped_thread.hpp>
#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include "msg_queue.h"
#include <boost/thread/condition_variable.hpp>

#include "modules.h"

/**
 * The telemetry server accepts connections and sends a message every second to
 * each client containing an integer count. This example can be used as the
 * basis for programs that expose a stream of telemetry data for logging,
 * dashboards, etc.
 *
 * This example uses the timer based concurrency method and is self contained
 * and singled threaded. Refer to telemetry client for an example of a similar
 * telemetry setup using threads rather than timers.
 *
 * This example also includes an example simple HTTP server that serves a web
 * dashboard displaying the count. This simple design is suitable for use
 * delivering a small number of files to a small number of clients. It is ideal
 * for cases like embedded dashboards that don't want the complexity of an extra
 * HTTP server to serve static files.
 *
 * This design *will* fall over under high traffic or DoS conditions. In such
 * cases you are much better off proxying to a real HTTP server for the http
 * requests.
 */

/* Friends functions don't have access to typedefs inside class */
typedef websocketpp::connection_hdl connection_hdl;
typedef websocketpp::server<websocketpp::config::asio> server;
// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;
typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;

class TelemetryServer {
public:


    TelemetryServer();
    ~TelemetryServer();

    /* Start TelemetrySocket server. */
    void run(std::string docroot, uint16_t port);

    /* API to pass/get messages from websocket.
     * In current version TelemetryServer accepts only string data. Maybe I will expand this functionality in future. */
    void send_msg(std::string&);
    void get_msg(std::string&);

    /* Module wrappers */
    Modules::mod_ret Register(const std::string& s, MsgQueue<std::string>* const msgq){
       return modules_.Register(s,msgq);
    }



    friend void outputdata_worker(TelemetryServer* inst);
    friend void inputdata_worker(TelemetryServer* inst);

private:

    /* Private module wrappers */
    Modules::mod_ret UnregisterAll(){
       return modules_.UnregisterAll();
    }

    void set_timer();

    void on_timer(websocketpp::lib::error_code const & ec);

    void on_open(connection_hdl hdl) {
        m_connections.insert(hdl);
    }

    void on_close(connection_hdl hdl) {
        m_connections.erase(hdl);
    }

    void on_message(connection_hdl hdl, message_ptr msg);

    Modules modules_;
    server m_endpoint;
    con_list m_connections;
    server::timer_ptr m_timer;

    MsgQueue<std::string> output_queue;
    MsgQueue<std::string> input_queue;
};


#endif /* TELEMETRY_SERVER_H_ */
