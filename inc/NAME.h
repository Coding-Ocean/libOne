#ifndef NAME_HD
#define NAME_HD

class NAME{
public:
    NAME();
    ~NAME();

    //char*
    NAME( const char* s );//初期化（変換コンストラクタ）
    void operator=( const char* s );//代入
    int operator==( const char* s ) const;
    int operator!=( const char* s ) const;
    
    //NAME&
    NAME( const NAME& n );//初期化（コピーコンストラクタ）
    void operator=( const NAME& n );//代入
    int operator==( const NAME& n ) const;
    int operator!=( const NAME& n ) const;

    //others
    void operator+=( const char* s );//文字列の連結
    operator char*(){ return S; }//変換関数
    
private:
    char* S=0;
};

#endif