/*
 * RtmpDataChunk.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: fabio
 */
#include "RtmpDataChunk.h"
using namespace std;

RtmpDataChunk::RtmpDataChunk() {
	payload = 0;
	size = 0;
}

void RtmpDataChunk::dump() {
	for (int i=0; i < size; i++) {
		cout << " 0x" << hex << (int)payload[i];
	}
}

RtmpDataChunk::~RtmpDataChunk() {
//	if (payload != 0)
//		delete (payload);
}


