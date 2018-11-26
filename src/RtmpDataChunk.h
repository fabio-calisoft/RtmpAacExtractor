/*
 * RtmpDataChunk.h
 *
 *  Created on: Jul 9, 2015
 *      Author: fabio
 */

#ifndef RTMPDATACHUNK_H_
#define RTMPDATACHUNK_H_

#include <iostream>
using namespace std;


class RtmpDataChunk {
	unsigned char *payload;
	int size;
public:
	RtmpDataChunk();
	virtual ~RtmpDataChunk();

	unsigned char * getPayload() const {
		return payload;
	}

	void setPayload(unsigned char *payload) {
		this->payload = payload;
	}

	unsigned int getSize() const {
		return size;
	}

	void setSize(unsigned int size) {
		this->size = size;
	}

	void dump();

};

#endif /* RTMPDATACHUNK_H_ */
