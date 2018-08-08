#!/bin/bash
version="0.1.2"

function addEntry {
    echo "$2<trelemorele>$3<trelemorele>$4<trelemorele>$5<trelemorele>$6<trelemorele>$7" >> pass
}

function deleteUser {
:
}

function verifyPassword {
    echo OK;
}
function showCategories {
ARRAY=()
readarray hasela < pass #loads password file to array
    for i in "${hasela[@]}";do
        sep='<trelemorele>' #set a delimiter
        readarray maszyna <<< $(printf '%s\n' "${i//$sep/$'\n'}")  #create array for a single entry
        ARRAY+=("${maszyna[0]}");
        
    done
    echo "${ARRAY[@]}" | tr ' ' '\n' | sort -u | tr '\n' '\n' | awk 'NF'
}


function changePass {

    readarray hasela < pass #loads password file to array
    for i in "${hasela[@]}";do
        sep='<trelemorele>' #set a delimiter
        readarray maszyna <<< $(printf '%s\n' "${i//$sep/$'\n'}")  #create array for a single entry
        category=${maszyna[0]}  #category
        uuid=${maszyna[1]}      #uuid
        name=${maszyna[2]}      #name
        ip=${maszyna[3]}        #ip
        port=${maszyna[4]}      #port
        password=${maszyna[5]}  #password
        if [[ "$uuid" == *$2* ]];then   #match UUID
            
            case $category in
                *"linux"*)
               
            echo "Machine `echo $name | sed ':a;N;$!ba;s/\n/ /g'` is being processed"
            #echo "sshpass -p`echo $password | sed ':a;N;$!ba;s/\n/ /g'` ssh -p `echo $port | sed ':a;N;$!ba;s/\n/ /g'` root@`echo $ip | sed ':a;N;$!ba;s/\n/ /g'` chpasswd $3:$4"
            echo "Changed pass for user $3"
                ;;
                *) : ;; 
            esac
            if [[ "$3" == "root" ]];then
                echo "Updated root password in database";
    
            fi
        fi
    done
    
    
}

function showEntries {
    readarray hasela < pass #loads password file to array
    
    for i in "${hasela[@]}";do
    #echo $i
    sep='<trelemorele>' #set a delimiter
    readarray maszyna <<< $(printf '%s\n' "${i//$sep/$'\n'}")  #create array for a single entry

    category=${maszyna[0]}  #category
    echo $category | grep $2 > /dev/null && echo $i
done
}

function usage {
echo 'Script for changing users passwords on remote systems. Needs to be provided with system specifications, ie. root password, IP and port.'
echo './pass.sh <passdb_secret> <action> <arguments>'
echo './pass.sh <passdb_secret> <se|cp|du|ae> <arguments>'
echo './pass.sh novell se linux : prints entries with category linux'
echo './pass.sh novell cp UEFE-FED2 adam secret : changes adam password to secret on machine with UUID UEFE-FED2'
echo './pass.sh novell ae linux UEFE-FED2 www-srv 192.168.0.4 2222 secret : adds entry for machine www-srv with UUID UEFE-FED2, IP ADDR 192.168.0.4, port 2222, password secret'
}

if [[ $1 == "-h" || $1 == "--help" ]];then
    usage
    exit
elif [[ $1 == "-v" ]];then
    echo $version
    exit
fi
case $2 in
  "se") if [ "$#" -eq 3 ];then showEntries $1 $3;fi        ;;  #secret category
  "sc") if [ "$#" -eq 2 ];then showCategories $1;fi        ;;  #secret
  "cp") if [ "$#" -eq 5 ];then changePass $1 $3 $4 $5;fi            ;;  #secret uuid user new_pw
  "du") if [ "$#" -eq 4 ];then deleteUser $1 $3 $4;fi               ;;  #secret uuid user
  "ae") if [ "$#" -eq 8 ];then addEntry   $1 $3 $4 $5 $6 $7 $8;fi   ;;  #secret category uuid name ip port secret
  "vp") if [ "$#" -eq 2 ];then verifyPassword $1;fi                 ;;  #verify password for database
  *) usage
esac

#####
##USAGE
#####
#./pass.sh <passdb_secret> <action> <arguments>
#./pass.sh novell se linux : prints entries with category linux
#./pass.sh novell cp UEFE-FED2 adam secret : changes adam's password to secret on machine with UUID UEFE-FED2
#./pass.sh novell ae linux UEFE-FED2 www-srv 192.168.0.4 2222 secret : adds entry for machine www-srv with UUID UEFE-FED2, IP ADDR 192.168.0.4, port 2222, password secret
#
#
