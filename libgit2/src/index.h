/*
 * Copyright (C) 2009-2012 the libgit2 contributors
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#ifndef INCLUDE_index_h__
#define INCLUDE_index_h__

#include "fileops.h"
#include "filebuf.h"
#include "vector.h"
#include "tree-cache.h"
#include "git2/odb.h"
#include "git2/index.h"

#define GIT_INDEX_FILE "index"
#define GIT_INDEX_FILE_MODE 0666

struct git_index {
	git_refcount rc;

	char *index_file_path;

	git_futils_filestamp stamp;
	git_vector entries;

	unsigned int on_disk:1;

	unsigned int ignore_case:1;
	unsigned int distrust_filemode:1;
	unsigned int no_symlinks:1;

	git_tree_cache *tree;

	git_vector reuc;

	git_vector_cmp entries_cmp_path;
	git_vector_cmp entries_search;
	git_vector_cmp entries_search_path;
	git_vector_cmp reuc_search;
};

extern void git_index__init_entry_from_stat(struct stat *st, git_index_entry *entry);

extern unsigned int git_index__prefix_position(git_index *index, const char *path);

#endif
