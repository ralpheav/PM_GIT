/*
 * pmgit.c
 *
 *  Created on: Jan 25, 2013
 *      Author: reav
 */

#include "php_git2.h"
#include <spl/spl_array.h>
#include <zend_interfaces.h>

PHPAPI zend_class_entry *git2_pmgit_class_entry;

void php_git2_pmgit_init(TSRMLS_D);

static zend_object_handlers git2_repository_object_handlers;

#define PMGIT_INIT_VAR(z) \
	ALLOC_ZVAL(z); \
	INIT_PZVAL(z); \
	ZVAL_NULL(z); \

/*
 * free the object created for this interface
 */
static void php_git2_pmgit_free_storage(php_git2_repository *object TSRMLS_DC)
{
	if (object->repository != NULL) {
		git_repository_free( object->repository);
		object->repository = NULL;
	}

	zend_object_std_dtor(&object->zo TSRMLS_CC);
	efree(object);
}

/*
 * create memory space for the object
 */
zend_object_value php_git2_pmgit_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;

	PHP_GIT2_STD_CREATE_OBJECT(php_git2_repository);
	retval.handlers = &git2_repository_object_handlers;
	return retval;
}
/*
 * count element in an array , if is a object count elements
 */
int pmgit_fast_count_ev(zval *value TSRMLS_DC)
{
	long count = 0;

	if (Z_TYPE_P(value) == IS_ARRAY)
	{
		return (int) zend_hash_num_elements(Z_ARRVAL_P(value)) > 0;
	}
	else
	{
		if (Z_TYPE_P(value) == IS_OBJECT) {

			if (Z_OBJ_HT_P(value)->count_elements) {
				Z_OBJ_HT(*value)->count_elements(value, &count TSRMLS_CC);
				return (int) count > 0;
			}
			return 0;
		}
		else
		{
			if (Z_TYPE_P(value) == IS_NULL)
			{
				return 0;
			}
		}
	}
	return 1;
}

size_t fetchCommand(const char* command, char** output)
{
	char* ret;
    FILE *in;
	size_t total_readbytes;
	int command_len;
	php_stream *stream;

	if ((in = VCWD_POPEN(command, "r")) == NULL)
	{
		    php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to execute '%s'", command);
			RETURN_FALSE;
	}

	stream = php_stream_fopen_from_pipe(in, "rb");
    total_readbytes = php_stream_copy_to_mem(stream, &ret, PHP_STREAM_COPY_ALL, 0);
	php_stream_close(stream);
	memcpy(*output,ret,total_readbytes);

    return total_readbytes;
}


ZEND_BEGIN_ARG_INFO_EX(arginfo_git2_pmgit___construct, 0,0,1)
	ZEND_ARG_INFO(0, repositoryPath)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git2_pmgit_add, 0,0,1)
	ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git2_pmgit_commit, 0,0,1)
	ZEND_ARG_INFO(0, files)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git2_pmgit_checkout, 0,0,1)
	ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()


static int php_git2_repository_initialize(zval *object, git_repository *repository TSRMLS_DC)
{
	zval *odb;
	php_git2_repository *m_repository;
	php_git2_odb *m_odb;

	m_repository = PHP_GIT2_GET_OBJECT(php_git2_repository, object);
	m_repository->repository = repository;

	MAKE_STD_ZVAL(odb);
	object_init_ex(odb,git2_odb_class_entry);
	m_odb = PHP_GIT2_GET_OBJECT(php_git2_odb, odb);
	git_repository_odb(&m_odb->odb,repository);

	add_property_string(object, "path", git_repository_path(repository), 1);
	add_property_zval(object, "odb", odb);
	zval_ptr_dtor(&odb);

	return 0;
}


