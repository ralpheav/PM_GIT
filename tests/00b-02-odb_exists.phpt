--TEST--
Check for Git2\ODB::exists
--SKIPIF--
<?php if (!extension_loaded("git2")) print "skip"; ?>
--FILE--
<?php
$path = __DIR__ . "/mock/001-01/";
$repo = new Git2\Repository($path);
if ($repo->odb->exists("ab68c54212af15d3545c41057e3a8f2f9ff6fd0d")) {
	echo "OK" . PHP_EOL;
} else {
	echo "FAIL" . PHP_EOL;
}

if ($repo->odb->exists("unabailablehashid00000000000000000000000")) {
	echo "FAIL" . PHP_EOL;
} else {
	echo "OK" . PHP_EOL;
}

--EXPECT--
OK
OK
