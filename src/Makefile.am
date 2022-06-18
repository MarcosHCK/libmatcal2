# Copyright 2021-2025 MarcosHCK
# This file is part of libmatcal.
#
# libmatcal is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libmatcal is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with libmatcal. If not, see <http://www.gnu.org/licenses/>.
#

# Keep sync with
# - src/Makefile.am
girdir=${pkgdatadir}/gir/

#
# Some variables
#

PACKAGE_NAME=@PACKAGE_NAME@
PACKAGE_RELEASE=@PACKAGE_RELEASE@
PACKAGE_VERSION_MAYOR=@PACKAGE_VERSION_MAYOR@
PACKAGE_VERSION_MINOR=@PACKAGE_VERSION_MINOR@
PACKAGE_VERSION_MICRO=@PACKAGE_VERSION_MICRO@
PACKAGE_VERSION_BUILD=@PACKAGE_VERSION_BUILD@

EXTRA_DIST=
CLEANFILES=
VOID=

#
# Subdirectories
#

SUBDIRS=\
	$(VOID)

#
# Extras
#

EXTRA_DIST+=\
	clonable.h \
	coreext.h \
	core.h \
	libmatcal.h \
	libmatcalext.h \
	number.h \
	numberext.h \
	object.h \
	$(VOID)

#
# Binaries and libraries
#

pkglib_LTLIBRARIES=\
	libmatcal.la \
	$(VOID)

libmatcal_la_SOURCES=\
	clonable.c \
	closure.c \
	core.c \
	number.c \
	numberconv.c \
	object.c \
	$(VOID)
libmatcal_la_CFLAGS=\
	$(GIO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(LIBGML_CFLAGS) \
	$(LIBMPFR_CFLAGS) \
	-DG_LOG_DOMAIN=\"MatCal\" \
	-D__LIBMATCAL_INSIDE__=1 \
	$(VOID)
libmatcal_la_LIBADD=\
	$(GIO_LIBS) \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	$(LIBGML_LIBS) \
	$(LIBMPFR_LIBS) \
	$(VOID)
libmatcal_la_LDFLAGS=\
	-rpath ${pkglibdir} \
	-release ${PACKAGE_RELEASE} \
	-version-info ${PACKAGE_VERSION_MAYOR}:${PACKAGE_VERSION_MINOR}:${PACKAGE_VERSION_MICRO} \
	$(VOID)

#
# GObject-Introspection
#

include $(INTROSPECTION_MAKEFILE)

GIR_VERSION=${PACKAGE_VERSION_MAYOR}.${PACKAGE_VERSION_MINOR}
GIR_NAMESPACE=Matcal

SUFFIXES=.typelib .gir

.gir.typelib:
	${INTROSPECTION_COMPILER} --output $@ $<

GIR_FILENAME=\
	${GIR_NAMESPACE}-${GIR_VERSION}.gir
	$(VOID)

GIR_INCLUDES=\
	Gio-2.0 \
	GLib-2.0 \
	GObject-2.0 \
	GL-1.0 \
	$(VOID)

${GIR_FILENAME}: sources.list ${pkglib_LTLIBRARIES}
	${INTROSPECTION_SCANNER} \
		--output $@ \
		--nsversion "${GIR_VERSION}" \
		--library "${top_srcdir}/src/libmatcal.la" \
		--warn-all \
		--namespace ${GIR_NAMESPACE} \
		--cflags-begin \
			-I${top_srcdir}/src/ \
			-I${top_srcdir}/src/jit/ \
		--cflags-end \
		--libtool "$(LIBTOOL)" \
		--sources-top-dirs=${top_srcdir} \
		--include-first-in-src="config.h" \
		$(addprefix --include , ${GIR_INCLUDES}) \
		--filelist $< \
		$(VOID)

gir_DATA=\
	${GIR_FILENAME} \
	$(GIR_FILENAME:.gir=.typelib) \
	$(VOID)
