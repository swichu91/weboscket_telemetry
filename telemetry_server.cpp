/*
 * telemetry_server.cpp
 *
 *  Created on: 3 lis 2016
 *      Author: Mateusz
 */


#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <fstream>
#include <iostream>
#include <set>
#include <streambuf>
#include <string>

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
class telemetry_server {
public:
    typedef websocketpp::connection_hdl connection_hdl;
    typedef websocketpp::server<websocketpp::config::asio> server;

    telemetry_server() : m_count(0) {
        // set up access channels to only log interesting things
        m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
        m_endpoint.set_access_channels(websocketpp::log::alevel::access_core);
        m_endpoint.set_access_channels(websocketpp::log::alevel::app);

        // Initialize the Asio transport policy
        m_endpoint.init_asio();

        // Bind the handlers we are using
        using websocketpp::lib::placeholders::_1;
        using websocketpp::lib::bind;
        m_endpoint.set_open_handler(bind(&telemetry_server::on_open,this,_1));
        m_endpoint.set_close_handler(bind(&telemetry_server::on_close,this,_1));
    }

    void run(std::string docroot, uint16_t port) {
        std::stringstream ss;
        ss << "Running telemetry server on port "<< port <<" using docroot=" << docroot;
        m_endpoint.get_alog().write(websocketpp::log::alevel::app,ss.str());

        m_docroot = docroot;

        // listen on specified port
        m_endpoint.listen(port);

        // Start the server accept loop
        m_endpoint.start_accept();

        // Set the initial timer to start telemetry
        set_timer();

        // Start the ASIO io_service run loop
        try {
            m_endpoint.run();
        } catch (websocketpp::exception const & e) {
            std::cout << e.what() << std::endl;
        }
    }

    void set_timer() {
        m_timer = m_endpoint.set_timer(
            1000,
            websocketpp::lib::bind(
                &telemetry_server::on_timer,
                this,
                websocketpp::lib::placeholders::_1
            )
        );
    }

    void on_timer(websocketpp::lib::error_code const & ec) {
        if (ec) {
            // there was an error, stop telemetry
            m_endpoint.get_alog().write(websocketpp::log::alevel::app,
                    "Timer Error: "+ec.message());
            return;
        }

        // Broadcast count to all connections
        con_list::iterator it;
        for (it = m_connections.begin(); it != m_connections.end(); ++it) {
          //  m_endpoint.send(*it,val.str(),websocketpp::frame::opcode::text);
        }

        // set timer for next telemetry check
        set_timer();
    }

    void on_open(connection_hdl hdl) {
        m_connections.insert(hdl);
    }

    void on_close(connection_hdl hdl) {
        m_connections.erase(hdl);
    }
private:
    typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;

    server m_endpoint;
    con_list m_connections;
    server::timer_ptr m_timer;

    std::string m_docroot;

    // Telemetry data
    uint64_t m_count;
};
