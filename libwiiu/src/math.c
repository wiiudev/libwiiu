#include "math.h"

long abs(long value) {
	if (value < 0) {
		return -value;
	}
	else {
		return value;
	}
}