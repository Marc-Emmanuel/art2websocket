#include "ART2WebSocket.h"

#define DUMMY_MODE 0

typedef websocketpp::server<websocketpp::config::asio> server;

using namespace std;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using namespace websocketpp::frame::opcode;

// ART vars
static DTrackSDK* dt = NULL;

// Websocket server vars
queue<Message*> targetMessages;
bool TerminateARTDispatchThread = false;
websocketpp::connection_hdl connectionHandler;
server websocketServer;
thread websocketServerThread;
bool connectionOpened = false;	
BodyMessage lastBodyMessage = BodyMessage( 0, 0, vector<double>{}, vector<double>{}, 0 );
HandMessage lastHandMessage = HandMessage( 0, 1, 0, 5, vector<double>{}, vector<double>{}, vector<FingerMessage>{}, 0 );
MarkerMessage lastMarkerMessage = MarkerMessage( 0, 0, vector<double>{}, 0 );

/*
╔══════════════════════════════════════════════════════════════════════════════╗
║ ░ ART2WebSocket :: log ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░ ║
╟──────────────────────────────────────────────────────────────────────────────╢
│ Input  :  Message to print.                                                  │
│ Output :  Nothing                                                            │
╟──────────────────────────────────────────────────────────────────────────────╢
│ Print text to the console with current timestamp.                            │
╟──────────────────────────────────────────────────────────────────────────────╢
│ thibautjacob -> Mon Jan 19 00:57:12 CET 2015                                 │
└──────────────────────────────────────────────────────────────────────────────┘ */
static void log( string message ) {
	time_t now = time( 0 );
	tm *ltm = localtime( &now );
	cout << ltm->tm_mday << "/" << ltm->tm_mon + 1 << "/" << ltm->tm_year + 1900 
		 << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "\t"
		 << message << endl;
}

/*
╔════════════════════════════════════════════╗
║ ░ main :: logText ░░░░░░░░░░░░░░░░░░░░░░░░ ║
╟────────────────────────────────────────────╢
│ Log text on filesystem.                    │
└────────────────────────────────────────────┘ */
static void logText( string message, string file ) {
	time_t now = time( 0 );
	tm *ltm = localtime( &now );
	ofstream logFile;
	ostringstream time;
	time << ltm->tm_mday << "." << ( ltm->tm_mon + 1 ) << "." << ( ltm->tm_year + 1900 )
		 			   << "_" << ltm->tm_hour << "h" << ltm->tm_min << "m" << ltm->tm_sec;
	logFile.open( file + "_" + time.str() + ".csv" , ios::in | ios::app );
	if ( logFile.is_open()) {
		logFile << message;
		logFile.close();
	} else {
		log( file + "_" + time.str() + ".csv: file couldn't be opened" );
	}
}

/*
╔══════════════════════════════════════════════════════════════════════════════╗
║ ░ ART2WebSocket :: dispatchARTMessagesThroughWebsocket ░░░░░░░░░░░░░░░░░░░░░ ║
╟──────────────────────────────────────────────────────────────────────────────╢
│ Input  :  Pointer to websocket server, connection handler                    │
│ Output :  Nothing                                                            │
╟──────────────────────────────────────────────────────────────────────────────╢
│ Dispatch queued events through websocket.                                    │
╟──────────────────────────────────────────────────────────────────────────────╢
│ thibautjacob -> Mon Jan 19 00:54:33 CET 2015                                 │
└──────────────────────────────────────────────────────────────────────────────┘ */
// static void dispatchARTMessagesThroughWebsocket( server* s, websocketpp::connection_hdl hdl ) {
// 	cout << "dispatchARTMessagesThroughWebsocket" << endl;
// 	while ( true ) {
// 	while ( !targetMessages.empty()) {
// 		cout << "Sending to websocket: " << targetMessages.front().toString() << endl;
// 		// Take the next BodyMessage in the queue, generate JSON and send it over the websocket
// 		s->send( hdl, targetMessages.front().toString(), websocketpp::frame::opcode::TEXT );
// 		// Remove it from the queue
// 		targetMessages.pop();
// 	}}
// }

