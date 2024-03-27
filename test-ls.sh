RED='\033[0;31m'
GREEN='\033[0;32m'
RESET='\033[0m'

DIR="$HOME/42/projects/ft_ls/"

#make -k
echo $@
"time" -p ls $@ > $DIR.ls.tmp 2> $DIR.ls.time.tmp
"time" -p $DIR"ft_ls" $@ > $DIR.ft_ls.tmp 2> $DIR.ft_ls.time.tmp
echo "Finished running ls and ft_ls, now comparing outputs..."
diff -U 3 $DIR.ls.tmp $DIR.ft_ls.tmp > $DIR.diff.tmp
if [ $? -eq 0 ]; then
  cat $DIR.ft_ls.tmp
  # rm -f .ls.tmp .ft_ls.tmp
  echo "ls time:"
  cat $DIR.ls.time.tmp
  echo "ft_ls time:"
  cat $DIR.ft_ls.time.tmp
  printf $GREEN"---OK---"$RESET"\n"
  # rm -f .ls.time.tmp .ft_ls.time.tmp
else
  cat $DIR.diff.tmp
  printf $RED"---KO---"$RESET"\n"
fi
