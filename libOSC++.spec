name: libOSC++
version: 0.9.3
release: 1
copyright: LGPL
vendor: Stetson University
URL: http://wiretap.stetson.edu/
group: Libraries
packager: Jared Jennings <jjenning@fastmail.fm>
source: http://wiretap.stetson.edu/downloads/%{name}-%{version}.tar.gz
buildroot: /var/tmp/%{name}-%{version}-root

summary: Open Sound Control C++ library
%description
A library for encoding and decoding messages in the Open Sound Control
protocol (which is transport-independent) and for sending and receiving
OSC via TCP and UDP on top of IPv4 or IPv6.

%prep
%setup

%build
./configure --prefix=/usr
make

%install
if [ "$RPM_BUILD_ROOT" != "/" ]; then
	rm -rf $RPM_BUILD_ROOT
fi
make DESTDIR=$RPM_BUILD_ROOT install

%files
%attr(-, root, root) /usr/lib/libOSC++.so.0.0.0
%attr(-, root, root) /usr/lib/libOSC++.so.0


%package devel
requires: libOSC++ = %{version}
group: Development/Libraries
summary: Open Sound Control C++ library development files

%description devel
A library for encoding and decoding messages in the Open Sound Control
protocol (which is transport-independent) and for sending and receiving
OSC via TCP and UDP on top of IPv4 or IPv6.

This package contains the static library files and header files necessary for
developing with libOSC++, and developer documentation.

%files devel
%attr(-, root, root) /usr/lib/libOSC++.so
%attr(-, root, root) /usr/lib/libOSC++.la
%attr(-, root, root) /usr/lib/libOSC++.a
%attr(-, root, root) /usr/include/OSC++
