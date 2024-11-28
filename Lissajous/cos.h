#ifndef _COS_H_

#define _COS_H_

#define COS(x) cos_table_rad(x)
#define SIN(X) COS(PI/2-X)


float cos_tailor(float x);
float cos_table_angle(float angle);
float cos_table_rad(float angle);

#endif
