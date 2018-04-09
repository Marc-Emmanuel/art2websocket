#include <vector>
#include <ostream>
#include <sstream>
#include <string>
#include "Lib/DTrackSDK.hpp"
#include <cmath>
#include "Message.h"

using namespace std;

class FingerMessage: public Message {
	public:
		static const float PRECISION_THRESHOLD_POSITION;
		static const float PRECISION_THRESHOLD_ROTATION;
		static const float PRECISION_THRESHOLD_PHALANX_LENGTH;
		static const float PRECISION_THRESHOLD_PHALANX_ANGLE;
		FingerMessage( int fingerID, vector<double> fingerLoc, vector<double> fingerRot, double fingerRadiusTip, vector<double> lengthPhalanx, vector<double> anglePhalanx, double timestamp );
		virtual string toString();
		string toCSV();
		static string CSVHeaders();
		bool equals( FingerMessage msg );
	private:
		int fingerID;
		vector<double> fingerLoc;
		vector<double> fingerRot;
		double fingerRadiusTip;
		vector<double> lengthPhalanx;
		vector<double> anglePhalanx;
		double timestamp;
};