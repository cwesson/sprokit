
run_bad_test() {
	err=`./bin/sprokit $1 2>&1`
	count=$?
	diff "$1.expected" <(echo "$err")
	if [ "$?" -ne "0" ]; then
		echo FAIL $1
		return
	fi
	if [ $count -ne 255 ]; then
		echo FAIL $1
		return
	fi
	echo PASS $1
}

run_good_test() {
	err=`./bin/sprokit $1 2>&1`
	if [ "$?" -ne "0" ]; then
		echo FAIL $1
		return
	fi
	if [ "$err" != "" ]; then
		echo FAIL $1
		return
	fi
	./a.out
	e=$?
	if [ "$e" -ne "$2" ]; then
		echo FAIL $1 $e
		return
	fi
	echo PASS $1
}

run_bad_test "test/bad/modified.spkt"
run_bad_test "test/bad/types.spkt"
run_bad_test "test/bad/scope.spkt"
run_bad_test "test/bad/usebeforedecl.spkt"
run_good_test "test/good/simple.spkt" 42
run_good_test "test/good/arith.spkt" 4
run_good_test "test/good/scope.spkt" 29
run_good_test "test/good/loop.spkt" 3
run_good_test "test/good/with.spkt" 40
run_good_test "test/good/shortcircuit.spkt" 28
