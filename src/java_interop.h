//
// Created by sergei_krotov on 7/30/19.
//


#pragma once

#ifndef RUBYPIT_JAVA_INTEROP_H
#define RUBYPIT_JAVA_INTEROP_H

namespace cdeler
{
	namespace java
	{


		class JavaClass
		{
		protected:
			explicit JavaClass(JNIEnv *env, const char *classname);

			JNIEnv *m_env;
			jclass m_clazz;
		public:

			virtual ~JavaClass();

			jclass get_class() const
				{
				return m_clazz;
				}
		};

		class JArrayList : public JavaClass
		{
		private:
			jmethodID m_init;
			jmethodID m_add;

		public:
			explicit JArrayList(JNIEnv *env);

			jmethodID init() const noexcept
				{
				return m_init;
				}

			jmethodID add() const noexcept
				{
				return m_add;
				}

		};

		class JAST : public JavaClass
		{
		private:
			jmethodID m_init;

		public:
			explicit JAST(JNIEnv *env);

			jmethodID init() const noexcept
				{
				return m_init;
				}

		};

		class JSourceToken : public JavaClass
		{
		private:
			jmethodID m_init;

		public:
			explicit JSourceToken(JNIEnv *env);

			jmethodID init() const noexcept
				{
				return m_init;
				}

		};

		class JTokenType : public JavaClass
		{
		private:
			jmethodID m_getEnum;
		public:
			explicit JTokenType(JNIEnv *env);

			jmethodID getEnum() const noexcept
				{
				return m_getEnum;
				}
		};
	}
}
#endif //RUBYPIT_JAVA_INTEROP_H