/*
{{{ proto: Git2\PMgit::__construct(string repositoryPath)
*/
PHP_METHOD(git2_pmgit, __construct)
{
	char *repositoryPath;
	int ret;
	int path_len = 0;
	zend_bool is_bare = 1;
	git_repository *repository;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s|b", &repositoryPath, &path_len) == FAILURE) {
		return;
	}

	ret = git_repository_init(&repository, repositoryPath, is_bare);
	if (ret == GIT_OK) {
		zval *object;

		MAKE_STD_ZVAL(object);
		object_init_ex(object, git2_repository_class_entry);
		php_git2_repository_initialize(object, repository TSRMLS_CC);
		RETVAL_ZVAL(object,0,1);
	}
	else
	{
		php_git2_exception_check(ret TSRMLS_CC);
		RETURN_FALSE;
	}
}
/* }}} */

/*
{{{ proto: Git2\PMgit::add(string $file)
*/
PHP_METHOD(git2_pmgit, add)
{
	int                 error = 0;
	char*               string_value = NULL;
	int                 string_lenght = NULL;
	char*				result;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &string_value, &string_lenght) == FAILURE) {
		return -1;
	}
	if (!file)
	{
		//string_value = Z_STRVAL_P( file);
		result = git__malloc(sizeof(char*));
	    strcpy( command, "git add ");
		strcat(command, string_value);
   		fetchCommand( command, result);
	}

    RETURN_LONG(error);
}
/* }}} */


/*{{{ proto: Git2\PMgit::commit(array $files)
*/
PHP_METHOD(git2_pmgit, commit)
{
	char*                  result;
	char*                  s_value = NULL;
	int                    error;
	zval*                  files = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &files) == FAILURE)
	{
		return -1;
	}

	if (!files)
	{
	    //PMGIT_INIT_VAR(files);
	    //array_init(routes);
		zval **zvalue = NULL;
		zval *zvaluep = NULL;
		int array_size = pmgit_fast_count_ev(files);
		if(array_size == 1)
		{
			if (zend_hash_find(Z_ARRVAL_P(z_entry),0,(void **)&zvalue) != FAILURE) {
				zvaluep = *zvalue;
				s_value = Z_STRVAL_P( zvaluep);
			    if( strcmp(s_value,"a") == 0)
			    {
			    	result = git__malloc(sizeof(char*));
			    	const char* command = {"git commit -a\0"};
			    	fetchCommand( command, result);
			    }
			}
	    } else {
	    	unsigned int i = 0;// should be ulong
	    	char* command = git__malloc(sizeof(char*));
	    	strcpy( command, "git commit ");
	    	unsigned int commandSize = strlen( command);
	    	while( i < array_size)
	    	{
	    		if (zend_hash_find(Z_ARRVAL_P(z_entry),(unsigned long)i,(void **)&zvalue) != FAILURE) {
	    			zvaluep = *zvalue;
	    			s_value = Z_STRVAL_P( zvaluep);
	    			strcat(command, s_value);
	    			strcat(command, " ");
	    	    }
	    	}
	    	if(strlen(command) > commandSize) {
	    		result = git__malloc(sizeof(char*));
	    		fetchCommand( command, result);
	    	}
	    }
	}

    RETURN_LONG(error);
}
/* }}} */

/*
{{{ proto: Git2\PMgit::checkout( string $file)
*/
PHP_METHOD(git2_pmgit, checkout)
{
	char*				result;
	char*               file;
	int                 len = 0;
	int                 error = 0;
	//char*               string_value = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file, &len) == FAILURE) {
		return -1;
	}
	if (!file)
	{
		//string_value = Z_STRVAL_P( file);
		result = git__malloc(sizeof(char*));
	    strcpy( command, "git checkout ");
		strcat(command, file);
   		fetchCommand( command, result);
	}

    RETURN_LONG(error);
}
/* }}} */


/*
{{{ proto: Git2\PMgit::log()
*/
PHP_METHOD(git2_pmgit, log)
{
	char*               result;
	char*               command;
	int                 len = 0;
	//zval*               output;

	command = git__malloc(sizeof(char*));
	result = git__malloc(sizeof(char*));
	//string_value = Z_STRVAL_P( file);
	strcpy( command, "git log");
	len = fetchCommand( command, result);

	//MAKE_STD_ZVAL(output);
	//ZVAL_STRING(output, result, 1);
	//RETVAL_ZVAL(output, 0, 1);
	RETVAL_STRINGL(result, len, 0);
}
/* }}} */

