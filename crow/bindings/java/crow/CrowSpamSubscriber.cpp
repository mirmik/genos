#include "crow_CrowSpamSubscriber.h"
#include <iostream>
#include <jni.h>

#include "Crow_library.h"
#include <crow/nodes/spammer.h>
#include <crow/tower.h>

struct envobj
{
    JNIEnv *env;
    jobject obj;
};

void spam_subscriber_helper(void *privarg, nos::buffer data)
{
    JNIEnv *env;
    int getEnvStat = g_vm->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED)
    {
        std::cout << "GetEnv: not attached" << std::endl;
        if (g_vm->AttachCurrentThread((JNIEnv **)&env, NULL) != 0)
        {
            std::cout << "Failed to attach" << std::endl;
        }
    }
    else if (getEnvStat == JNI_OK)
    {
        //
    }
    else if (getEnvStat == JNI_EVERSION)
    {
        std::cout << "GetEnv: version not supported" << std::endl;
    }
    jclass cls = env->GetObjectClass((jobject)privarg);
    if (cls == NULL)
    {
        std::cout << "Unable to class" << std::endl;
    }

    auto g_mid = env->GetMethodID(cls, "on_message", "([B)V");
    if (g_mid == NULL)
    {
        std::cout << "Unable to get method ref" << std::endl;
    }

    auto jb = env->NewByteArray(data.size());
    env->SetByteArrayRegion(jb, 0, data.size(), (const jbyte *)data.data());

    env->CallVoidMethod((jobject)privarg, g_mid, jb);

    // env->ReleaseByteArrayElements(jb, (jbyte*) data.data(), JNI_ABORT);
    env->DeleteLocalRef(jb);
}

static jobject
getObjectFromObject(JNIEnv *env, jobject obj, const char *fieldName)
{
    jfieldID fid; /* store the field ID */
    jobject i;

    /* Get a reference to obj's class */
    jclass cls = env->GetObjectClass(obj);

    /* Look for the instance field s in cls */
    fid = env->GetFieldID(cls, fieldName, "J");
    if (fid == NULL)
    {
        return 0; /* failed to find the field */
    }

    /* Read the instance field s */
    i = env->GetObjectField(obj, fid);

    return i;
}

static crow::spam_subscriber *getObject(JNIEnv *env, jobject self)
{
    jclass cls = env->GetObjectClass(self);
    if (!cls)
        env->FatalError("GetObjectClass failed");

    jfieldID nativeObjectPointerID = env->GetFieldID(cls, "instance", "J");
    if (!nativeObjectPointerID)
        env->FatalError("GetFieldID failed");

    jlong nativeObjectPointer = env->GetLongField(self, nativeObjectPointerID);
    return reinterpret_cast<crow::spam_subscriber *>(nativeObjectPointer);
}

extern "C" JNIEXPORT jlong JNICALL
Java_crow_CrowSpamSubscriber_nativeNew(JNIEnv *env, jobject obj)
{
    auto ptr = new crow::spam_subscriber(
        igris::make_delegate(spam_subscriber_helper, env->NewGlobalRef(obj)));

    return (jlong)ptr;
}

extern "C" JNIEXPORT void JNICALL
Java_crow_CrowSpamSubscriber_bind(JNIEnv *env, jobject jobj, jint id)
{
    auto sub = getObject(env, jobj);
    sub->bind(id);
}

std::string jstring2string(JNIEnv *env, jstring jStr)
{
    if (!jStr)
        return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes =
        env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray)env->CallObjectMethod(
        jStr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t)env->GetArrayLength(stringJbytes);
    jbyte *pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

extern "C" JNIEXPORT void JNICALL Java_crow_CrowSpamSubscriber_subscribe(
    JNIEnv *env, jobject jobj, jint nid, jstring addr, jint qos, jint ackquant)
{
    auto sub = getObject(env, jobj);
    sub->subscribe(nid, crow::address(jstring2string(env, addr)), qos,
                   ackquant);
}

extern "C" JNIEXPORT void JNICALL Java_crow_CrowSpamSubscriber_resubscribe(
    JNIEnv *env, jobject jobj, jint qos, jint ackquant)
{
    auto sub = getObject(env, jobj);
    sub->resubscribe(qos, ackquant);
}
