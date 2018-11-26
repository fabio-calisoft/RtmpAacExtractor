/*
 * RtmpWrapper.h
 *
 *  Created on: Jul 19, 2015
 *      Author: fabio
 */

#ifndef RTMPWRAPPER_H_
#define RTMPWRAPPER_H_

#include <iostream>
#include <fstream>
#include <list>
#include "../librtmp/rtmp.h"
#include "../librtmp/log.h"

#define BYTE	unsigned char
#define RTMP_DATA_SIZE		1024

#define RTMP_OK_CONNECT                  0
#define RTMP_ERROR_SERVER_CONNECT       -1
#define RTMP_ERROR_STREAM_CONNECT       -2

namespace std {

class RtmpWrapper {
public:
	RtmpWrapper();
	virtual ~RtmpWrapper();

	void setUri(string uri);
	void connect();
	int readData();
	void debugSaveFlvToFile(string filename);
	void debugReadFromFlv(string filename);
	void setDataList(std::list<BYTE> *pList);


private:
	string rtmp_url;
	RTMP * rtmp;
	ifstream inDebugFlvFile;
	ofstream outDebugFlvFile;
	bool isDebugSaveFlvToFile;
	bool isReadFlvToFile;
	std::list<BYTE> *rtmpDataList;
};

} /* namespace std */

#endif /* RTMPWRAPPER_H_ */
