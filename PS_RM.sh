var=`ps | grep -v bash | grep -v ps |grep -v PID | grep -v grep | grep -v sed | sed 's/^[^0-9]\([0-9]\+\)[^0-9].*$/\1/g'`
for i in $var; do kill $i;done
