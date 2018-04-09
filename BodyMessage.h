#include <vector>
#include <ostream>
#include <sstream>
#include <string>
#include "Lib/DTrackSDK.hpp"
#include <cmath>
#include "Message.h"

using namespace std;

class BodyMessage: public Message {
	public:
		static const float PRECISION_THRESHOLD_POSITION;
		static const float PRECISION_THRESHOLD_ROTATION;
		BodyMessage( int id, double quality, vector<double> 
					 loc, vector<double> rot, double timestamp );
		virtual string toString();
		bool equals( BodyMessage msg );
	private:
		int bodyID;
		double quality;
		vector<double> loc;
		vector<double> rot;
		double timestamp;
};