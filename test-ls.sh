RED='\033[0;31m'
GREEN='\033[0;32m'
RESET='\033[0m'

#make -k
"time" -p ls $@ > .ls.tmp 2> .ls.time.tmp
"time" -p ft_ls $@ > .ft_ls.tmp 2> .ft_ls.time.tmp
echo "Finished running ls and ft_ls, now comparing outputs..."
diff -U 3 .ls.tmp .ft_ls.tmp
if [ $? -eq 0 ]; then
  cat .ft_ls.tmp
  # rm -f .ls.tmp .ft_ls.tmp
  echo "ls time:"
  cat .ls.time.tmp
  echo "ft_ls time:"
  cat .ft_ls.time.tmp
  printf $GREEN"---OK---"$RESET"\n"
  # rm -f .ls.time.tmp .ft_ls.time.tmp
else
  printf $RED"---KO---"$RESET"\n"
fi
