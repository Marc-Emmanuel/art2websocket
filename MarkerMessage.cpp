#include "MarkerMessage.h"

using namespace std;

const float MarkerMessage::PRECISION_THRESHOLD_POSITION = 0.5;

MarkerMessage::MarkerMessage( int markerID, double quality, 
							  vector<double> markerLoc, double timestamp ) {
	this->markerID = markerID;
	this->quality = quality;
	this->markerLoc = markerLoc;
	this->timestamp = timestamp;
}

string MarkerMessage::toString() {
	ostringstream convert;
	std::ostringstream locString;
	locString << "{ \"x\": " << this->markerLoc[0] << ", \"y\": " << this->markerLoc[1] << ", \"z\": " << this->markerLoc[2] << " }";
	convert << "{ \"markerID\": " << this->markerID << ", \"quality\": "<< this->quality 
			<< ", \"location\": " << locString.str() << ", \"timestamp\": " << this->timestamp << " }";
	return convert.str();
}

string MarkerMessage::CSVHeaders() {
	return "MarkerID, quality, x, y, z, timestamp\n";
}

string MarkerMessage::toCSV() {
	ostringstream convert;
	std::ostringstream locString;
	locString << this->markerLoc[0] << ", " << this->markerLoc[1] << ", " << this->markerLoc[2];
	convert << this->markerID << ", " << this->quality << ", " 
			<< locString.str() << ", " << this->timestamp << "\n";
	return convert.str();
}

bool MarkerMessage::equals( MarkerMessage msg ) {
	if ( this->markerID == msg.markerID && this->quality == msg.quality && this->timestamp == msg.timestamp ) {
		// Compare rot and loc
		for ( int i = 0 ; i < this->markerLoc.size() ; i++ ) {
			if ( abs( this->markerLoc[i] - msg.markerLoc[i] ) > PRECISION_THRESHOLD_POSITION ) {
				return false;
			}
		}
		return true; 	
	}
	return false;
}
	