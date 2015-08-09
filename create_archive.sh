#!/bin/bash

SRCDIR="sources"
MAJOR=$(grep -m1 PROJECT_VERSION_MAJOR sources/CMakeLists.txt | awk '{print $2}' | sed 's/.$//g')
MINOR=$(grep -m1 PROJECT_VERSION_MINOR sources/CMakeLists.txt | awk '{print $2}' | sed 's/.$//g')
PATCH=$(grep -m1 PROJECT_VERSION_PATCH sources/CMakeLists.txt | awk '{print $2}' | sed 's/.$//g')
VERSION="${MAJOR}.${MINOR}.${PATCH}"

# update submodules
git submodule update --init --recursive

# build widget
ARCHIVE="awesome-widgets"
FILES="AUTHORS CHANGELOG CHANGELOG-RU COPYING"
IGNORELIST="build usr .kdev4 *.kdev4"
# create archive
[[ -e ${ARCHIVE}-${VERSION}-src.tar.xz ]] && rm -f "${ARCHIVE}-${VERSION}-src.tar.xz"
[[ -d ${ARCHIVE} ]] && rm -rf "${ARCHIVE}"
cp -r "${SRCDIR}" "${ARCHIVE}"
for FILE in ${FILES[*]}; do cp -r "$FILE" "${ARCHIVE}"; done
for FILE in ${IGNORELIST[*]}; do rm -rf "${ARCHIVE}/${FILE}"; done
tar cJf "${ARCHIVE}-${VERSION}-src.tar.xz" "${ARCHIVE}"
ln -sf "../${ARCHIVE}-${VERSION}-src.tar.xz" arch
rm -rf "${ARCHIVE}"

# update md5sum
MD5SUMS=$(md5sum ${ARCHIVE}-${VERSION}-src.tar.xz | awk '{print $1}')
sed -i "/md5sums=('[0-9A-Fa-f]*/s/[^'][^)]*/md5sums=('${MD5SUMS}'/" arch/PKGBUILD
sed -i "s/pkgver=[0-9.]*/pkgver=${VERSION}/" arch/PKGBUILD
# clear
find . -type f -name '*src.tar.xz' -not -name "*${VERSION}-src.tar.xz" -exec rm -rf {} \;
find arch -type l -xtype l -exec rm -rf {} \;
