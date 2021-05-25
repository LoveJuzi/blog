#include <iostream>
#include <string.h>

using namespace std;

class String {
public:
    String();
    String(char * sch);
    String(const String & oth);
    String & operator = (const String & oth);
    ~String();

    void SetString(char * sch);

private:
    void NewString(int size);
    void ReleaseString();

private:
    char * m_ch; // 存储实际的字符串
    int m_len;   // 存储当前字符串的长度，此长度不含有'\0'
};

String::String() : m_len(-1), m_ch(nullptr) {
}

String::String(char *sch) : m_len(-1), m_ch(nullptr) {
    this->SetString(sch);
}

String::String(const String & oth) : m_len(-1), m_ch(nullptr) {
    this->SetString(oth.m_ch);
}

String & String::operator=(const String &oth) {
    if (this == &oth) {
        return *this;
    }
    this->SetString(oth.m_ch);
    return *this;
}

String::~String() {
    this->ReleaseString();
}

// 引入一个字符串
void String::SetString(char * sch) {
    this->ReleaseString();
    if (this->sch != nullptr) {
        this->NewString(strlen(sch));
        strcpy(this->m_ch, sch);
    }
}

// 引入辅助函数，创建一片字符串存储空间
void NewString(int size) {
    this->m_len = size;
    this->m_ch = new char[size+1];
    this->m_ch[size] = '\0';
}

// 引入辅助函数，释放原有的内存
void String::ReleaseString() {
    if (this->m_ch != nullptr) {
        delete [] this->m_ch;
        this->m_ch = nullptr;
    }

    this->m_len = -1;
}

int main() {

}