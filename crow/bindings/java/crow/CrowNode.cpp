#include <jni.h>
#include "crow_CrowNode.h"
#include <crow/tower.h>
#include <crow/proto/node.h>
#include <iostream>
#include "util.h"

JNIEXPORT void JNICALL Java_crow_CrowNode_bind__I
(JNIEnv * env, jobject jobj, jint id)
{
	auto sub = getObject<crow::node>(env, jobj);
	sub->bind(id);
}

JNIEXPORT void JNICALL Java_crow_CrowNode_bind__
(JNIEnv * env, jobject jobj)
{
	auto sub = getObject<crow::node>(env, jobj);
	sub->bind();
}
