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

PHPAPI zend_class_entry *git2_index_entry_class_entry;

static void php_git2_index_entry_free_storage(php_git2_index_entry *object TSRMLS_DC)
{
	if (object->entry != NULL) {
		free(object->entry);
		object->entry = NULL;
	}
	zend_object_std_dtor(&object->zo TSRMLS_CC);
	efree(object);
}

zend_object_value php_git2_index_entry_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;

	PHP_GIT2_STD_CREATE_OBJECT(php_git2_index_entry);
	return retval;
}

static zend_function_entry php_git2_index_entry_methods[] = {
	{NULL,NULL,NULL}
};

void php_git2_index_entry_init(TSRMLS_D)
{
	zend_class_entry ce;
	
	INIT_NS_CLASS_ENTRY(ce, PHP_GIT2_NS, "IndexEntry", php_git2_index_entry_methods);
	git2_index_entry_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	git2_index_entry_class_entry->create_object = php_git2_index_entry_new;

	zend_declare_property_null(git2_index_entry_class_entry, "path", sizeof("path")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(git2_index_entry_class_entry, "oid", sizeof("oid")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(git2_index_entry_class_entry, "dev", sizeof("dev")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(git2_index_entry_class_entry, "ino", sizeof("ino")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(git2_index_entry_class_entry, "mode", sizeof("mode")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(git2_index_entry_class_entry, "uid", sizeof("uid")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(git2_index_entry_class_entry, "gid", sizeof("gid")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(git2_index_entry_class_entry, "file_size", sizeof("file_size")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(git2_index_entry_class_entry, "flags", sizeof("flags")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(git2_index_entry_class_entry, "flags_extended", sizeof("flags_extended")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(git2_index_entry_class_entry, "mtime", sizeof("mtime")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(git2_index_entry_class_entry, "ctime", sizeof("ctime")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);

}