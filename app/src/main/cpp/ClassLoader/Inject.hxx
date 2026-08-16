#ifndef ANTIK_INJECT_HXX
#define ANTIK_INJECT_HXX

#include <jni.h>

namespace Antik::Loader {

/**
 * Handles injection DEX from memory
 */
class DexInjector {
public:
    static jclass Initialize(JNIEnv* env, jobject context, const char* processName);
};

} // namespace Antik::Loader

#endif // ANTIK_INJECT_HXX
