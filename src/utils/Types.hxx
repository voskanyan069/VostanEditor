#ifndef __UTILS_TYPES_HXX__
#define __UTILS_TYPES_HXX__

typedef enum
{
    HOSTNAME,
    IP_PORT
} ConnType_t;

typedef enum
{
    STD,
    WARN,
    ERROR
} MessageType_t;

struct Dimensions_t
{
    bool show = 0;
    int top = -1;
    int left = -1;
    int width = -1;
    int height = -1;
};

#endif // __UTILS_TYPES_HXX__
