#pragma once

#include <jni.h>
/* Header for class cdeler_highlight_JNITokenizer */

#ifndef _Included_cdeler_highlight_JNITokenizer
#define _Included_cdeler_highlight_JNITokenizer

class malloc_string
{
private:
	char *m_str;

public:
	explicit malloc_string(char *str) noexcept  : m_str{str}
		{}

	~malloc_string() noexcept
		{
		if (m_str)
			free(m_str);
		}

	char *str()
		{
		return m_str;
		}
};

template<typename j_ref_type_t>
class local_ref
{
private:
	JNIEnv *m_env;
	j_ref_type_t m_obj;
public:
	explicit local_ref(JNIEnv *env, j_ref_type_t ref) noexcept : m_env{env}, m_obj{ref}
		{}

	local_ref(const local_ref &) = delete;

	local_ref(local_ref &&ref) noexcept : m_env{ref.m_env}, m_obj{ref.m_obj}
		{}

	~local_ref() noexcept
		{
		if (m_env && m_obj)
			{
			m_env->DeleteLocalRef(m_obj);
			}
		}

	j_ref_type_t ref() const noexcept
		{
		return m_obj;
		}

};

class tree_sitter_tree_t
{
private:
	TSTree *m_tree;
public:
	explicit tree_sitter_tree_t(TSTree *tree) noexcept : m_tree{tree}
		{}

	tree_sitter_tree_t(const tree_sitter_tree_t &) = delete;

	tree_sitter_tree_t(tree_sitter_tree_t &&) = delete;

	TSTree *tree() const
		{
		return m_tree;
		}

	~tree_sitter_tree_t() noexcept
		{
		if (m_tree)
			{
			ts_tree_delete(m_tree);
			}
		}
};

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     cdeler_highlight_token_JNITokenizer
 * Method:    feed_internal
 * Signature: (Ljava/lang/String;)Lcdeler/highlight/token/AST;
 */
JNIEXPORT jobject JNICALL Java_cdeler_highlight_token_JNITokenizer_feed_1internal
		(JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif