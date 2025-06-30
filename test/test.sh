
run_bad_test() {
	err=`./bin/sprokit $1 2>&1`
	if [ "$?" -ne $2 ]; then
		echo FAIL $1
		return
	fi
	diff "$1.expected" <(echo "$err")
	if [ "$?" -ne "0" ]; then
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
	echo PASS $1
}

run_bad_test "test/bad/modified.spkt" 2
run_bad_test "test/bad/types.spkt" 7
run_bad_test "test/bad/scope.spkt" 1
run_bad_test "test/bad/usebeforedecl.spkt" 4
run_good_test "test/good/simple.spkt"
