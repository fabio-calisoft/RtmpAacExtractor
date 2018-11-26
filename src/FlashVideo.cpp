/*
 * FlashVideo.cpp
 *
 *  Created on: Jul 19, 2015
 *      Author: fabio
 */

#include "FlashVideo.h"
#include <stdio.h>
#include <iostream>

namespace std {

FlashVideo::FlashVideo() {
	aacFile.open(AAC_DEST_FILE, ios::binary | ios::out);
}

FlashVideo::~FlashVideo() {
	aacFile.close();
}

uint getx(std::list<BYTE>::iterator &it, int index) {
	uint res = 0;
	int i = 0;
	for (; i < index; i++) {
		unsigned char x = *(it++);
		uint ex = pow(256, (index - 1 - i));
		res += x * ex;
	}
	return res;
}

uint get8(std::list<BYTE>::iterator &it) {
	return getx(it, 1);
}

uint get24(std::list<BYTE>::iterator &it) {
	return getx(it, 3);
}

uint get32(std::list<BYTE>::iterator &it) {
	return getx(it, 4);
}

void dumpPacketInfo(uint prev_size, uint packet_type, uint payload_size,
uint timestamp, uint stream_id) {
	cout << endl << dec << "++++++++packet++++++++++++++++" << endl;
	cout << "prev_size=" << prev_size << endl;
	cout << "packet_type=" << packet_type;
	if (packet_type == 0x12)
		cout << " (AMF Metadata)";
	cout << endl;
	cout << "payload_size=" << payload_size << endl;
	cout << "timestamp=" << timestamp << endl;
	cout << "stream_id=" << stream_id << endl;
	cout << "--------packet----------------" << endl;
}

/**
 * makes the ATDS header based on the AudioSpecificConfig defined in ISO/IEC 14496-3:2005
 *
 */
void FlashVideo::make_AAC_ADTS_Header(std::list<BYTE>::iterator &it) {
	//unsigned char configParams[] = {0x13, 0x08, 0x56, 0xe5, 0x98};
	uint profile = ((*it) >> 3) & 0x1f;
	uint frequency_index = (((*it) & 0x7) << 1) | (((*++it) >> 7) & 0x1);
	uint channel_config = ((*it++) >> 3) & 0xf;
	uint finallength = 7;
	cout << "___make_AAC_ADTS_Header____" << endl;
	cout << "profile=0x" << profile << endl;
	cout << "frequency_index=0x" << frequency_index << endl;
	cout << "channel_config=0x" << channel_config << endl;
	cout << "***make_AAC_ADTS_Header***" << endl;
	aac_ADTS_header[0] = (unsigned char) 0xff;
	aac_ADTS_header[1] = (unsigned char) 0xf1;
	aac_ADTS_header[2] = (unsigned char) (((profile - 1) << 6)
			+ (frequency_index << 2) + (channel_config >> 2));
	aac_ADTS_header[3] = (unsigned char) (((channel_config & 0x3) << 6)
			+ (finallength >> 11));
	aac_ADTS_header[4] = (unsigned char) ((finallength & 0x7ff) >> 3);
	aac_ADTS_header[5] = (unsigned char) (((finallength & 7) << 5) + 0x1f);
	aac_ADTS_header[6] = (unsigned char) 0xfc;
}

void FlashVideo::set_AAC_ADTS_Header(std::list<BYTE>::iterator &it,
uint encoded_length) {
	uint finallength = encoded_length + 7;
	aac_ADTS_header[4] = (unsigned char) ((finallength & 0x7ff) >> 3);
	aac_ADTS_header[5] = (unsigned char) (((finallength & 7) << 5) + 0x1f);
}

void FlashVideo::parseBuffer(std::list<BYTE> &pDataList) {
	cout << dec << "__parseBuffer__" << endl;
	if (!headerReceived) {
		cout << "FlashVideo::parseBuffer look for header..." << endl;
		if (!findHeader(pDataList)) {
			cout << "ERROR couldn't find a header" << endl;
		}
		headerReceived = true;

	} else {
		cout << "I have the header, parse the package" << endl;
		std::list<BYTE>::iterator it;

//		for (it = pDataList.begin(); it != pDataList.end(); ++it)
//			cout << "..0x" << hex << (int) (unsigned char) *it << ",";
		it = pDataList.begin();

		unsigned int prev_size = get32(it);
		unsigned int packet_type = get8(it);
		unsigned int payload_size = get24(it);
		unsigned int timestamp = get32(it);
		unsigned int stream_id = get24(it);
		dumpPacketInfo(prev_size, packet_type, payload_size, timestamp,
				stream_id);

		cout << "data available:" << pDataList.size();
		cout << " / payload size:" << payload_size << " [ ";
		if ((payload_size + 15) <= pDataList.size()) {
			cout << "FLV audio payload: I have enough data" << endl;
		} else {
			cout << "WARNING: FLV audio payload: NOT_ENOUGH_DATA" << endl;
			return;
		}
		if (packet_type == 0x12) {
			cout << "found AMF Metadata" << endl;

		} else if (packet_type == 0x08) {
			cout << "audio packet found" << endl;

			uint soundFormat = ((*it) & 0xF0) >> 4;
			uint soundRate = ((*it) & 0x0C) >> 2;
			uint soundSize = ((*it) & 0x02) >> 1;
			uint soundType = ((*it) & 0x01);
			cout << hex << "soundFormat=0x" << soundFormat << endl;
			if (soundFormat == 0x0A)
				cout << " (AAC)" << endl;
			cout << "soundRate=0x" << soundRate << endl;
			if (soundRate == 0x03)
				cout << " (44KHz)" << endl;
			if (soundFormat == 0x0A && soundRate != 0x03)
				cout << "ERROR !!! incorrect sound rare for AAC" << endl;
			cout << "soundSize=0x" << soundSize << endl;
			if (soundSize == 0x00)
				cout << " (snd8bit)" << endl;
			else if (soundSize == 0x01)
				cout << " (snd16bit)" << endl;
			else
				cout << "ERROR !!! incorrect soundSize" << endl;
			cout << "soundType=0x" << soundType << endl;
			if (soundType == 0x00)
				cout << " (sndMono)" << endl;
			else if (soundType == 0x01)
				cout << " (sndStereo)" << endl;
			if (soundFormat == 0x0A) {
				// AACAUDIODATA
				uint AACPacketType = ((*(++it)) & 0xFF);
				uint AAC_frame_data_size = payload_size - 2;
				it++;
				if (AACPacketType == 0x00) {
					cout << " found a AAC sequence header" << endl;
					make_AAC_ADTS_Header(it);
					cout << " AAC ADTS generic header:" << endl;
//					hex_print(aac_ADTS_header , 7 );
				} else if (AACPacketType == 0x01) {
					cout << "found a Raw AAC frame data:" << endl;
//					const unsigned char *pRaw_AAC_frame_data = (it + 2);
					cout << dec << " AAC_frame_data_size::"
							<< AAC_frame_data_size << endl;
					cout << " AAC ADTS custom header:" << endl;
					set_AAC_ADTS_Header(it, AAC_frame_data_size);
//					hex_print(INFO, *aac_ADTS_header,7);
					cout << " AAC raw data:" << endl;
//					hex_print(INFO, pRaw_AAC_frame_data, AAC_frame_data_size );

// saving AAC into file
					aacFile.write((const char*) aac_ADTS_header, 7);
					BYTE res;
					for (uint i = 0; i < AAC_frame_data_size; i++) {
						res = *(it++);
						aacFile.write((const char*) &res, 1);
					}
//					fwrite(*aac_ADTS_header, 1, 7, fPointer );
//					fwrite(pRaw_AAC_frame_data, 1, AAC_frame_data_size, fPointer );
				} else
					cout << "ERROR !!! incorrect AACPacketType" << endl;
			}

		} else {
			cout << "ERROR not audio type packet" << endl;
		}
		cout << "removing elems from list. size:" << pDataList.size() << endl;
		cout << "elems to be removed:" << (payload_size + 15) << endl;
		pDataList.begin();
		for (unsigned int i = 0; i < (payload_size + 15); i++)
			pDataList.pop_front();

	}

}

void hex_print(const unsigned char *pv, size_t len) {

	if (NULL == pv)
		cout << "ERROR: hex_print pv is NULL" << endl;
	else {
		size_t i = 0;
		int x = 0;
		char row[100];
		cout << ".0..1..2..3  | ..4..5..6..7" << endl;
		for (; i < len; ++i) {
			if (x == 0) {
				sprintf(row, "%zd..", i);
			}
			sprintf(row, "%s %02x ", row, pv[i]);
			if (x == 3)
				sprintf(row, "%s | ", row);
			if (x == 7) {
				x = 0;
				cout << row << endl;
				;
//				bzero(row, 100);
			} else
				x++;
		}
		cout << row << endl;
	}
	cout << endl;
}

/**
 * find the signature of a FLV file: 0x46 0x4C 0x56
 * returns the header size or negative if the pointer
 * is not aligned to a FLV packet.
 * The header is made of:
 * [46][4c][56][Version][Type][hs][hs][hs][hs][payload...]
 */
bool FlashVideo::findHeader(std::list<BYTE> &pDataList) {

//	for (std::list<BYTE>::iterator it=pDataList.begin(); it != pDataList.end(); ++it)
//		cout <<"..0x" << hex << (int)(unsigned char)*it << ",";

	std::list<BYTE>::iterator it = pDataList.begin();

	cout << "----FlashVideo::findHeader-----" << endl;
	if (*(it++) == 0x46 && *(it++) == 0x4c && *(it++) == 0x56) {
		cout << "found FLV Signature" << endl;
	} else {
		cout << "Didn't find FLV Signature" << endl;
		return false;
	}
	if (*(it++) == 0x01) {
		cout << "found FLV version" << endl;
	} else {
		cout << "didn't find FLV version" << endl;
		return false;
	}
	int flag = *(it++);
	if (flag == 4) {
		cout << "flag is an audio stream" << endl;
	} else {
		cout << "error, flag is not for audio:" << flag << endl;
		return false;
	}
	uint headerSize = get32(it);

	cout << "headerSize=" << headerSize << endl;
	cout << "prima pDataList size=" << pDataList.size() << endl;
// remove elements:
	for (uint x = 0; x < headerSize; x++)
		pDataList.pop_front();
	cout << "dopo pDataList size=" << pDataList.size() << endl;

	return true;
}

} /* namespace std */
