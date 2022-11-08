#ifndef __UTILS_DEFINES_HXX__
#define __UTILS_DEFINES_HXX__

/// URL Paths
#define CREATE_NODE_PATH "/db/save/node?enc=0&local=1" // EDIT
#define DELETE_NODE_PATH(UUID) "/db/delete?id="#UUID

/// Headers
#define HEADER_CONTENT_TYPE(X) "Metax-Content-Type:"#X

#endif // __UTILS_DEFINES_HXX__
