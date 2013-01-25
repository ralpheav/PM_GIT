/*
 * The MIT License
 *
 * Copyright (c) 2010 - 2012 Shuhei Tanuma
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "php_git2.h"
#include <spl/spl_array.h>
#include <zend_interfaces.h>

PHPAPI zend_class_entry *git2_index_class_entry;

static void php_git2_index_free_storage(php_git2_index *object TSRMLS_DC)
{
	if (object->index != NULL) {
			object->index = NULL;
	}
	zend_object_std_dtor(&object->zo TSRMLS_CC);
	efree(object);
}

zend_object_value php_git2_index_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;

	PHP_GIT2_STD_CREATE_OBJECT(php_git2_index);
	return retval;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_git2_index___construct, 0,0,1)
	ZEND_ARG_INFO(0, repository)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git2_index_change, 0,0,1)
	ZEND_ARG_INFO(0, repository)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git2_index_add, 0,0,1)
	ZEND_ARG_INFO(0, repository)
ZEND_END_ARG_INFO()

/*ZEND_BEGIN_ARG_INFO_EX(arginfo_git2_index_create, 0,0,1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()*/

/*{{{ proto: Git2\Index::__construct(string $path)
PHP_METHOD(git2_index, __construct)
{
	char *path;
	git_index *index;
	int error, path_len = 0;
	php_git2_index *m_index;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &path, &path_len) == FAILURE) {
		return;
	}
	error = git_index_open(&index, path);
	m_index = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	m_index->index = index;
}*/


/*
{{{ proto: Git2\Index::__construct(Git2\Repository $repository)
*/
PHP_METHOD(git2_index, __construct)
{
    php_git2_repository *m_repository;
	const char*     index_path;
	git_index*      index;
	int             error = 0;
	php_git2_index* m_index;
	zval*           repository;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"O", &repository, git2_repository_class_entry) == FAILURE) {
		return;
	}
    m_repository = PHP_GIT2_GET_OBJECT(php_git2_repository, repository);

	index_path = git_repository_path( m_repository->repository);  //const char * git_repository_path(git_repository *repository);
	error      = git_index_new( &index);                          //int git_index_new(git_index **out);
	zend_throw_exception_ex(NULL, 0 TSRMLS_CC,"1 (error code %d) path %s", error, index_path);
	error      = git_index_write_brand_new( index, index_path);             //int git_index_write(git_index *index);
	zend_throw_exception_ex(NULL, 0 TSRMLS_CC,"2 (error code %d) path %s", error, index_path);
	git_repository_set_index( m_repository->repository, index);//void git_repository_set_index(git_repository *repository, git_index *index);

	m_index = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	m_index->index = index; //set index property's m_index with the index that have been created ns ago.
}
/* }}} */

/*
{{{ proto: Git2\Index::change(Git2\Repository $repository)
*/
PHP_METHOD(git2_index, change)
{
	php_git2_repository *m_repository;
	git_index           *index;
	zval                *repository;
	php_git2_index      *m_index;
	int                 error;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"O", &repository, git2_repository_class_entry) == FAILURE) {
		return -1;
	}
	m_repository = PHP_GIT2_GET_OBJECT(php_git2_repository, repository);
	zend_throw_exception_ex(NULL, 0 TSRMLS_CC,"%d\n(error code %d) ", giterr_last(), 0);
    error = git_repository_index(&index, m_repository->repository);
    zend_throw_exception_ex(NULL, 0 TSRMLS_CC,"%d\n(error code %d) ", giterr_last(), error);
	git_index_read(index); // why?

	m_index = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	m_index->index = index;
    RETURN_LONG(error);
}
/* }}} */


/*{{{ proto: Git2\Index::addEntry(Git2\IndexEntry $entry)
*/
PHP_METHOD(git2_index, add)
{
	git_index*             index;
	int                    error;
	php_git2_index*        m_index;
	php_git2_index_entry*  m_index_entry;
	//git_index_entry*       index_entry;
	zval*                  entry;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"O", &entry, git2_index_entry_class_entry) == FAILURE) {
		return -1;
	}
	m_index_entry = PHP_GIT2_GET_OBJECT(php_git2_index_entry, entry);
	zend_throw_exception_ex(NULL, 0 TSRMLS_CC,"(error code %d) ", giterr_last(), 10);
	m_index = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	zend_throw_exception_ex(NULL, 0 TSRMLS_CC,"(error code %d) ", giterr_last(), 20);
	error = git_index_add(m_index->index, m_index_entry->entry);
	zend_throw_exception_ex(NULL, 0 TSRMLS_CC,"(error code %d) ", giterr_last(), 30);

    RETURN_LONG(error);
}
/* }}} */

