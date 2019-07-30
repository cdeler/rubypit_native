#include <cstring>
#include <tuple>
#include <tree_sitter/api.h>

#include "tree-sitter-ruby.h"
#include "java_interop.h"

using namespace std;
using namespace cdeler::java;

#ifdef __cplusplus
extern "C"
{
#endif
// Declare the `tree_sitter_ruby` function, which is
// implemented by the `tree_sitter_ruby` library.
TSLanguage *tree_sitter_ruby();
#ifdef __cplusplus
}
#endif

static TSParser *parser = nullptr;
static TSTree *previousTree = nullptr;


#ifdef _MSC_VER
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif
BOOL WINAPI DllMain (
	HINSTANCE const instance,  // handle to DLL module
	DWORD     const reason,    // reason for calling function
	LPVOID    const reserved)  // reserved
{
	// Perform actions based on the reason for calling.
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		parser = ts_parser_new();
		ts_parser_set_language(parser, tree_sitter_ruby());

		break;

	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;

	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;

	case DLL_PROCESS_DETACH:
		ts_parser_delete(parser);
		break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
#ifdef __cplusplus
};
#endif

#else

static __attribute__((constructor)) void init()
	{
	parser = ts_parser_new();
	ts_parser_set_language(parser, tree_sitter_ruby());
	}

static __attribute__((destructor)) void deinit()
	{
	ts_parser_delete(parser);
	}

#endif // _MSC_VER

static jobject _walk_over_tree_internal(JNIEnv *env, TSNode node,
                                        tuple<JArrayList, JAST, JSourceToken, JTokenType> &classes)
	{
	auto &[j_array_list, j_ast, j_source_token, j_token_type] = classes;

	auto node_start = ts_node_start_point(node);
	auto node_end = ts_node_end_point(node);

	const char *node_string = ts_node_type(node);

	local_ref<jstring> j_node_type_string{env, env->NewStringUTF(node_string)};

	local_ref<jobject> node_type_enum{env, env->CallStaticObjectMethod(j_token_type.get_class(),
	                                                                   j_token_type.getEnum(),
	                                                                   j_node_type_string.get())};

	local_ref<jobject> source_token_object{env, env->NewObject(j_source_token.get_class(),
	                                                           j_source_token.init(),
	                                                           node_type_enum.get(),
	                                                           node_start.row, node_start.column,
	                                                           node_end.row, node_end.column
	)};

	uint32_t childs_count = ts_node_named_child_count(node);

	local_ref<jobject> child_list{env, env->NewObject(j_array_list.get_class(),
	                                                  j_array_list.init(),
	                                                  childs_count)};

	for (uint32_t i = 0; i < childs_count; ++i)
		{
		local_ref<jobject> child_node{env, _walk_over_tree_internal(env, ts_node_named_child(node, i), classes)};
		env->CallBooleanMethod(child_list.get(), j_array_list.add(), child_node.get());
		}

	jobject ast_object = env->NewObject(j_ast.get_class(),
	                                    j_ast.init(),
	                                    source_token_object.get(),
	                                    child_list.get()
	);

	return ast_object;
	}

static jobject walk_over_tree(JNIEnv *env, TSNode root_node)
	{
	JArrayList j_array_list{env};
	JAST j_ast{env};
	JSourceToken j_source_token{env};
	JTokenType j_token_type{env};

	auto classes = make_tuple(j_array_list, j_ast, j_source_token, j_token_type);

	auto result = _walk_over_tree_internal(env, root_node, classes);
	return result;
	}

#ifdef __cplusplus
extern "C"
{
#endif
JNIEXPORT
jobject JNICALL Java_cdeler_highlight_JNITokenizer_feed_1internal(
		JNIEnv *env,
		jobject thisObject,
		jstring source_code)
	{
	const char *native_string = env->GetStringUTFChars(source_code, nullptr);
	TSTree *tree = ts_parser_parse_string(parser, previousTree, native_string, strlen(native_string));

	TSNode root_node = ts_tree_root_node(tree);

	jobject result = walk_over_tree(env, root_node);

	fflush(stdout);
	previousTree = ts_tree_copy(tree);
	ts_tree_delete(tree);
	env->ReleaseStringUTFChars(source_code, native_string);

	return result;
	}

#ifdef __cplusplus
};
#endif
