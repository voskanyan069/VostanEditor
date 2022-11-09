#ifndef __UTILS_DEFINES_HXX__
#define __UTILS_DEFINES_HXX__

/// URL Paths
#define CREATE_NODE_PATH "/db/save/node?enc=0&local=1" // EDIT
#define DELETE_NODE_PATH "/db/delete?id="

/// Headers
#define HEADER_CONTENT_TYPE(X) "Metax-Content-Type:"#X

/// Terminal colors
#define STD_TERMINAL   "\033[32m"
#define WARN_TERMINAL  "\033[33m"
#define ERROR_TERMINAL "\033[31m"
#define RESET_TERMINAL "\033[0m"

/// Deallocate
#define DELETE_PTR(PTR) if ( nullptr != PTR )  delete PTR;

#endif // __UTILS_DEFINES_HXX__
