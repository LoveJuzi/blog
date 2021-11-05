#ifndef OpenGLSingleton_H
#define OpenGLSingleton_H

#include <QOpenGLFunctions_3_3_Core>

#include "utils/Singleton.h"

class OpenGLSingleton : public Singleton<OpenGLSingleton> {
public:
    OpenGLSingleton() {}
    void set(QOpenGLFunctions_3_3_Core* core) { _glcore = core; }
    QOpenGLFunctions_3_3_Core* get() { return _glcore; }

private:
    QOpenGLFunctions_3_3_Core* _glcore = NULL;
};

#define OpenGLInstanceInit(CORE) OpenGLSingleton::getInstance().set(CORE)
#define OpenGLInstance           OpenGLSingleton::getInstance().get()

#endif // OpenGLSingleton_H
