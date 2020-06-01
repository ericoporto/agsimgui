/*
 * Copyright (C) 2020  Érico Vieira Porto
 *
 * This program is free software. You can use and redistribute it
 *  under the terms and conditions of the MIT License (see LICENCE).
 */

#include "SerialHelper.h"
#include <cassert>

namespace SerialHelper {

	char* IntToChar(int i, char * buf, char* end) {
		assert(buf + sizeof(int) < end);

		*((int*)buf) = i;

		return buf + sizeof(int);
	}

    char* CharToInt(int &i, char * buf) {
        i = *((int*)buf);
        return buf + sizeof(int);
    }

	char* FloatToChar(float f, char * buf, char* end) {
		assert(buf + sizeof(float) < end);

		*((float*)buf) = f;

		return buf + sizeof(float);
	}

    char* CharToFloat(float &f, char * buf) {
        f = *((float*)buf);
        return buf + sizeof(float);
    }

	char* BoolToChar(bool b, char* buf, char* end) {
		assert(buf + sizeof(char) < end);

		if (b) {
			*buf = 1;
		}
		else {
			*buf = 0;
		}
		return buf + sizeof(char);
	}

	char* CharToBool(bool &b, char* buf) {
		if (*buf == 0) {
			b = false;
		}
		else {
			b = true;
		}
		return buf + sizeof(char);
	}




}
