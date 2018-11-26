//============================================================================
// Name        : RtmpAacExtractor.cpp
// Author      : Fabio De Lorenzo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "RtmpWrapper.h"
#include "FlashVideo.h"

using namespace std;

int main() {
	string rtmp_url =
			"rtmp://cp154821.live.edgefcs.net/live/Radio24Audio@69140 live=1";
	std::list<BYTE> mRtmpDataList;
	FlashVideo mFlv;

	cout << "!!!Hello World!!!" << endl;
	int i = 0, size, totalread = 0;

	// +++++++++++++++++++ main program ++++++++++++++
	cout << "main program";
	RtmpWrapper mRtmpWrapper;
	mRtmpWrapper.setUri(rtmp_url);
	mRtmpWrapper.connect();
//	mRtmpWrapper.debugSaveFlvToFile("/tmp/lazanzara-fdl.flv");
//	mRtmpWrapper.debugReadFromFlv("lazanzara-fdl.flv");
	mRtmpWrapper.setDataList(&mRtmpDataList);

	while (i < 100) {
		cout << endl << "\n\n-----------------> iteration:" << dec << i << "::"
				<< totalread << endl;
		size = mRtmpWrapper.readData();
		totalread += size;
		for (std::list<BYTE>::iterator it=mRtmpDataList.begin(); it != mRtmpDataList.end(); ++it)
			cout <<"0x" << hex << (int)(unsigned char)*it << ",";
		if (size != 0) {
			try {
				mFlv.parseBuffer(mRtmpDataList);
			} catch (int e) {
				cout << "not enough data in buffer";
			}
		}
		i++;

	}
	// ------------------- main program --------------

	return 0;
}
