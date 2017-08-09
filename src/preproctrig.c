#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define TWO_PI (M_PI * 2.0)

#define HALF_PI (M_PI / 2.0)
#define HALF3_PI (HALF_PI * 3.0)

#define c1 0.99999999999999806767
#define c2 -0.4999999999998996568
#define c3 0.04166666666581174292
#define c4 -0.001388888886113613522
#define c5 0.000024801582876042427
#define c6 -0.0000002755693576863181
#define c7 0.0000000020858327958707
#define c8 -0.000000000011080716368

/* approximation taken from http://www.ganssle.com/approx.htm */
#define cosapprox(xx) (c1 + xx * (c2 + xx * (c3 + xx * (c4 + xx * (c5 + xx * (c6 + xx * (c7 + xx * c8)))))))
#define msqr(x) ((x) * (x))
#define mabsf(x) ((x) < 0.0 ? -(x) : (x))
#define quad1(x) (cosapprox(msqr(x)))
#define quad2(x) (-cosapprox(msqr(M_PI - x)))
#define quad3(x) (-cosapprox(msqr(x - M_PI)))
#define quad4(x) (cosapprox(msqr(TWO_PI - x)))
#define mcos(x) ((x) < HALF_PI ? quad1(x) : (x) < M_PI ? quad2(x) : (x) < HALF3_PI ? quad3(x) : quad4(x))

/* technique found in http://lolengine.net/blog/2011/12/20/cpp-lookup-table-generation */
#define S4(i)    S1((i)),   S1((i)+1),     S1((i)+2),     S1((i)+3)
#define S16(i)   S4((i)),   S4((i)+4),     S4((i)+8),     S4((i)+12)
#define S64(i)   S16((i)),  S16((i)+16),   S16((i)+32),   S16((i)+48)
#define S256(i)  S64((i)),  S64((i)+64),   S64((i)+128),  S64((i)+192)
#define S512(i)	 S256((i)), S256((i)+256)

#define TRIG_TABLE_SIZE 0x0200

float COS_TABLE[] = {
#define S1(i) (mcos((((i + 0.5) / TRIG_TABLE_SIZE * TWO_PI))))
	S512(0)
#undef S1
};

#define TRIG_TABLE_MASK (TRIG_TABLE_SIZE - 1)
#define RADIANS_TO_TABLE_INDEX (TRIG_TABLE_SIZE / TWO_PI)

inline float lutsin(float rad) {
	return COS_TABLE[(int) (mabsf(HALF_PI - rad) * RADIANS_TO_TABLE_INDEX) & TRIG_TABLE_MASK];
}
inline float lutcos(float rad) {
	return COS_TABLE[(int) (rad * RADIANS_TO_TABLE_INDEX) & TRIG_TABLE_MASK];
}

int main() {
	printf("%d\n", sizeof(COS_TABLE) / sizeof(COS_TABLE[0]));
	int i;
	for (i = 0; i < TRIG_TABLE_SIZE; i++) {
		double rad = (i + 0.5) / TRIG_TABLE_SIZE * TWO_PI;
		double approx = lutsin(rad);
		double actual = sin(rad);
		double diff = approx - actual;
		diff = (diff / actual) * 100.0;
		printf("%d (%f): %f, %f, %f\n", i, rad, actual, approx, diff);
	}
	getchar();
	return 0;
}
