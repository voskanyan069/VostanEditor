#ifndef __UTILS_DEFINES_HXX__
#define __UTILS_DEFINES_HXX__

/// URL Paths
#define CREATE_NODE_PATH "/db/save/node?enc=0&local=1" // EDIT
#define UPDATE_NODE_PATH "/db/save/node?id="
#define DELETE_NODE_PATH "/db/delete?id="
#define GET_NODE_PATH "/db/get?id="

/// Headers
#define HEADER_CONTENT_TYPE(X) "Metax-Content-Type:"#X

/// Terminal colors
#define STD_TERMINAL   "\033[32m"
#define WARN_TERMINAL  "\033[33m"
#define ERROR_TERMINAL "\033[31m"
#define RESET_TERMINAL "\033[0m"

/// Default JSON node
#define NODE_JSON "{\"id\":\"#\",\"uuid\":\"#\",\"enc\":\"0\",\"view\":\"1:20:20:20:20\",\"title\":{\"value\":\"#\",\"view\":\"0:undefined:undefined:undefined:undefined\"},\"txt\":{\"value\":\"\",\"view\":\"0:undefined:undefined:undefined:undefined\"},\"img\":{\"value\":\"\",\"view\":\"0:undefined:undefined:undefined:undefined\"},\"script\":\"\",\"viewers\":\"all\",\"tags\":[],\"in\":[],\"out\":[],\"layout\":[],\"created\":\"#0\",\"updated\":\"#0\",\"saved\":0,\"deleted\":0,\"moved\":{\"date\":0,\"domain\":\"\",\"id\":\"\"},\"domain\":\"\"}"

/// Values
#define DEFAULT_DIM {0, -1, -1, -1, -1}
#define RESTORE_DIM {1, -2, -2, -2, -2}
#define DEFAULT_DIM_STR "0:-1:-1:-1:-1"

/// Functions
#define STRING_DIM(X) (( -1 == X ) ? "undefined" : std::to_string(X))
#define DIM2STR(OBJ) STRING_DIM(OBJ.show) + ":" + \
    STRING_DIM(OBJ.top) + ":" + STRING_DIM(OBJ.left) + ":" + \
    STRING_DIM(OBJ.width) + ":" + STRING_DIM(OBJ.height)

/// Deallocate
#define DELETE_PTR(PTR) if ( nullptr != PTR )  delete PTR;

#endif // __UTILS_DEFINES_HXX__
