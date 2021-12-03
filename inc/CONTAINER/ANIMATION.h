#include"NAME.h"
class ANIMATION_NODE;
class FILE_BUFFER;

class ANIMATION{
public:
    ANIMATION( FILE_BUFFER& fb );
    ~ANIMATION();
    const ANIMATION_NODE* animationNode( const NAME& name ) const;
    const NAME& name() const;
private:    
    NAME Name;
    ANIMATION_NODE* AnimationNodes;
    int NumAnimationNodes;
};
    