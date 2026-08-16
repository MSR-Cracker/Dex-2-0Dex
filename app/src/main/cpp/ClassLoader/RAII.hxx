#ifndef ZEROART_RAII_HXX
#define ZEROART_RAII_HXX

#include <jni.h>

/**
 *
 * @addtogroup
 * RAII helper  references
 */

template <typename T>

class JniRef {

    JNIEnv* env;
    T ref;

public:
    JniRef(JNIEnv* e, T r) : env(e), ref(r) {

    }
                                            ///// this is my  sef JniRef
    ~JniRef() {
        if (ref) env->DeleteLocalRef(ref);
    }

    T get() const {
        return ref;
    }

    /**
     * Support for implicit conversion
     *
     * @return
     */
    // nullptr
    operator T() const {

        return ref;

    }
    JniRef(const JniRef&) = delete;

    JniRef& operator=(const JniRef&) = delete;
};

#endif //ZEROART_RAII_HXX
