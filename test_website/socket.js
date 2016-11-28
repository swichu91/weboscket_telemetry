
socketOpDataReq = "0";
socketOpDataCyclic = "1";
socketOpStop = "2";
socketOpModSpecific = "66";


var socket =(function () {
	
	var ws;
	
	return {
		
		/** Attempts to connect to webserver specified by URL */
		connect: function (url) {

			if ("WebSocket" in window) {
				ws = new WebSocket(url);
			} else if ("MozWebSocket" in window) {
				ws = new MozWebSocket(url);
			} else {
				alert("This Browser does not support WebSockets<br />");
				return;
			}
		},
		
		/** Disconnects from webserver */
		disconnect: function(){			
			ws.close();			
		},
		
		/** Register OnOpen handler */
		registerOnOpen : function(handler){
			ws.onopen = handler;	
		},
		
		/** Register OnError handler */
		registerOnError:	function (handler){		
			ws.onerror = handler;
		},
		
		/** Register OnClose handler */
		registerOnClose: 	function (handler){		
			ws.onclose = handler;
		},
		
		/** Register OnMsg handler */
		registerOnMsg: 	function (handler){		
			ws.onmessage = handler;
		},
		
		/** Send message to webserver
		 * Params: module name,
		 * cmd,
		 *  socketOpDataReq = "0";
			socketOpDataCyclic = "1";
			socketOpStop = "2";
			socketOpModSpecific = "66"; 
		 * val
		 */
		emit: 	function (mod_name,cmd,val){		
			ws.send(mod_name + ":" + cmd + "=" + val + ",");		
		},
		
		/** Returns connection url */
		url: function(){
			return ws.url;
		}
			
	}

}());
