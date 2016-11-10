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

class TelemetryServer {
public:
    typedef websocketpp::connection_hdl connection_hdl;
    typedef websocketpp::server<websocketpp::config::asio> server;
    // pull out the type of messages sent by our config
    typedef server::message_ptr message_ptr;

    TelemetryServer();

    void run(std::string docroot, uint16_t port);

    void set_timer();

    void on_timer(websocketpp::lib::error_code const & ec);

    void on_open(connection_hdl hdl) {
        m_connections.insert(hdl);
    }

    void on_close(connection_hdl hdl) {
        m_connections.erase(hdl);
    }

    void on_message(connection_hdl hdl, message_ptr msg);

private:
    typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;

    server m_endpoint;
    con_list m_connections;
    server::timer_ptr m_timer;

    std::string m_docroot;

    // Telemetry data
    uint64_t m_count;
};


#endif /* TELEMETRY_SERVER_H_ */