/*
╔══════════════════════════════════════════════════════════════════════════════╗
║ ░ ART2WebSocket :: onWebsocketMessage ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░ ║
╟──────────────────────────────────────────────────────────────────────────────╢
│ Input  :  Websocket server, connection handler, input websocket message.     │
│ Output :  Nothing                                                            │
╟──────────────────────────────────────────────────────────────────────────────╢
│ Callback function for the on_message event. If input message is getEvents,   │
│ dispatch queued events.                                                      │
╟──────────────────────────────────────────────────────────────────────────────╢
│ thibautjacob -> Mon Jan 19 01:02:57 CET 2015                                 │
└──────────────────────────────────────────────────────────────────────────────┘ */
static void onWebsocketMessage( server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
	string msg_content = msg->get_payload();
	// cout << "Message received: " << msg_content << endl;
	if ( msg_content.compare( "getEvents" ) == 0 ) {
		// Create new message
		// dispatchARTMessagesThroughWebsocket( s, hdl );
	} else if ( msg_content.compare( "stopEvents" ) == 0 ) {
		connectionOpened = false;
	}
}

static void onOpen( server* s, websocketpp::connection_hdl hdl ) {
	cout << "Opening connection" << endl;
	connectionHandler = hdl;
	connectionOpened = true;
}

/*
╔══════════════════════════════════════════════════════════════════════════════╗
║ ░ ART2WebSocket :: websocketServerSetup ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░ ║
╟──────────────────────────────────────────────────────────────────────────────╢
│ Input  :  Websocket port to run the server on.                               │
│ Output :  Nothing                                                            │
╟──────────────────────────────────────────────────────────────────────────────╢
│ Setup websocket server.                                                      │
╟──────────────────────────────────────────────────────────────────────────────╢
│ thibautjacob -> Mon Jan 19 01:04:30 CET 2015                                 │
└──────────────────────────────────────────────────────────────────────────────┘ */
static void websocketServerSetup( int port ) {
	websocketServer.set_message_handler( bind( &onWebsocketMessage,&websocketServer, ::_1, ::_2 ));
	websocketServer.set_open_handler( bind( &onOpen, &websocketServer, ::_1 ));
	websocketServer.init_asio();
	websocketServer.set_reuse_addr( true );
	websocketServer.listen( port );
	websocketServer.start_accept();
	cout << "Websocket server running" << endl;
	try {
		websocketServer.run();
	} catch ( ... ) {}
	
	cout << "Websocket server down" << endl;
}

/*
╔══════════════════════════════════════════════════════════════════════════════╗
║ ░ ART2WebSocket :: rotationMatrix2Euler ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░ ║
╟──────────────────────────────────────────────────────────────────────────────╢
│ Input  :  DTrack_Body_Type_d body object.                                    │
│ Output :  Vector<double> containing Euler angles in radiants corresponding   │
│           to the body rotation matrix.                                       │
╟──────────────────────────────────────────────────────────────────────────────╢
│ Converts ART body rotation matrix to Euler angles in radiants.               │
╟──────────────────────────────────────────────────────────────────────────────╢
│ thibautjacob -> Mon Jan 19 01:07:01 CET 2015                                 │
└──────────────────────────────────────────────────────────────────────────────┘ */
static vector<double> rotationMatrix2Euler( double rot[] ) {
	vector<double> angles;
	double eulerX, eulerY, eulerZ;
	assert( fabs( rot[2]) <= 1 );
	eulerY = -asin( -rot[6]);
	if( fabs( cos( eulerY )) > std::numeric_limits<double>::epsilon()) { // cos(y) != 0 Gimbal-Lock
		eulerX = -atan2( rot[7], rot[8]);
		eulerZ = -atan2( rot[3], rot[0]);
	} else {
		eulerZ = 0;
		eulerX = -atan2( rot[1], rot[4]);
	}
	angles.push_back( eulerX );
	angles.push_back( eulerY );
	angles.push_back( eulerZ );
	return angles;
}

