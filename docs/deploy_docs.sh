#!/bin/sh
doxygen doxyfile
git branch -b gh-pages
git push --quiet https://$GH_TOKEN@github.com/tatsy/lime.git gh-pages:gh-pages 2> /dev/null
