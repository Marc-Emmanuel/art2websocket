#include "Lib/DTrackSDK.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include <assert.h>
#include <thread>
#include <chrono>
#include <queue>
#include <vector>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "Message.h"
#include "BodyMessage.h"
#include "HandMessage.h"
#include "MarkerMessage.h"
#include <cstdlib>
#include <unistd.h>


static void log( std::string message );
// static void logText( std::string message, std::string file );