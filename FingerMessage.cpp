#include "FingerMessage.h"

using namespace std;

const float FingerMessage::PRECISION_THRESHOLD_POSITION = 0.5;
const float FingerMessage::PRECISION_THRESHOLD_ROTATION = 0.5;
const float FingerMessage::PRECISION_THRESHOLD_PHALANX_LENGTH = 0.5;
const float FingerMessage::PRECISION_THRESHOLD_PHALANX_ANGLE = 0.5;

FingerMessage::FingerMessage( int fingerID, vector<double> fingerLoc, 
							  vector<double> fingerRot, double fingerRadiusTip, 
							  vector<double> lengthPhalanx, vector<double> anglePhalanx, 
							  double timestamp ) {
	this->fingerID = fingerID;
	this->fingerLoc = fingerLoc;
	this->fingerRot = fingerRot;
	this->fingerRadiusTip = fingerRadiusTip;
	this->lengthPhalanx = lengthPhalanx;
	this->anglePhalanx = anglePhalanx;
	this->timestamp = timestamp;
}

string FingerMessage::toString() {
	ostringstream convert;
	std::ostringstream locString;
	std::ostringstream rotString;
	std::ostringstream lengthPhalanxString;
	std::ostringstream anglePhalanxString;
	locString << "x: " << this->fingerLoc[0] << ", y: " << this->fingerLoc[1] << ", z: " << this->fingerLoc[2];
	rotString << "rotX: " << this->fingerRot[0] << ", rotY: " << this->fingerRot[1] << ", rotZ: " << this->fingerRot[2];
	lengthPhalanxString << "phalanxLengthX: " << this->lengthPhalanx[0] << ", phalanxLengthY: " << this->lengthPhalanx[1] 
						<< ", phalanxLengthZ: " << this->lengthPhalanx[2];
	anglePhalanxString << "phalanxAngle1: " << this->anglePhalanx[0] << ", phalanxAngle2: " << this->anglePhalanx[1];
	convert << "[ fingerID: " << this->fingerID << ", " << locString.str() << ", " << rotString.str() << ", fingerRadiusTip: " 
			<< this->fingerRadiusTip << ", " << lengthPhalanxString.str() << ", "
			<< anglePhalanxString.str() << ", timestamp: " << this->timestamp << " ]";
	return convert.str();
}

string FingerMessage::CSVHeaders() {
	return "fingerID, x, y, z, rotX, rotY, rotZ, fingerRadiusTip, phalanxLengthX, phalanxLengthY, phalanxLengthZ, phalanxAngle1, phalanxAngle2, timestamp\n";
}

string FingerMessage::toCSV() {
	ostringstream convert;
	std::ostringstream locString;
	std::ostringstream rotString;
	std::ostringstream lengthPhalanxString;
	std::ostringstream anglePhalanxString;
	locString << this->fingerLoc[0] << ", " << this->fingerLoc[1] << ", " << this->fingerLoc[2];
	rotString << this->fingerRot[0] << ", " << this->fingerRot[1] << ", " << this->fingerRot[2];
	lengthPhalanxString << this->lengthPhalanx[0] << ", " << this->lengthPhalanx[1] << ", " << this->lengthPhalanx[2];
	anglePhalanxString << this->anglePhalanx[0] << ", " << this->anglePhalanx[1];
	convert << this->fingerID << ", " << locString.str() << ", " << rotString.str() << ", " 
			<< this->fingerRadiusTip << ", " << lengthPhalanxString.str() << ", "
			<< anglePhalanxString.str() << ", " << this->timestamp << "\n";
	return convert.str();
}

bool FingerMessage::equals( FingerMessage msg ) {
	if ( this->fingerID == msg.fingerID && this->fingerRadiusTip == msg.fingerRadiusTip && this->timestamp == msg.timestamp ) {
		// Compare rot and loc
		for ( int i = 0 ; i < this->fingerLoc.size() ; i++ ) {
			if ( abs( this->fingerLoc[i] - msg.fingerLoc[i] ) > PRECISION_THRESHOLD_POSITION || 
				 abs( this->fingerRot[i] - msg.fingerRot[i] ) > PRECISION_THRESHOLD_ROTATION ) {
				return false;
			}
		}
		// Compare phalanx length
		for ( int i = 0 ; i < this->lengthPhalanx.size() ; i++ ) {
			if ( abs( this->lengthPhalanx[i] - msg.lengthPhalanx[i] ) > PRECISION_THRESHOLD_PHALANX_LENGTH ) {
				return false;
			}
		}
		// Compare phalanx angle
		for ( int i = 0 ; i < this->anglePhalanx.size() ; i++ ) {
			if ( abs( this->anglePhalanx[i] - msg.anglePhalanx[i] ) > PRECISION_THRESHOLD_PHALANX_ANGLE ) {
				return false;
			}
		}
		return true; 	
	}
	return false;
}
	