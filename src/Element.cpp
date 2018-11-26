/*
 * Element.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: fabio
 */
#include "Element.h"
using namespace std;

Element::Element() {
	payload = 0;
	size = 0;
}

Element::removeBytes(int size) {

}


void Element::dump() {
	for (int i=0; i < size; i++) {
		cout << " 0x" << hex << (int)payload[i];
	}
}

Element::~Element() {
//	if (payload != 0)
//		delete (payload);
}