/*
╔══════════════════════════════════════════════════════════════════════════════╗
║ ░ ART2WebSocket :: acquireData ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░ ║
╟──────────────────────────────────────────────────────────────────────────────╢
│ Input  :  Nothing.                                                           │
│ Output :  Nothing.                                                           │
╟──────────────────────────────────────────────────────────────────────────────╢
│ Gather latest ART data and store them in the event queue for dispatch. Run   │
│ angle conversion to radiants.                                                │
╟──────────────────────────────────────────────────────────────────────────────╢
│ thibautjacob -> Mon Jan 19 01:09:53 CET 2015                                 │
└──────────────────────────────────────────────────────────────────────────────┘ */
static void acquireData() {
	cout.precision( 3 );
	cout.setf( ios::fixed, ios::floatfield );
	// Tree target
	DTrack_Body_Type_d body;
	for ( int i = 0 ; i < dt->getNumBody() ; i++ ) {
		body = *dt->getBody( i );
		if ( body.quality > 0 ) {
			vector<double> angles = rotationMatrix2Euler( body.rot );
			// Place ART message on the queue for websocket transmission
			vector<double> locations( body.loc, body.loc + sizeof body.loc / sizeof body.loc[0]);
			BodyMessage* message = new BodyMessage( body.id, body.quality, locations, angles, dt->getTimeStamp());
			// We don't take duplicate messages
			if ( !message->equals( lastBodyMessage )) {
				lastBodyMessage = *message;
				targetMessages.push( message );
			}
		}
	}
	
	// Marker tracking
	DTrack_Marker_Type_d marker;
	for ( int i = 0 ; i < dt->getNumMarker() ; i++ ) {
		marker = *dt->getMarker( i );
		if ( marker.quality > 0 ) {
			vector<double> markerLocation( marker.loc, marker.loc + sizeof marker.loc / sizeof marker.loc[0]);
			MarkerMessage* msg = new MarkerMessage( marker.id, marker.quality, markerLocation, dt->getTimeStamp());
			// We don't take duplicate messages
			if ( !msg->equals( lastMarkerMessage )) {
				lastMarkerMessage = *msg;
				targetMessages.push( msg );
			}
		}
	}
	
	// Finger tracking
	DTrack_Hand_Type_d hand;
	for ( int i = 0; i < dt->getNumHand() ; i++ ) {
		hand = *dt->getHand( i );
		if ( hand.quality > 0 ) {
			vector<double> angles = rotationMatrix2Euler( hand.rot );
			// Place ART message on the queue for websocket transmission
			vector<double> locations( hand.loc, hand.loc + sizeof hand.loc / sizeof hand.loc[0]);
			// Create vector of FingerMessage
			vector<FingerMessage> fingers;
			for ( int j = 0 ; j < hand.nfinger ; j++ ) {
				vector<double> fingerLocations( hand.finger[j].loc, hand.finger[j].loc + 
												sizeof hand.finger[j].loc / sizeof hand.finger[j].loc[0]);
				vector<double> fingerAngles = rotationMatrix2Euler( hand.finger[j].rot );
				vector<double> fingerLengthPhalanx( hand.finger[j].lengthphalanx, hand.finger[j].lengthphalanx + 
													sizeof hand.finger[j].lengthphalanx / sizeof hand.finger[j].lengthphalanx[0]);
				vector<double> fingerAnglePhalanx( hand.finger[j].anglephalanx, hand.finger[j].anglephalanx + 
												   sizeof hand.finger[j].anglephalanx / sizeof hand.finger[j].anglephalanx[0]);
				FingerMessage finger = FingerMessage( j, fingerLocations, fingerAngles, hand.finger[j].radiustip, 
													  fingerLengthPhalanx, fingerAnglePhalanx, dt->getTimeStamp());
				fingers.push_back( finger );
				string text = finger.toCSV();
				logText( text, "/Users/thibautjacob/Documents/Projects/ART2WebSocket/log" );
				// cout << " fi "  << j
// 					 << " loc " << hand.finger[j].loc[0] << " " << hand.finger[j].loc[1] << " " << hand.finger[j].loc[2]
// 					 << " rot " << hand.finger[j].rot[0] << " " << hand.finger[j].rot[1] << " " << hand.finger[j].rot[2]
// 					 << " "     << hand.finger[j].rot[3] << " " << hand.finger[j].rot[4] << " " << hand.finger[j].rot[5]
// 					 << " "     << hand.finger[j].rot[6] << " " << hand.finger[j].rot[7] << " " << hand.finger[j].rot[8] << endl;
// 				cout << " fi "  << j
// 					 << " tip " << hand.finger[j].radiustip
// 					 << " pha " << hand.finger[j].lengthphalanx[0] << " " << hand.finger[j].lengthphalanx[1]
// 					 << " "     << hand.finger[j].lengthphalanx[2]
// 					 << " ang " << hand.finger[j].anglephalanx[0]  << " " << hand.finger[j].anglephalanx[1] << endl;
			}
			HandMessage* message = new HandMessage( hand.id, hand.quality, hand.lr, hand.nfinger, locations, angles, fingers, dt->getTimeStamp());
			// We don't take duplicate messages
			if ( !message->equals( lastHandMessage )) {
				lastHandMessage = *message;
				targetMessages.push( message );
			}
			// cout << "hand " << hand.id << " qu " << hand.quality
// 				 << " lr "  << (( hand.lr == 0 ) ? "left" : "right") << " nf " << hand.nfinger
// 				 << " loc " << hand.loc[0] << " " << hand.loc[1] << " " << hand.loc[2]
// 				 << " rot " << hand.rot[0] << " " << hand.rot[1] << " " << hand.rot[2]
// 				 << " "     << hand.rot[3] << " " << hand.rot[4] << " " << hand.rot[5]
// 				 << " "     << hand.rot[6] << " " << hand.rot[7] << " " << hand.rot[8] << endl;
		}
	}
}

