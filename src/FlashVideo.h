/*
 * FlashVideo.h
 *
 *  Created on: Jul 19, 2015
 *      Author: fabio
 */

#ifndef FLASHVIDEO_H_
#define FLASHVIDEO_H_

#define	AAC_DEST_FILE	"/tmp/fabioAac.aac"
#define BYTE	unsigned char
#define	NOT_ENOUGH_DATA	1
#define uint    unsigned int

#include <list>
#include <math.h>
#include <iostream>
#include <fstream>


namespace std {

class FlashVideo {
public:
	FlashVideo();
	virtual ~FlashVideo();
	void parseBuffer(std::list<BYTE> &pDataList);
	bool findHeader(std::list<BYTE> &pDataList);

private:
	void make_AAC_ADTS_Header(std::list<BYTE>::iterator &it);
	void set_AAC_ADTS_Header(std::list<BYTE>::iterator &it, uint encoded_length);

	// FLV header
	bool headerReceived;
    uint prev_size;
    uint packet_type;
    uint payload_size;
    uint timestamp;
    uint stream_id;
    BYTE aac_ADTS_header[7];
    fstream aacFile;

};

} /* namespace std */

#endif /* FLASHVIDEO_H_ */
