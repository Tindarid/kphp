@kphp_should_fail
/@kphp-serialize is allowed only for classes/
<?php

require_once 'polyfills.php';

/** @kphp-serializable */
interface IntA {
}

/** @kphp-serializable */
class A implements IntA {
}

$a = new A();
instance_serialize($a);
