#!/bin/bash

# Массивы для сравнивания оригинальной команды и тестируемой 

SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""
DIFF2=""


my_command=(
    "./s21_grep"
    )
sys_command=(
    "grep"
    )
 txt=(
   "reflag 2 text.txt"
   "reflag byron text.txt"
   "reflag Sun text2.txt text.txt"
   "reflag , text2.txt text.txt"
 )   
flags=(
    "i"
    "v"
    "c"
    "l"
    "n"
    "h"
    "s"
    "o"
)


echo "\nAUTOTESTS\n"

Autotest(){
    flags_txt=$(echo "$@" | sed "s/reflag/$nflag/")
    "${my_command[@]}" $flags_txt > "${my_command[@]}".log
    "${sys_command[@]}" $flags_txt > "${sys_command[@]}".log
    DIFF="$(diff -s "${my_command[@]}".log "${sys_command[@]}".log)"
    let "COUNTER++"
    # Если файлы идентичны, увеличиваем счетчик успешных тестов и выводим сообщение об успехе
    if [ "$DIFF" == "Files "${my_command[@]}".log and "${sys_command[@]}".log are identical" ]
    then
        let "SUCCESS++"
        echo "$COUNTER - Success $flags_txt"
    # Иначе увеличиваем счетчик неудачных тестов и выводим сообщение об ошибке
    else
        let "FAIL++"
      echo  "$COUNTER - Fail $flags_txt"
    fi
    rm -f "${my_command[@]}".log "${sys_command[@]}".log

    
}

   echo  "1 параметр\n"

for flag in "${flags[@]}"
do
    for txtc in "${txt[@]}"
    do
    nflag="-$flag"
    Autotest "$txtc"
    done 
done

echo  "2 параметра\n"
for flag in "${flags[@]}"
do
    for flag2 in "${flags[@]}"
    do
        if [ $flag != $flag2 ]
        then
            for txtc in "${txt[@]}"
            do
            nflag="-$flag -$flag2"
            Autotest "$txtc"
            done
        fi    
    done 
done

echo  "2 параметра через 1 -\n"
for flag in "${flags[@]}"
do
    for flag2 in "${flags[@]}"
    do
        if [ $flag != $flag2 ]
        then
            for txtc in "${txt[@]}"
            do
            nflag="-$flag$flag2"
            Autotest "$txtc"
            done
        fi    
    done 
done

echo  "Тест для -e"
let "COUNTER++"
./s21_grep -e Sun -e How text.txt > s21_grep.txt
grep  -e Sun -e How text.txt > grep.txt
DIFF2="$(diff -s s21_grep.txt grep.txt)"
# Если файлы идентичны, увеличиваем счетчик успешных тестов и выводим сообщение об успехе
if [ "$DIFF2" == "Files s21_grep.txt and grep.txt are identical" ]
then
    let "SUCCESS++"
    echo "$COUNTER - Success  -e Sun -e How text.txt"
    # Иначе увеличиваем счетчик неудачных тестов и выводим сообщение об ошибке
    else
    let "FAIL++"
    echo  "$COUNTER - Success  -e Sun -e How text.txt"
    fi
rm -f s21_grep.txt grep.txt
let "COUNTER++"
./s21_grep -e Sun -e How text.txt text2.txt> s21_grep.txt
grep  -e Sun -e How text.txt text2.txt> grep.txt
DIFF2="$(diff -s s21_grep.txt grep.txt)"
# Если файлы идентичны, увеличиваем счетчик успешных тестов и выводим сообщение об успехе
if [ "$DIFF2" == "Files s21_grep.txt and grep.txt are identical" ]
then
    let "SUCCESS++"
    echo "$COUNTER - Success  -e Sun -e How text.txt text2.txt"
    # Иначе увеличиваем счетчик неудачных тестов и выводим сообщение об ошибке
    else
    let "FAIL++"
    echo  "$COUNTER - Success  -e Sun -e How text.txt text2.txt"
    fi
rm -f s21_grep.txt grep.txt


echo  "Тест для -f"
let "COUNTER++"
./s21_grep -f pattern.txt text.txt > s21_grep.txt
grep -f pattern.txt text.txt > grep.txt
# diff -s 
DIFF2="$(diff -s s21_grep.txt grep.txt)"

    # # Если файлы идентичны, увеличиваем счетчик успешных тестов и выводим сообщение об успехе
if [ "$DIFF2" == "Files s21_grep.txt and grep.txt are identical" ]
then
    let "SUCCESS++"
    echo "$COUNTER - Success -f pattern.txt text.txt"
    # Иначе увеличиваем счетчик неудачных тестов и выводим сообщение об ошибке
    else
    let "FAIL++"
    echo  "$COUNTER - Success -f pattern.txt text.txt"
    fi
rm -f s21_grep.txt grep.txt

let "COUNTER++"
./s21_grep -f pattern.txt text.txt text2.txt > s21_grep.txt
grep -f pattern.txt text.txt text2.txt > grep.txt
# diff -s 
DIFF2="$(diff -s s21_grep.txt grep.txt)"

    # # Если файлы идентичны, увеличиваем счетчик успешных тестов и выводим сообщение об успехе
if [ "$DIFF2" == "Files s21_grep.txt and grep.txt are identical" ]
then
    let "SUCCESS++"
    echo "$COUNTER - Success -f pattern.txt text.txt text2.txt"
    # Иначе увеличиваем счетчик неудачных тестов и выводим сообщение об ошибке
    else
    let "FAIL++"
    echo  "$COUNTER - Success -f pattern.txt text.txt text2.txt"
    fi
rm -f s21_grep.txt grep.txt

printf "\n"
echo "ALL: $COUNTER"
echo "FAILED: $FAIL"
echo "SUCCESSFUL: $SUCCESS"
printf "\n"

FAIL=0
SUCCESS=0
COUNTER=0
