#include "BodyMessage.h"

using namespace std;

const float BodyMessage::PRECISION_THRESHOLD_POSITION = 0.5;
const float BodyMessage::PRECISION_THRESHOLD_ROTATION = 0.5;

BodyMessage::BodyMessage( int id, double quality, vector<double> loc, vector<double> rot, double timestamp ) {
	this->bodyID = id;
	this->quality = quality;
	this->loc = loc;
	this->rot = rot;
	this->timestamp = timestamp;
}

string BodyMessage::toString() {
	ostringstream convert;
	std::ostringstream locString;
	std::ostringstream rotString;
	locString << "x: " << this->loc[0] << ", y: " << this->loc[1] << ", z: " << this->loc[2];
	rotString << "rotX: " << this->rot[0] << ", rotY: " << this->rot[1] << ", rotZ: " << this->rot[2];
	convert << "[ bodyID: " << this->bodyID << ", quality: " << this->quality << ", " << locString.str() << ", " 
			<< rotString.str() << ", timestamp: " << this->timestamp << " ]";
	return convert.str();
}

bool BodyMessage::equals( BodyMessage msg ) {
	if ( this->bodyID == msg.bodyID && this->quality == msg.quality && this->timestamp == msg.timestamp ) {
		for ( int i = 0 ; i < this->loc.size() ; i++ ) {
			if ( abs( this->loc[i] - msg.loc[i] ) > PRECISION_THRESHOLD_POSITION || 
				 abs( this->rot[i] - msg.rot[i] ) > PRECISION_THRESHOLD_ROTATION ) {
				return false;
			}
		}
		return true; 	
	}
	return false;
}
	