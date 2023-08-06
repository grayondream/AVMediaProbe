#pragma once

template<class T>
T gcd(const T a, const T b) {
	return b == 0 ? a : gcd(b, a%b);
}