/*
╔══════════════════════════════════════════════════════════════════════════════╗
║ ░ ART2WebSocket :: data_error_to_console ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░ ║
╟──────────────────────────────────────────────────────────────────────────────╢
│ Input  :  Nothing.                                                           │
│ Output :  Boolean.                                                           │
╟──────────────────────────────────────────────────────────────────────────────╢
│ Check for various errorS and print them to the console. Returns false in     │
│ case of error.                                                               │
╟──────────────────────────────────────────────────────────────────────────────╢
│ thibautjacob -> Mon Jan 19 01:25:57 CET 2015                                 │
└──────────────────────────────────────────────────────────────────────────────┘ */
static bool data_error_to_console() {
	if ( dt->getLastDataError() == DTrackSDK::ERR_TIMEOUT ) {
		log( "--- timeout while waiting for tracking data" );
		return false;
	}

	if ( dt->getLastDataError() == DTrackSDK::ERR_NET ) {
		log( "--- error while receiving tracking data" );
		return false;
	}

	if ( dt->getLastDataError() == DTrackSDK::ERR_PARSE ) {
		log( "--- error while parsing tracking data" );
		return false;
	}
	return true;
}

/*
╔══════════════════════════════════════════════════════════════════════════════╗
║ ░ ART2WebSocket :: messages_to_console ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░ ║
╟──────────────────────────────────────────────────────────────────────────────╢
│ Input  :  Nothing.                                                           │
│ Output :  Nothing.                                                           │
╟──────────────────────────────────────────────────────────────────────────────╢
│ Print ATC messages to the console.                                           │
╟──────────────────────────────────────────────────────────────────────────────╢
│ thibautjacob -> Mon Jan 19 01:26:55 CET 2015                                 │
└──────────────────────────────────────────────────────────────────────────────┘ */
static void messages_to_console() {
	while ( dt->getMessage()) {
		cout << "ATC message: \"" << dt->getMessageStatus() << "\" \"" << dt->getMessageMsg() << "\"" << endl;
	}
}

