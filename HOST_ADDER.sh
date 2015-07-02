#parametro 1: -a add rutas to host -r restore hosts 2: ruta del archivo con los hosts a agregar

#obtengo los datos nuevos

ip=""
cantHostsPorIp=""
hosts=""

cantIP=0
cantHost=0
while read -r line || [[ -n $line ]]; do
    if [ "$line" != "" ]; then
      j=0
      for word in $line; do
	if [ $j == 0 ]; then
	  ip[$cantIP]=$word
	else
	  hosts[$cantHost]=$word
	  let cantHost=cantHost+1
	fi
	let j=j+1
      done
      let j=j-1
      cantHostsPorIp[$cantIP]=$j
      let cantIP=cantIP+1
    fi
done < "$2"

echo ${ip[@]}
echo ${cantHostsPorIp[@]}
echo ${hosts[@]}
echo "---------------"

#obtengo los datos actuales

ipH=""
cantHostsPorIpH=""
hostsH=""

cantIPH=0
cantHostH=0
while read -r line || [[ -n $line ]]; do
    if [ "$line" != "" ]; then
      j=0
      for word in $line; do
	if [ $j == 0 ]; then
	  ipH[$cantIPH]=$word
	else
	  hostsH[$cantHostH]=$word
	  let cantHostH=cantHostH+1
	fi
	let j=j+1
      done
      let j=j-1
      cantHostsPorIpH[$cantIPH]=$j
      let cantIPH=cantIPH+1
    fi
done < "/etc/hosts"

echo ${ipH[@]}
echo ${cantHostsPorIpH[@]}
echo ${hostsH[@]}
echo "----------"

#saco los del actual que concuerdan con los nombres de host que quiero poner
cantIpSobrevivientes=0
ipSobreviviente=""
hostsipSobreviviente=""

ipActual=0
inicioNamesHost=0
while [ $ipActual -lt $cantIPH ]; do
    names=""
    tmp=${cantHostsPorIpH[$ipActual]}
    let maxNameHost=inicioNamesHost+tmp
    while [ $inicioNamesHost -lt $maxNameHost ]; do
      if [[ names == "" ]]; then
	names="${hostsH[$inicioNamesHost]}"	
      else
	names="$names ${hostsH[$inicioNamesHost]}"
      fi
      let inicioNamesHost=inicioNamesHost+1
    done
    
    i=0
    while [ $i -lt $cantHost ]; do
      tmp=`echo ${hosts[$i]}`
      names=`echo $names | sed "s| $tmp | |g" | sed "s|^$tmp ||g" | sed "s| $tmp\\$||g" | sed "s|^$tmp\\$||g"`
      let i=i+1
    done
    
    if [[ $names != "" ]] ;then
      ipSobreviviente[$cantIpSobrevivientes]=${ipH[$ipActual]}
      hostsipSobreviviente[$cantIpSobrevivientes]=$names
      let cantIpSobrevivientes=cantIpSobrevivientes+1
    fi
   
    let ipActual=ipActual+1
done

#las mesclo con las que quiero poner
ipActual=0
inicioNamesHost=0
while [ $ipActual -lt $cantIP ]; do
    names=""
    tmp=${cantHostsPorIp[$ipActual]}
    let maxNameHost=inicioNamesHost+tmp
    while [ $inicioNamesHost -lt $maxNameHost ]; do
      if [[ $names == "" ]]; then
	names="${hosts[$inicioNamesHost]}"	
      else
	names="$names ${hosts[$inicioNamesHost]}"
      fi
      let inicioNamesHost=inicioNamesHost+1
    done
    
    i=0
    j=""
    while [ $i -lt $cantIpSobrevivientes ]; do
      if [[ ${ip[$ipActual]} == ${ipSobreviviente[$i]} ]]; then
	 j="found"
	 hostsipSobreviviente[$i]="${hostsipSobreviviente[$i]} $names"
      fi
      let i=i+1
    done
    
    if [[ $j != "found" ]]; then
      ipSobreviviente[$cantIpSobrevivientes]=${ip[$ipActual]}
      hostsipSobreviviente[$cantIpSobrevivientes]="$names"
      let cantIpSobrevivientes=cantIpSobrevivientes+1
    fi
   
    let ipActual=ipActual+1
done


#las escribo finalmente
echo > tmp
i=0
while [ $i -lt $cantIpSobrevivientes ]; do
  echo "${ipSobreviviente[$i]} ${hostsipSobreviviente[$i]}" >> tmp
  let i=i+1
done

cat tmp