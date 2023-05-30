#!/bin/bash
echo "Test 1"
./s21_cat my_text.txt >> s21_cat.txt
cat my_text.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt

echo "Test 2 -b"
./s21_cat -b my_text.txt >> s21_cat_b.txt
cat -b my_text.txt >> cat_b.txt
diff -s s21_cat_b.txt cat_b.txt
rm s21_cat_b.txt cat_b.txt

echo "Test 3 -e"
./s21_cat -e my_text.txt >> s21_cat_e.txt
cat -e my_text.txt >> cat_e.txt
diff -s s21_cat_e.txt cat_e.txt
rm s21_cat_e.txt cat_e.txt

echo "Test 4 -n"
./s21_cat -n my_text.txt >> s21_cat_n.txt
cat -n my_text.txt >> cat_n.txt
diff -s s21_cat_n.txt cat_n.txt
rm s21_cat_n.txt cat_n.txt

echo "Test 5 -s"
./s21_cat -s my_text.txt >> s21_cat_s.txt
cat -s my_text.txt >> cat_s.txt
diff -s s21_cat_s.txt cat_s.txt
rm s21_cat_s.txt cat_s.txt

echo "Test 6 -t"
./s21_cat -t my_text.txt >> s21_cat_t.txt
cat -t my_text.txt >> cat_t.txt
diff -s s21_cat_t.txt cat_t.txt
rm s21_cat_t.txt cat_t.txt