/*
╔════════════════════════════════════════════╗
║ ░ ART2WebSocket :: main ░░░░░░░░░░░░░░░░░░ ║
╟────────────────────────────────────────────╢
│ Main loop and initialization.              │
└────────────────────────────────────────────┘ */
int main( int argc, char** argv ) {
	if (argc != 5 ) {
		cout << "Usage: " << argv[0] << " <server host> <server port> <data port> <websocket port>" << endl;
		return -1;
	}
	int port = 0, rport = 0, websocketPort = 0;

	istringstream portstream( argv[2]);
	portstream >> rport;  // remote port
	if ( portstream.fail() || rport <= 0 || rport >= 65536 ) {
		log( "invalid remote port '" + (string)argv[3] + "'" );
		return -2;
	}

	portstream.clear();
	portstream.str( argv[3]);
	portstream >> port;  // data port
	if ( portstream.fail() || port <= 0 || port >= 65536 ) {
		log( "invalid remote port '" + (string)argv[2] + "'" );
		return -2;
	}
	
	websocketPort = atoi(argv[4]);
	if ( websocketPort <= 0 || websocketPort >= 65536 ) {
		log( "invalid websocket port '" + (string)argv[4] + "'" );
		return -2;
	}

	// Initialize websocket server into a thread
	thread websocketServerThread( websocketServerSetup, websocketPort );
	logText( FingerMessage::CSVHeaders(), "/Users/thibautjacob/Documents/Projects/ART2WebSocket/log" );

	// init library:
	dt = new DTrackSDK(( const char * ) argv[1], rport, port );

	if ( !dt->isLocalDataPortValid()) {
		log( "DTrackSDK init error" );
		return -3;
	}

	log( "connected to ATC '" + (string)argv[1] + "' (local data port " + to_string( dt->getDataPort()) + ")" );

	// start measurement:
	if ( !dt->startMeasurement()) {
		log( "Start measurement failed!" );
		messages_to_console();
		delete dt;
		return -4;
	}
	
	// receiving:
#if DUMMY_MODE == 1
	double timestamp = 0;
#endif
	while ( true ) {
#if DUMMY_MODE == 0
		if ( dt->receive()) {
			acquireData();
#else
		if ( true ) {
			usleep( 50000 );
#endif
			if ( connectionOpened ) {
				// cout << "Dispatching data" << endl;
#if DUMMY_MODE == 1
				// Dummy random generated body messages
				// targetMessages.push( new BodyMessage( rand() % 10, 1, vector<double>{ (double)(rand() % 2000), (double)(rand() % 2000), (double)(rand() % 2000) }, vector<double>{ 23.0, 50.0, 34.0 }, timestamp++ ));
				// Dummy random generated hand messages
				vector<double> rot = { 23.0, 50.0, 34.0 };
				int nbFingers = rand() % 5 + 1;
				vector<FingerMessage> fingers;
				for ( int fingerIndex = 0 ; fingerIndex < 5 ; fingerIndex++ ) {
					fingers.push_back( FingerMessage( rand() % 10, vector<double>{ (double)(rand() % 1000), (double)(rand() % 1000), (double)(rand() % 1000) }, vector<double>{ (double)(rand() % 1000), (double)(rand() % 1000), (double)(rand() % 1000) }, rand() % 10, vector<double>{ (double)(rand() % 100), (double)(rand() % 100), (double)(rand() % 100) }, vector<double>{ (double)(rand() % 100), (double)(rand() % 100) }, timestamp ));
				}
				 
				// targetMessages.push( new HandMessage( rand() % 10, 1, rand() % 2, nbFingers,  vector<double>{ (double)(rand() % 1000), (double)(rand() % 1000), (double)(rand() % 1000) }, vector<double>{ (double)(rand() % 100), (double)(rand() % 100), (double)(rand() % 100) }, fingers, timestamp++ ));
				// Dummy random generated marker messages
				targetMessages.push( new MarkerMessage( rand() % 10, (double)(rand() % 1), vector<double>{ (double)(rand() % 1000), (double)(rand() % 1000), (double)(rand() % 1000) }, timestamp ));
#endif
				while ( !targetMessages.empty()) {
					// cout << "Sending to websocket: " << targetMessages.front()->toString() << endl;
					// Take the next BodyMessage in the queue, generate JSON and send it over the websocket
					try {
						websocketServer.send( connectionHandler, targetMessages.front()->toString(), websocketpp::frame::opcode::TEXT );
					} catch ( ... ) {
						cout << "Couldn't send to the socket" << endl;
					};
					
					// Remove it from the queue
					targetMessages.pop();
				}
			}
		} else {
			data_error_to_console();
		}
		messages_to_console();
	}
	log( "Shutting down" );

	// stop measurement & clean up:
	dt->stopMeasurement();
	messages_to_console();
	delete dt;
	return 0;
}
