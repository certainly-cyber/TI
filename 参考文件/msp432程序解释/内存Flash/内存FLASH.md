# 
void *memset(void *s, int ch, size_t n);
函数解释：将s中当前位置后面的n个字节 （typedef unsigned int size_t ）用 ch 替换并返回 s 。
+ eg: memset(simulatedCalibrationData, 0xA5, 4096);
