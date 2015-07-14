mkdir -p Copy.cp
var=`find ./ -type f | grep "^\./[^\.]\+$" | grep -v nbproject | grep -v Makefile`
for i in "$var";do 
	cp --parents -v -u $i "./Copy.cp"
done

var=`find ./ -type f | grep -e "\.sh" -e "\.conf" -e "\.cfg" -e"ids.txt" | grep -v "~"`
for i in "$var";do 
	cp --parents -v -u $i "./Copy.cp"
done
