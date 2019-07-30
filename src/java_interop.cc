//
// Created by sergei_krotov on 7/30/19.
//
#include <jni.h>

#include "java_interop.h"

using namespace cdeler::java;

JavaClass::JavaClass(JNIEnv *env, const char *classname) : m_env{env}
	{
	m_clazz = static_cast<jclass>(m_env->NewGlobalRef(m_env->FindClass(classname)));
	}

JavaClass::~JavaClass()
	{
	m_env->DeleteGlobalRef(m_clazz);
	}

JArrayList::JArrayList(JNIEnv *env) : JavaClass{env, "java/util/ArrayList"}
	{
	m_init = m_env->GetMethodID(m_clazz, "<init>", "(I)V");
	m_add = m_env->GetMethodID(m_clazz, "add", "(Ljava/lang/Object;)Z");
	}


JAST::JAST(JNIEnv *env) : JavaClass{env, "cdeler/highlight/AST"}
	{
	m_init = m_env->GetMethodID(m_clazz, "<init>", "(Ljava/lang/Object;Ljava/util/List;)V");
	}

JSourceToken::JSourceToken(JNIEnv *env) : JavaClass{env, "cdeler/highlight/SourceToken"}
	{
	m_init = m_env->GetMethodID(m_clazz, "<init>", "(Lcdeler/highlight/TokenType;IIII)V");
	}

JTokenType::JTokenType(JNIEnv *env) : JavaClass{env, "cdeler/highlight/TokenType"}
	{
	m_getEnum = m_env->GetStaticMethodID(m_clazz, "getEnum", "(Ljava/lang/String;)Lcdeler/highlight/TokenType;");
	}
