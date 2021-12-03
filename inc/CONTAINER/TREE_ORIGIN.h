#ifndef TREE_ORIGIN_HD
#define TREE_ORIGIN_HD

#include"NAME.h"
class FILE_BUFFER;
class CONTAINER;
class NODE_ORIGIN;

class TREE_ORIGIN{
public:
    TREE_ORIGIN( FILE_BUFFER& fb, const CONTAINER* container );
    ~TREE_ORIGIN();
    const NODE_ORIGIN* nodeOrigins( int idx ) const;
    const int numNodeOrigins() const;
    const NAME& name() const;
private:
    void SetNodeOrigin( FILE_BUFFER& fb, const CONTAINER* container );
    NODE_ORIGIN* NodeOrigins;
    int NumNodeOrigins;
    int Idx;
    NAME Name;
};

#endif
