#!/bin/bash

cd ../

for App in *.app; do
  rsync -rlt --delete \
    --exclude "*.app" \
    --exclude clean/ \
    --exclude "Code Blocks/" \
    --exclude Doxyfile \
    --exclude Frameworks/ \
    --exclude include/ \
    --exclude SConstruct \
    --exclude src/ \
    --exclude "Visual Studio 2008/" \
    --exclude "Visual Studio 2010/" \
    --exclude Xcode/ \
    ./ "$App/Contents/zenilib/"
done
