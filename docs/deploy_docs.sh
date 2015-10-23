#!/bin/sh
set +x
doxygen doxyfile
git checkout -b gh-pages
git push --quiet https://$GH_TOKEN@github.com/tatsy/lime.git gh-pages:gh-pages 2> /dev/null
set -x
