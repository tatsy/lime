#!/bin/sh
set -x
doxygen doxyfile
cd html
git checkout -b gh-pages
git add --all .
git commit -m "Documentation updated from Travis CI, Build No. $TRAVIS_BUILD_NUMBER."
git push --quiet https://$GH_TOKEN@github.com/tatsy/lime.git gh-pages:gh-pages 2> /dev/null
set +x
