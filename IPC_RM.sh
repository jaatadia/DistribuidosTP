#eliminando memcompartida
var=`ipcs | awk '/^------ Shared Memory Segments --------$/,/^------ Semaphore Arrays --------$/' | grep '^0x[0-9,a-f]\+[^0-9,^a-z][0-9]\+[^0-9,^a-z]\+[a-z]\+[^0-9,^a-z]\+660.*$'| sed 's/^0x[0-9,a-f]\+[^0-9,^a-f]\([0-9]\+\)[^0-9,^a-f].*$/\1/g'`;
for i in $var; do ipcrm -m $i;done

#eliminando semaforos
var=`ipcs | awk '/^------ Semaphore Arrays --------$/,/^------ Message Queues --------$/' | grep '^0x.*$' | sed 's/^0x[0-9,a-f]\+[^0-9,^a-f]\([0-9]\+\)[^0-9,^a-f].*$/\1/g'`;
for i in $var; do ipcrm -s $i;done

#eliminando colas
var=`ipcs | awk '/^------ Message Queues --------$/,/^$/' | grep '^0x.*$' | sed 's/^0x[0-9,a-f]\+[^0-9,^a-f]\([0-9]\+\)[^0-9,^a-f].*$/\1/g'`;
for i in $var; do ipcrm -q $i;done
