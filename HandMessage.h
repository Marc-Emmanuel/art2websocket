#include <vector>
#include <ostream>
#include <sstream>
#include <string>
#include "Lib/DTrackSDK.hpp"
#include <cmath>
#include "Message.h"
#include "FingerMessage.h"

using namespace std;

class HandMessage: public Message {
	public:
		static const float PRECISION_THRESHOLD_POSITION;
		static const float PRECISION_THRESHOLD_ROTATION;
		HandMessage( int handID, double handQuality, int handLR, int handNbFinger, vector<double> handLoc, vector<double> handRot, vector<FingerMessage> fingers, double timestamp );
		virtual string toString(); 
		bool equals( HandMessage msg );
	private:
		int handID;
		double handQuality;
		int handLR;
		int handNbFinger;
		vector<double> handLoc;
		vector<double> handRot;
		vector<FingerMessage> fingers;
		double timestamp;
};