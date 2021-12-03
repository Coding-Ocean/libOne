#ifndef NODE_ORIGIN_HD
#define NODE_ORIGIN_HD

#include"VECTOR.h"
#include"MATRIX.h"
#include"NAME.h"
class BATCH;

class NODE_ORIGIN{
public:
    NODE_ORIGIN();
    ~NODE_ORIGIN();
    const NAME& name() const;
    const BATCH* batch() const;
    const VECTOR& scale() const;
    const VECTOR& rotate() const;
    const VECTOR& translate() const;
    const MATRIX& toOriginWorld() const;
    int chounanIdx() const;
    int otoutoIdx() const;
private:
    friend class TREE_ORIGIN; //TREE_ORIGINからはアクセスし放題
    NAME Name;
    BATCH* Batch;
    VECTOR Scale;
    VECTOR Rotate;
    VECTOR Translate;
    MATRIX ToOriginWorld;
    //長男、弟方式のツリー
    int ChounanIdx;
    int OtoutoIdx;
};

#endif