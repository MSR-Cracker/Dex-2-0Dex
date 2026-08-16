#include "Inject.hxx"
#include "RAII.hxx"
#include "math.hxx"
#include "../log.h"
#include "../SafeClass/Dex.hxx"

#include <string_view>

#define MAIN_CLASS "com.antik.antik.Antik"
#define MAIN_METHOD "iamnull"
#define ACTIVITY_CLASS "com.msr.nodex.MainActivity"

namespace Antik::Loader {

    static bool sInitialized = false;
    static jclass sActivityClass = nullptr;

    jclass DexInjector::Initialize(JNIEnv* env, jobject context, const char* proc) {

        if (sInitialized) {
            return sActivityClass;
        }

        std::string_view mHex = hexdex;
        auto mDex = DecodeHex(mHex);

        if (mDex.empty()) {
            LOGE("DEX is empty");
            return nullptr;
        }

        JniRef<jbyteArray> mBytes(env, env->NewByteArray((jsize) mDex.size()));
        env->SetByteArrayRegion(mBytes.get(), 0, (jsize) mDex.size(), (jbyte*) mDex.data());

        JniRef<jclass> mByteBufferClass(env, env->FindClass("java/nio/ByteBuffer"));
        jmethodID mWrap = env->GetStaticMethodID(mByteBufferClass.get(), "wrap", "([B)Ljava/nio/ByteBuffer;");
        JniRef<jobject> mBuffer(env, env->CallStaticObjectMethod(mByteBufferClass.get(), mWrap, mBytes.get()));

        JniRef<jclass> mContextClass(env, env->GetObjectClass(context));
        jmethodID mGetClassLoader = env->GetMethodID(mContextClass.get(), "getClassLoader", "()Ljava/lang/ClassLoader;");
        JniRef<jobject> mAppLoader(env, env->CallObjectMethod(context, mGetClassLoader));

        JniRef<jclass> mDexLoaderClass(env, env->FindClass("dalvik/system/InMemoryDexClassLoader"));

        jobject mLoaderObject = nullptr;

        JniRef<jobjectArray> mBuffers(env, env->NewObjectArray(1, mByteBufferClass.get(), mBuffer.get()));

        jmethodID mConstructor = env->GetMethodID(mDexLoaderClass.get(), "<init>", "([Ljava/nio/ByteBuffer;Ljava/lang/ClassLoader;)V");

        if (mConstructor) {
            mLoaderObject = env->NewObject(mDexLoaderClass.get(), mConstructor, mBuffers.get(), mAppLoader.get());
        } else {
            env->ExceptionClear();

            mConstructor = env->GetMethodID(mDexLoaderClass.get(), "<init>", "(Ljava/nio/ByteBuffer;Ljava/lang/ClassLoader;)V");

            mLoaderObject = env->NewObject(mDexLoaderClass.get(), mConstructor, mBuffer.get(), mAppLoader.get());
        }

        JniRef<jobject> mDexLoader(env, mLoaderObject);

        JniRef<jclass> mBaseDexClass(env, env->FindClass("dalvik/system/BaseDexClassLoader"));
        jfieldID mPathListField = env->GetFieldID(mBaseDexClass.get(), "pathList", "Ldalvik/system/DexPathList;");

        JniRef<jobject> mAppPathList(env, env->GetObjectField(mAppLoader.get(), mPathListField));
        JniRef<jobject> mDexPathList(env, env->GetObjectField(mDexLoader.get(), mPathListField));

        JniRef<jclass> mPathListClass(env, env->FindClass("dalvik/system/DexPathList"));
        jfieldID mElementsField = env->GetFieldID(mPathListClass.get(), "dexElements", "[Ldalvik/system/DexPathList$Element;");

        JniRef<jobjectArray> mAppElements(env, (jobjectArray) env->GetObjectField(mAppPathList.get(), mElementsField));
        JniRef<jobjectArray> mDexElements(env, (jobjectArray) env->GetObjectField(mDexPathList.get(), mElementsField));

        jsize mAppCount = env->GetArrayLength(mAppElements.get());
        jsize mDexCount = env->GetArrayLength(mDexElements.get());

        JniRef<jclass> mElementClass(env, env->FindClass("dalvik/system/DexPathList$Element"));
        JniRef<jobjectArray> mElements(env, env->NewObjectArray(mDexCount + mAppCount, mElementClass.get(), nullptr));

        for (jsize i = 0; i < mDexCount; i++) {
            JniRef<jobject> mElement(env, env->GetObjectArrayElement(mDexElements.get(), i));
            env->SetObjectArrayElement(mElements.get(), i, mElement.get());
        }

        for (jsize i = 0; i < mAppCount; i++) {
            JniRef<jobject> mElement(env, env->GetObjectArrayElement(mAppElements.get(), i));
            env->SetObjectArrayElement(mElements.get(), mDexCount + i, mElement.get());
        }

        env->SetObjectField(mAppPathList.get(), mElementsField, mElements.get());

        JniRef<jclass> mClassLoaderClass(env, env->FindClass("java/lang/ClassLoader"));
        jmethodID mLoadClass = env->GetMethodID(mClassLoaderClass.get(), "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

        JniRef<jstring> mActivityName(env, env->NewStringUTF(ACTIVITY_CLASS));

        jclass mActivityLocal = (jclass) env->CallObjectMethod(mAppLoader.get(), mLoadClass, mActivityName.get());

        sActivityClass = (jclass) env->NewGlobalRef(mActivityLocal);
        env->DeleteLocalRef(mActivityLocal);

        JniRef<jstring> mMainName(env, env->NewStringUTF(MAIN_CLASS));
        JniRef<jclass> mMainClass(env, (jclass) env->CallObjectMethod(mAppLoader.get(), mLoadClass, mMainName.get()));

        if (!env->ExceptionCheck() && mMainClass.get()) {
            jmethodID mMethod = env->GetStaticMethodID(mMainClass.get(), MAIN_METHOD, "(Ljava/lang/String;)V");

            if (mMethod) {
                JniRef<jstring> mProcess(env, env->NewStringUTF(proc));
                env->CallStaticVoidMethod(mMainClass.get(), mMethod, mProcess.get());
            }
        }

        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();
        }

        sInitialized = true;
        return sActivityClass;
    }

}