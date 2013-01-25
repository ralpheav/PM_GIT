--TEST--
Check for Git2\Config dimension set
--SKIPIF--
<?php if (!extension_loaded("git2")) print "skip"; ?>
--FILE--
<?php
$path = __DIR__ . '/fixtures/testrepo.git/config';
$data = file_get_contents($path);
$path = __DIR__ . '/fixtures/testrepo.git/config.test';
file_put_contents($path,$data);

$config = new Git2\Config($path);
echo $config["core.bare"] . PHP_EOL;
$config["core.bare"] = false;
echo $config["core.bare"] . PHP_EOL;
$config["core.bare"] = true;
echo $config["core.bare"] . PHP_EOL;
$config["core.bare"] = false;
echo $config["core.bare"] . PHP_EOL;
$config["core.bare"] = 1;
echo $config["core.bare"] . PHP_EOL;
$config["core.bare"] = 0;
echo $config["core.bare"] . PHP_EOL;
$config["core.bare"] = "1";
echo $config["core.bare"] . PHP_EOL;
$config["core.bare"] = "0";
echo $config["core.bare"] . PHP_EOL;
try  {
$config["core.bare"] = array();
echo "FAIL\n";
} catch (\Exception $e) {
echo "OK\n";
}
unlink($path);
--EXPECT--
true
false
true
false
1
0
1
0
OK