#include "HandMessage.h"

using namespace std;

const float HandMessage::PRECISION_THRESHOLD_POSITION = 0.5;
const float HandMessage::PRECISION_THRESHOLD_ROTATION = 0.5;

HandMessage::HandMessage( int handID, double handQuality, int handLR, 
						  int handNbFinger, vector<double> handLoc, 
						  vector<double> handRot, vector<FingerMessage> fingers, double timestamp ) {
	this->handID = handID;
	this->handQuality = handQuality;
	this->handLR = handLR;
	this->handNbFinger = handNbFinger;
	this->handLoc = handLoc;
	this->handRot = handRot;
	this->fingers = fingers;
	this->timestamp = timestamp;
}

string HandMessage::toString() {
	ostringstream convert;
	std::ostringstream locString;
	std::ostringstream rotString;
	std::ostringstream fingersString;
	locString << "x: " << this->handLoc[0] << ", y: " << this->handLoc[1] << ", z: " << this->handLoc[2];
	rotString << "rotX: " << this->handRot[0] << ", rotY: " << this->handRot[1] << ", rotZ: " << this->handRot[2];
	for ( int fingerIndex = 0 ; fingerIndex < this->fingers.size() ; fingerIndex++ ) {
		fingersString << this->fingers[fingerIndex].toString();
	}
	convert << "[ HandID: " << this->handID << ", quality: " << this->handQuality << ", handLR: " << this->handLR 
			<< ", NbFinger: " << this->handNbFinger << ", " << locString.str() << ", " << rotString.str() << ", fingers: "
			<< fingersString.str() << ", timestamp: " << this->timestamp << " ]";
	return convert.str();
}

bool HandMessage::equals( HandMessage msg ) {
	if ( this->handID == msg.handID && this->handQuality == msg.handQuality && 
		 this->timestamp == msg.timestamp && this->handLR == msg.handLR && 
		 this->handNbFinger == msg.handNbFinger ) {
		// Compare loc and rot
		for ( int i = 0 ; i < this->handLoc.size() ; i++ ) {
			if ( abs( this->handLoc[i] - msg.handLoc[i] ) > PRECISION_THRESHOLD_POSITION || 
				 abs( this->handRot[i] - msg.handRot[i] ) > PRECISION_THRESHOLD_ROTATION ) {
				return false;
			}
		}
		// Compare fingers
		// If the number of fingers is different, then return false
		if ( this->fingers.size() != msg.fingers.size()) {
			return false;
		}
		for ( int i = 0 ; i < this->fingers.size() ; i++ ) {
			if ( !this->fingers[i].equals( msg.fingers[i])) {
				return false;
			}
		}
		return true; 	
	}
	return false;
}
	