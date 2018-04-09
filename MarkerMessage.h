#include <vector>
#include <ostream>
#include <sstream>
#include <string>
#include "Lib/DTrackSDK.hpp"
#include <cmath>
#include "Message.h"

using namespace std;

class MarkerMessage: public Message {
	public:
		static const float PRECISION_THRESHOLD_POSITION;
		MarkerMessage( int markerID, double quality, vector<double> markerLoc, double timestamp );
		virtual string toString();
		string toCSV();
		static string CSVHeaders();
		bool equals( MarkerMessage msg );
	private:
		int markerID;
		double quality;
		vector<double> markerLoc;
		double timestamp;
};