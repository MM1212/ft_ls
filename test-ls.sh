make
ls $@ > .ls.tmp
./ft_ls $@ > .ft_ls.tmp
diff .ls.tmp .ft_ls.tmp
if [ $? -eq 0 ]; then
  echo "OK"
  cat .ft_ls.tmp
  rm -f .ls.tmp .ft_ls.tmp
else
  echo "KO"
fi
