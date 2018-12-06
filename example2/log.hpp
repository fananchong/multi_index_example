#pragma once

#define TRACEINFO(format, ...) printf(format, ##__VA_ARGS__);printf("\n");
#define TRACEERROR(format, ...) printf(format, ##__VA_ARGS__);printf("\n");
