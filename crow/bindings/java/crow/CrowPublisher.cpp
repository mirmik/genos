#include "crow_CrowPublisher.h"
#include <jni.h>
#include <crow/tower.h>
#include <crow/address.h>
#include <crow/nodes/publisher_node.h>
#include "util.h"

JNIEXPORT jlong JNICALL Java_crow_CrowPublisher_nativeNew
(JNIEnv * env, jobject obj)
{
	auto ptr = new crow::publisher_node();
	return (jlong) ptr;
}

/*
 * Class:     crow_CrowPublisher
 * Method:    publish
 * Signature: ([B)V
 */
JNIEXPORT void JNICALL Java_crow_CrowPublisher_publish___3B
(JNIEnv * env, jobject obj, jbyteArray array)
{
	auto * self = getObject<crow::publisher_node>(env, obj);
	jbyte * olddata = (jbyte *) env-> GetByteArrayElements (array, 0);
	jsize oldsize = env-> GetArrayLength (array);
	self->publish({olddata, oldsize});
}

JNIEXPORT void JNICALL Java_crow_CrowPublisher_set_1crowker_1address
(JNIEnv *env, jobject obj, jstring str)
{
	auto * self = getObject<crow::publisher_node>(env, obj);
	self->set_address(crow::address(jstring2string(env, str)));
}

JNIEXPORT void JNICALL Java_crow_CrowPublisher_set_1theme
(JNIEnv * env, jobject obj, jstring str)
{
	auto * self = getObject<crow::publisher_node>(env, obj);
	self->set_theme(jstring2string(env, str));
}