/*
{{{ proto: Git2\PMgit::status()
*/
PHP_METHOD(git2_pmgit, status)
{
	char*               result;
	char*               command;
	int                 len = 0;

	command = git__malloc(sizeof(char*));
	result = git__malloc(sizeof(char*));
	//string_value = Z_STRVAL_P( file);
	strcpy( command, "git status");
	len = fetchCommand( command, result);
	RETVAL_STRINGL(result, len, 0);
}
/* }}} */

/*
{{{ proto: Git2\PMgit::count()
*/
PHP_METHOD(git2_pmgit, count)
{
	php_git2_index *m_index;

	m_index     = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	RETURN_LONG(git_index_entrycount(m_index->index));
}
/* }}} */

/*
{{{ proto: Git2\PMgit::writeTree()
*/
PHP_METHOD(git2_pmgit, writeTree)
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
{{{ proto: Git2\PMgit::current()
*/
PHP_METHOD(git2_pmgit, current)
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
{{{ proto: Git2\PMgit::key()
*/
PHP_METHOD(git2_pmgit, key)
{
	php_git2_index *m_index;

	m_index     = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	RETURN_LONG(m_index->offset);
}

/*
{{{ proto: Git2\PMgit::next()
*/
PHP_METHOD(git2_pmgit, next)
{
	php_git2_index *m_index;

	m_index     = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	m_index->offset++;
}

/*
{{{ proto: Git2\PMgit::rewind()
*/
PHP_METHOD(git2_pmgit, rewind)
{
	php_git2_index *m_index;

	m_index     = PHP_GIT2_GET_OBJECT(php_git2_index, getThis());
	m_index->offset = 0;
}

/*
{{{ proto: Git2\PMgit::valid()
*/
PHP_METHOD(git2_pmgit, valid)
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


static zend_function_entry php_git2_pmgit_methods[] = {
	PHP_ME(git2_pmgit, __construct, arginfo_git2_pmgit___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(git2_pmgit, add,         arginfo_git2_pmgit_add,         ZEND_ACC_PUBLIC)
	PHP_ME(git2_pmgit, commit,      arginfo_git2_pmgit_commit,      ZEND_ACC_PUBLIC)
	PHP_ME(git2_pmgit, checkout,    arginfo_git2_pmgit_checkout,    ZEND_ACC_PUBLIC)
	PHP_ME(git2_pmgit, log,         NULL,                           ZEND_ACC_PUBLIC)
	PHP_ME(git2_pmgit, status ,     NULL,                           ZEND_ACC_PUBLIC)
	/* Iterator Implementation */
	PHP_ME(git2_pmgit, count,       NULL,                           ZEND_ACC_PUBLIC)
	PHP_ME(git2_pmgit, current,     NULL,                           ZEND_ACC_PUBLIC)
	PHP_ME(git2_pmgit, key,         NULL,                           ZEND_ACC_PUBLIC)
	PHP_ME(git2_pmgit, next,        NULL,                           ZEND_ACC_PUBLIC)
	PHP_ME(git2_pmgit, rewind,      NULL,                           ZEND_ACC_PUBLIC)
	PHP_ME(git2_pmgit, valid,       NULL,                           ZEND_ACC_PUBLIC)
	PHP_ME(git2_pmgit, writeTree,   NULL,                           ZEND_ACC_PUBLIC)
	{NULL,NULL,NULL}
};

void php_git2_pmgit_init(TSRMLS_D)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, PHP_GIT2_NS, "PMgit", php_git2_pmgit_methods);

	git2_pmgit_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	git2_pmgit_class_entry->create_object = php_git2_pmgit_new;

	memcpy(&git2_repository_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	git2_repository_object_handlers.clone_obj = NULL;

	//zend_class_implements(git2_pmgit_class_entry TSRMLS_CC, 1, spl_ce_Iterator);
}


