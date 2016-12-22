c_lines=$(find -name "*.c" -print | xargs grep -c . )
c_total=$(find -name "*.c" -print| xargs grep -c . | cut -d ':' -f2)

h_lines=$(find -name "*.h" -print | xargs grep -c .)
h_total=$(find -name "*.h" -print | xargs grep -c .| cut -d ':' -f2)

echo "-----------C Files-----------"
for i in $c_lines
do
	echo $i
done
echo "-----------------------------"

echo "-----------H Files-----------"
for i in $h_lines
do
	echo $i
done
echo "-----------------------------"


for i in $c_total
do
	sum_c=$[$sum_c+$i]
done

for i in $h_total
do
	sum_h=$[$sum_h+$i]
done

echo "-----------Total lines-------"
echo "C lines:", $sum_c
echo "H lines:", $sum_h
echo "Total Lines of Code:", $[$sum_c+$sum_h]



