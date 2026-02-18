
run_bad_test() {
	exp=`wc -l "$1.expected" | awk '{print $1}'`
	err=`./bin/sprokit $1 2>&1`
	count=$?
	diff "$1.expected" <(echo "$err")
	if [ "$?" -ne "0" ]; then
		echo FAIL $1
		return
	fi
	if [ $count -ne $exp ]; then
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

run_bad_test "test/bad/modified.spkt"
run_bad_test "test/bad/types.spkt"
run_bad_test "test/bad/scope.spkt"
run_bad_test "test/bad/usebeforedecl.spkt"
run_good_test "test/good/simple.spkt"
run_good_test "test/good/arith.spkt"
