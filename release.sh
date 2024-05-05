#!/bin/bash

read -p "Enter release (eg v0.0.0): " release
echo "Creating release $release"
git tag -f -s -m "Release $release" $release
git push origin # without --follow-tags so it doesn't push the tag if it fails
git push --follow-tags origin
gh release create $release --generate-notes ./build/vband.uf2
