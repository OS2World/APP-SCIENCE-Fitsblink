for a in *.gif; do \
  b=`basename $a .gif`; \
  giftopnm $a | pnmtops -noturn >$b.eps; \
done