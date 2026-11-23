(set -e; for ((i=1;;i++)); do echo $i; ./gen >in; ./sol <in >out; ./bf <in >ans; diff -Z ans out; done)
