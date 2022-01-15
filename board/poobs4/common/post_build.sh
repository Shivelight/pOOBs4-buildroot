#!/bin/sh

SCRIPTPATH=$(dirname "$0")

for f in $(eval echo "$SCRIPTPATH/post_build_scripts/*.sh"); do
  sh "$f"
done

exit 0
