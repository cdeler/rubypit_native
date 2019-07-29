#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <tree_sitter/api.h>
#include <zconf.h>

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

#include "tree-sitter-ruby.h"

struct malloc_string
{
public:
	char *str;

	explicit malloc_string(char *str_) noexcept  : str{str_}
		{}

	~malloc_string() noexcept
		{
		if (this->str)
			free(this->str);
		}
};

static void _walk_over_tree_internal(TSNode node)
	{
	malloc_string node_string{ts_node_string(node)};
	auto node_type = ts_node_type(node);
	auto node_start = ts_node_start_point(node);
	auto node_end = ts_node_end_point(node);

	fprintf(stdout, "Node:\t%s %d %d %d %d\n", node_type,
	        node_start.row, node_start.column, node_end.row, node_end.column);

	uint32_t node_count = ts_node_named_child_count(node);

	for (uint32_t i = 0; i < node_count; ++i)
		{
		_walk_over_tree_internal(ts_node_named_child(node, i));
		}
	}

static void walk_over_tree(TSNode root_node)
	{
	_walk_over_tree_internal(root_node);
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

	walk_over_tree(root_node);

	fflush(stdout);
	previousTree = ts_tree_copy(tree);
	ts_tree_delete(tree);
	env->ReleaseStringUTFChars(source_code, native_string);

	return nullptr;
	}

#ifdef __cplusplus
};
#endif
