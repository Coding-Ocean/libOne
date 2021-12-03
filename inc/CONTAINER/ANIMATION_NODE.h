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
    void update( double frameNumber, //in TREE�̎���frameNumber���󂯎�� 
                 VECTOR* scale, //out �eNode�̃f�[�^���X�V����
                 VECTOR* rotate, //out �eNode�̃f�[�^���X�V����
                 VECTOR* translate //out �eNode�̃f�[�^���X�V����
    ) const;
    void update2( double frameNumber2, //in TREE�̎���frameNumber2���󂯎��
                 double weight2,
                 VECTOR* scale, //out �eNode�̃f�[�^���X�V����
                 VECTOR* rotate, //out �eNode�̃f�[�^���X�V����
                 VECTOR* translate //out �eNode�̃f�[�^���X�V����
    ) const;
private:
    NAME Name;
    CURVE* Curves;
    int NumCurves;
};