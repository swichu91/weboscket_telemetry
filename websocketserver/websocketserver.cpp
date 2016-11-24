/*
* telemetry_server.cpp
*
*  Created on: 3 lis 2016
*      Author: Mateusz
*/

#include "websocketserver.h"

#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <fstream>
#include <iostream>
#include <set>
#include <streambuf>
#include <string>
#include <sstream>
#include <boost/thread/scoped_thread.hpp>
#include <boost/thread/thread.hpp>


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

void outputdata_worker(TelemetryServer* inst){

	while(1){
	    std::string temp;

	    temp = inst->output_queue.rd();

	    for(auto it : inst->m_connections){
	        inst->m_endpoint.send(it,temp,websocketpp::frame::opcode::text);
	    }
	}
}

TelemetryServer::TelemetryServer(){
    // set up access channels to only log interesting things
    m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
    m_endpoint.set_access_channels(websocketpp::log::alevel::access_core);
    m_endpoint.set_access_channels(websocketpp::log::alevel::app);

    // Initialize the Asio transport policy
    m_endpoint.init_asio();

    // Bind the handlers we are using
    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;
    //using websocketpp::lib::placeholders::_3;
    using websocketpp::lib::bind;
    m_endpoint.set_open_handler(bind(&TelemetryServer::on_open,this,_1));
    m_endpoint.set_close_handler(bind(&TelemetryServer::on_close,this,_1));
    m_endpoint.set_message_handler(bind(&TelemetryServer::on_message,this,_1,_2));
}

TelemetryServer::~TelemetryServer()
{
    UnregisterAll();
	//outputdata_thread.join();
	//inputdata_thread.join();
}


void TelemetryServer::run(std::string docroot, uint16_t port) {
    std::stringstream ss;
    ss << "Running telemetry server on port "<< port <<" using docroot=" << docroot;
    m_endpoint.get_alog().write(websocketpp::log::alevel::app,ss.str());

    // listen on specified port
    m_endpoint.listen(port);

    // Start the server accept loop
    m_endpoint.start_accept();

    // Set the initial timer to start telemetry
    //set_timer();

    /*
     *  Use of scoped thread eliminates need for calling join inside class destructor
     * Pass messages from websocket to external clients(producers)
     */
    boost::scoped_thread<> outputdata_thread{boost::thread{outputdata_worker,this}};

    // Start the ASIO io_service run loop
    try {
        m_endpoint.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}

void TelemetryServer::set_timer() {
    m_timer = m_endpoint.set_timer(
        1000,
        websocketpp::lib::bind(
            &TelemetryServer::on_timer,
            this,
            websocketpp::lib::placeholders::_1
        )
    );
}

/* Looks like a good callback to insert here module_package */
void TelemetryServer::on_timer(websocketpp::lib::error_code const & ec) {
    if (ec) {
        // there was an error, stop telemetry
        m_endpoint.get_alog().write(websocketpp::log::alevel::app,
                "Timer Error: "+ec.message());
        return;
    }
    static int count = 0;
    std::stringstream ss;
    ss << count++;
    std::string data = "test_inny" + ss.str();
    output_queue.wr(data);

    // set timer for next telemetry check*/
    set_timer();
}

void TelemetryServer::SendMsg(std::string& msg)
{
	output_queue.wr(msg);
}

void TelemetryServer::on_message(connection_hdl hdl, message_ptr msg){

    /* Put incoming message into corresponding module's queue for further processing */
	modules_.RouteMsg(msg->get_payload());

}
