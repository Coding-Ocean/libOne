#include"NAME.h"
class CURVE;
class FILE_BUFFER;
class VECTOR;

class ANIMATION_NODE{
public:
    ANIMATION_NODE();
    void set( FILE_BUFFER& fb );
    ~ANIMATION_NODE();
    const NAME& name() const;
    void update( double frameNumber, //in TREEの持つframeNumberを受け取る 
                 VECTOR* scale, //out 各Nodeのデータを更新する
                 VECTOR* rotate, //out 各Nodeのデータを更新する
                 VECTOR* translate //out 各Nodeのデータを更新する
    ) const;
    void update2( double frameNumber2, //in TREEの持つframeNumber2を受け取る
                 double weight2,
                 VECTOR* scale, //out 各Nodeのデータを更新する
                 VECTOR* rotate, //out 各Nodeのデータを更新する
                 VECTOR* translate //out 各Nodeのデータを更新する
    ) const;
private:
    NAME Name;
    CURVE* Curves;
    int NumCurves;
};