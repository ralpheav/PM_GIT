<!DOCTYPE html>
<html>
<title> pmgit test page</title>
<body>

<div>this IS A TEST</div>

<?php
//https://github.com/libgit2/php-git
//http://libgit2.github.com/
//http://www.gitguys.com/topics/whats-the-deal-with-the-git-index/
if (!extension_loaded("git2")) { 
    print "skip";
    exit();
}

$newPath = "/home/test_work";
$pmgit = new Git2\PMgit($newPath); 

echo "<br>".$newPath." was initializated by PMgit"
//$commitSha = "6849ae04b4607cb03d4cd4f6ea65b0ba68dc18e7";

?>

<div>end</div>

</body>
</html>
