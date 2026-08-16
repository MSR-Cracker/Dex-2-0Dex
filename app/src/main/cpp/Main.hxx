#ifndef NODEX_MAIN_HXX
#define NODEX_MAIN_HXX

/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * JNI specification, as defined by Sun:
 * http://java.sun.com/javase/6/docs/technotes/guides/jni/spec/jniTOC.html
 *
 * Everything here is expected to be VM-neutral.
 */

#include <jni.h>

/*
 * Created by aanti
 * 8/14/2026 11:40 PM
 *
 *   ⋆    ႔ ႔
 *     ᠸ^ ^ ⸝⸝
 *       |、˜〵
 *       じしˍ,)⁐̤ᐷ
 *
 * Fox Mode 🍺
 */

jstring Native_mStringCxx(JNIEnv *env, jobject thiz);

jint Native_mIntCxx(JNIEnv *env, jobject thiz);

static JNINativeMethod gMethods[] = {
        {
            "mStringCxx",
            "()Ljava/lang/String;",
            reinterpret_cast<void*>(Native_mStringCxx)
        },
        {
            "mIntCxx",
            "()I",
            reinterpret_cast<void*>(Native_mIntCxx)
        }
};



#endif //NODEX_MAIN_HXX
