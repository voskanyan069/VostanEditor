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
#define NODE_JSON "{\"id\":\"#\",\"uuid\":\"#\",\"enc\":\"0\",\"view\":\"1:800:84:227:86\",\"title\":{\"value\":\"#\",\"view\":\"1:54:24:153:34\"},\"txt\":{\"value\":\"\",\"view\":\"0:undefined:undefined:undefined:undefined\"},\"img\":{\"value\":\"\",\"view\":\"0:undefined:undefined:undefined:undefined\"},\"script\":\"\",\"viewers\":\"all\",\"tags\":[],\"in\":[],\"out\":[],\"layout\":[],\"created\":\"#0\",\"updated\":\"#0\",\"saved\":0,\"deleted\":0,\"moved\":{\"date\":0,\"domain\":\"\",\"id\":\"\"},\"domain\":\"\"}"

/// Deallocate
#define DELETE_PTR(PTR) if ( nullptr != PTR )  delete PTR;

#endif // __UTILS_DEFINES_HXX__
