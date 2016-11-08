#include "utils.h"

bool over(float value, float boundary) {
	return (value + ERROR_TOLERANCE >= boundary);
}

bool exceed(float value, float boundary) {
	return (value + ERROR_TOLERANCE > boundary);
}
