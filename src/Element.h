/*
 * Element.h
 *
 *  Created on: Jul 9, 2015
 *      Author: fabio
 */

#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <iostream>
using namespace std;


class Element {
	unsigned char *payload;
	int size;
public:
	Element();
	virtual ~Element();

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

	void removeBytes(int size);

	void dump();

};

#endif /* ELEMENT_H_ */