/*
{{{ proto: Git2\Index::count()
*/
PHP_METHOD(git2_index, count)
{
	php_git2_index *m_index;
	
	m_index     = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	RETURN_LONG(git_index_entrycount(m_index->index));
}
/* }}} */

/*
{{{ proto: Git2\Index::writeTree()
*/
PHP_METHOD(git2_index, writeTree)
{
	php_git2_index *m_index;
	git_oid tree_oid;
	char oid_out[GIT_OID_HEXSZ+1] = {0};
	int error = 0;
	
	m_index = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	error = git_tree_create_from_index(&tree_oid, m_index->index);

	git_oid_fmt(oid_out, &tree_oid);
	RETVAL_STRINGL(oid_out,GIT_OID_HEXSZ,1);
}
/* }}} */



/* Iterator Implementation */

/*
{{{ proto: Git2\Tree::current()
*/
PHP_METHOD(git2_index, current)
{
	php_git2_index *m_index;
	const git_index_entry *entry;
	zval *z_entry;

	m_index     = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	entry = git_index_get(m_index->index, m_index->offset);
	if (entry == NULL) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0 TSRMLS_CC,
			"specified offset does not exist. %d");
		RETURN_FALSE;
	}
	
	php_git2_create_index_entry(&z_entry, (git_index_entry *)entry TSRMLS_CC);
	RETURN_ZVAL(z_entry, 0, 1);
}

/*
{{{ proto: Git2\Tree::key()
*/
PHP_METHOD(git2_index, key)
{
	php_git2_index *m_index;

	m_index     = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	RETURN_LONG(m_index->offset);
}

/*
{{{ proto: Git2\Tree::next()
*/
PHP_METHOD(git2_index, next)
{
	php_git2_index *m_index;

	m_index     = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	m_index->offset++;
}

/*	
{{{ proto: Git2\Tree::rewind()
*/
PHP_METHOD(git2_index, rewind)
{
	php_git2_index *m_index;

	m_index     = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	m_index->offset = 0;
}

/*
{{{ proto: Git2\Index::valid()
*/
PHP_METHOD(git2_index, valid)
{
	php_git2_index *m_index;
	int entry_count = 0;
	
	m_index     = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	entry_count = git_index_entrycount(m_index->index);
	if (m_index->offset < entry_count) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
/* }}} */


static zend_function_entry php_git2_index_methods[] = {
	PHP_ME(git2_index, __construct, arginfo_git2_index___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	//PHP_ME(git2_index, create,      arginfo_git2_index_create,      ZEND_ACC_PUBLIC)
	PHP_ME(git2_index, count,       NULL,                           ZEND_ACC_PUBLIC)
	PHP_ME(git2_index, change,      arginfo_git2_index_change,      ZEND_ACC_PUBLIC)
	PHP_ME(git2_index, add,         arginfo_git2_index_add,         ZEND_ACC_PUBLIC)
	/* Iterator Implementation */
	PHP_ME(git2_index, current,     NULL,                           ZEND_ACC_PUBLIC)
	PHP_ME(git2_index, key,         NULL,                           ZEND_ACC_PUBLIC)
	PHP_ME(git2_index, next,        NULL,                           ZEND_ACC_PUBLIC)
	PHP_ME(git2_index, rewind,      NULL,                           ZEND_ACC_PUBLIC)
	PHP_ME(git2_index, valid,       NULL,                           ZEND_ACC_PUBLIC)
	PHP_ME(git2_index, writeTree,   NULL,                           ZEND_ACC_PUBLIC)
	{NULL,NULL,NULL}
};

void php_git2_index_init(TSRMLS_D)
{
	zend_class_entry ce;
	
	INIT_NS_CLASS_ENTRY(ce, PHP_GIT2_NS, "Index", php_git2_index_methods);
	git2_index_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	git2_index_class_entry->create_object = php_git2_index_new;
	zend_class_implements(git2_index_class_entry TSRMLS_CC, 1, spl_ce_Iterator);
}
