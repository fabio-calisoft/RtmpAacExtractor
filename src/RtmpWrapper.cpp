/*
 * RtmpWrapper.cpp
 *
 *  Created on: Jul 19, 2015
 *      Author: fabio
 */

#include "RtmpWrapper.h"

namespace std {

RtmpWrapper::RtmpWrapper() :
		isDebugSaveFlvToFile(false), isReadFlvToFile(false) {
	rtmp = RTMP_Alloc();
	if (!rtmp) {
		cout << "ERROR: Unable to create rtmp object";
		throw;
	}
	RTMP_LogSetLevel(RTMP_LOGWARNING);
	//RTMP_LogSetOutput(fp);
	//RTMP_LogSetCallback(rtmp_log);
	// RTMP_LOGCRIT RTMP_LOGERROR RTMP_LOGWARNING RTMP_LOGINFO RTMP_LOGDEBUG RTMP_LOGDEBUG2

	RTMP_Init(rtmp);
}

RtmpWrapper::~RtmpWrapper() {
	RTMP_Free(rtmp);
	if (isDebugSaveFlvToFile) {
		outDebugFlvFile.close();
	}
	if (isReadFlvToFile) {
		inDebugFlvFile.close();
	}
}

void RtmpWrapper::setUri(string uri) {
	this->rtmp_url = uri;
}

void RtmpWrapper::setDataList(std::list<BYTE> *pList) {
	rtmpDataList = pList;
}

void RtmpWrapper::connect() {
	// convert the const char* string to char*
	char * writable_rtmp_url = new char[rtmp_url.size() + 1];
	std::copy(rtmp_url.begin(), rtmp_url.end(), writable_rtmp_url);
	writable_rtmp_url[rtmp_url.size()] = '\0';
	RTMP_SetupURL(rtmp, writable_rtmp_url);
	cout << "connect to server:" << writable_rtmp_url;
	if (!RTMP_Connect(rtmp, NULL)) {
		cout << "Unable to connect to server";
		delete writable_rtmp_url;
		throw RTMP_ERROR_SERVER_CONNECT;
	}
	cout << "connect to stream";
	if (!RTMP_ConnectStream(rtmp, 0)) {
		cout << "Unable to connect to stream";
		delete writable_rtmp_url;
		throw RTMP_ERROR_STREAM_CONNECT;
	}
	delete writable_rtmp_url;
}

int RtmpWrapper::readData() {
	int data_size;
	char *buf = new char[RTMP_DATA_SIZE];
	if (isReadFlvToFile) {
		cout << "Debug :: read from file";
		data_size = 100;  //TODO 100 should be random
		char mBuf;
		for (int i = 0; i < data_size; i++) {
			inDebugFlvFile.read(&mBuf, 1);
			rtmpDataList->push_back(mBuf);
//			cout <<"0x" << hex << (int)(unsigned char)mBuf << ".";
		}
//		inDebugFlvFile.read(buf, data_size);
	} else {
		data_size = RTMP_Read(rtmp, (char*) buf, RTMP_DATA_SIZE);
	}
	cout << "readData - read " << data_size << " bytes" << endl;
	if (isDebugSaveFlvToFile) {
		cout << "readData - debugFlvFileName write data into file" << endl;
		outDebugFlvFile.write((const char *) buf, data_size);
	}

	// add the data to buffer list
	for (int i = 0; i < data_size; i++) {
		rtmpDataList->push_back( *(buf+i) );
//			cout <<"0x" << hex << (int)(unsigned char)mBuf << ".";
	}
	cout << "rtmpDataList has now " << rtmpDataList->size() << " elements" << endl;

	return data_size;
}

void RtmpWrapper::debugSaveFlvToFile(string filename) {
	if (filename.length() > 0) {
		isDebugSaveFlvToFile = true;
		outDebugFlvFile.open(filename.c_str(), ios::out | ios::binary);
	}
}

void RtmpWrapper::debugReadFromFlv(string filename) {
	if (filename.length() > 0) {
		isReadFlvToFile = true;
		inDebugFlvFile.open(filename.c_str(), ios::in | ios::binary);
	}
}

} /* namespace std */
