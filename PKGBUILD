# Maintainer: Evgeniy "arcanis" Alexeev <arcanis.arch at gmail dot com>

pkgname=kdeplasma-applets-pytextmonitor
_pkgname=pytextmonitor
pkgver=1.7.3
pkgrel=1
pkgdesc="Minimalistic Plasmoid script written on Python2. It looks like widgets in Awesome WM"
arch=('i686' 'x86_64')
url="http://arcanis.name/projects/pytextmonitor"
license=('GPLv3')
depends=('kdebase-workspace' 'kdebindings-python2' 'lm_sensors' 'sysstat')
optdepends=("hddtemp: for HDD temperature monitor" 
            "catalyst: for GPU monitor" 
            "nvidia-utils: for GPU monitor" 
            "amarok: for music player monitor" 
            "mpd: for music player monitor" 
            "qmmp: for music player monitor")
makedepends=('automoc4' 'cmake')
source=(https://github.com/arcan1s/pytextmonitor/releases/download/V.${pkgver}/${_pkgname}-${pkgver}-src.tar.xz)
install=${pkgname}.install
md5sums=('74175a65186daa6d4a68772a3bcdd368')
backup=('usr/share/config/extsysmon.conf')

prepare() {
  [[ -d ${srcdir}/build ]] && rm -rf "${srcdir}/build"
  mkdir "${srcdir}/build"
}

build () {
  cd "${srcdir}/build"
  cmake -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=$(kde4-config --prefix) \
        "../${_pkgname}"
  make
}

package() {
  cd "${srcdir}/build"
  make DESTDIR="${pkgdir}" install
}
