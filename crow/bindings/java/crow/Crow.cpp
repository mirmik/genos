#include <iostream>
#include "crow_Crow.h"

#include "util.h"
#include <crow/tower.h>
#include <crow/warn.h>
#include <crow/gates/udpgate.h>

JavaVM * g_vm;

JNIEXPORT void JNICALL Java_crow_Crow_diagnostic
(JNIEnv *, jclass, jboolean trans)
{
	crow::diagnostic_setup(trans);
}

JNIEXPORT void JNICALL Java_crow_Crow_start_1spin
(JNIEnv *, jclass)
{
	crow::start_spin();
}


JNIEXPORT void JNICALL Java_crow_Crow_join_1spin
(JNIEnv *, jclass)
{
    std::cout << "join_spin" << std::endl;
	crow::join_spin();
}

JNIEXPORT void JNICALL Java_crow_Crow_create_1udpgate
(JNIEnv *, jclass, jint id, jint port)
{
    std::cout << "create_udpgate" << std::endl;
	crow::create_udpgate(id, port);
}


JNIEXPORT void JNICALL Java_crow_Crow_init_1crow_1library
(JNIEnv * env, jclass)
{
    std::cout << "GetJavaVM" << std::endl;
	env->GetJavaVM(&g_vm);
}

JNIEXPORT void JNICALL Java_crow_Crow_stop_1spin
(JNIEnv *, jclass, jboolean join)
{
    std::cout << "stop_spin" << std::endl;
	crow::stop_spin(join);
}

JNIEXPORT void JNICALL Java_crow_Crow_warn
(JNIEnv * env, jclass obj, jstring str)
{
	crow::warn(jstring2string(env, str));
}