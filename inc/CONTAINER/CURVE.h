class FILE_BUFFER;

class CURVE{
public:
    enum TYPE{
        TYPE_ROTATE_X,
        TYPE_ROTATE_Y,
        TYPE_ROTATE_Z,
        TYPE_TRANSLATE_X,
        TYPE_TRANSLATE_Y,
        TYPE_TRANSLATE_Z,
        TYPE_SCALE_X,
        TYPE_SCALE_Y,
        TYPE_SCALE_Z,
    };
    CURVE();
    void set( FILE_BUFFER& fb );
    ~CURVE();
    TYPE type() const;
    void getInterpolation( double t, float* v ) const;    
private:
    TYPE Type;
    struct DATA{
        double frameNumber;
        double value;
        int interporationIdx;
        double leftSlope;
        double rightSlope;
    };
    DATA* Data;//”z—ñ
    int NumData;
};
