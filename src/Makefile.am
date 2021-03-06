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
typelibdir=${girdir}

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

if CSBINDINGS
SUBDIRS+=sharp/
endif

#
# Binaries and libraries
# - declare
#

pkglib_LTLIBRARIES=\
	libmatcal.la \
	libmatlib.la \
	$(VOID)

pkginclude_HEADERS=\
	arithmetics.h \
	coreext.h \
	core.h \
	expression.h \
	generic.h \
	libmatcal.h \
	libmatcalext.h \
	libmatlib.h \
	number.h \
	numberext.h \
	object.h \
	potency.h \
	rules.h \
	rulesext.h \
	trigonometric.h \
	$(VOID)

noinst_LTLIBRARIES=\
	libast.la \
	libjit.la \
	$(VOID)

noinst_PROGRAMS=\
	matcal \
	$(VOID)

#
# Binaries and libraries
# - sources
#

libmatcal_la_SOURCES=\
	core.c \
	number.c \
	numberconv.c \
	expression.c \
	object.c \
	rules.c \
	$(VOID)
libmatcal_la_CFLAGS=\
	$(GIO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(LIBGML_CFLAGS) \
	$(LIBMPFR_CFLAGS) \
	-DG_LOG_DOMAIN=\"Matcal\" \
	-D__LIBMATCAL_INSIDE__=1 \
	$(VOID)
libmatcal_la_LIBADD=\
	$(GIO_LIBS) \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	$(LIBGML_LIBS) \
	$(LIBMPFR_LIBS) \
	libjit.la \
	$(VOID)
libmatcal_la_LDFLAGS=\
	-rpath ${pkglibdir} \
	-release ${PACKAGE_RELEASE} \
	-version-info ${PACKAGE_VERSION_MAYOR}:${PACKAGE_VERSION_MINOR}:${PACKAGE_VERSION_MICRO} \
	$(VOID)

libmatlib_la_SOURCES=\
	arithmetics.c \
	generic.c \
	potency.c \
	setup.c \
	trigonometric.c \
	$(VOID)
libmatlib_la_CFLAGS=\
	$(GIO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(LIBGML_CFLAGS) \
	$(LIBMPFR_CFLAGS) \
	-DG_LOG_DOMAIN=\"Matlib\" \
	-D__LIBMATLIB_INSIDE__=1 \
	$(VOID)
libmatlib_la_LIBADD=\
	$(GIO_LIBS) \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	$(LIBGML_LIBS) \
	$(LIBMPFR_LIBS) \
	libmatcal.la \
	$(VOID)
libmatlib_la_LDFLAGS=\
	-rpath ${pkglibdir} \
	-release ${PACKAGE_RELEASE} \
	-version-info ${PACKAGE_VERSION_MAYOR}:${PACKAGE_VERSION_MINOR}:${PACKAGE_VERSION_MICRO} \
	$(VOID)

EXTRA_DIST+=\
	node.h \
	$(VOID)

libast_la_SOURCES=\
	node.c \
	$(VOID)
libast_la_CFLAGS=\
	$(GIO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	-DG_LOG_DOMAIN=\"Ast\" \
	-D__LIBAST_INSIDE__=1 \
	$(VOID)
libast_la_LIBADD=\
	$(GIO_LIBS) \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	$(VOID)
libast_la_LDFLAGS=\
	$(VOID)

EXTRA_DIST+=\
	compiler.h \
	compiler_override.h \
	$(VOID)

EXTRA_DIST+=\
	compiler_x86_64.c \
	compiler_x86_64.h \
	$(VOID)

libjit_la_SOURCES=\
	compiler_${host_cpu}.c \
	$(VOID)
libjit_la_CFLAGS=\
	$(GIO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	-I${top_builddir}/build/ \
	-DG_LOG_DOMAIN=\"Jit\" \
	-D__LIBJIT_INSIDE__=1 \
	-D__DASC__=1 \
	$(VOID)
libjit_la_LIBADD=\
	$(GIO_LIBS) \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	libast.la \
	$(VOID)
libjit_la_LDFLAGS=\
	$(VOID)

matcal_SOURCES=\
	matcal.c \
	$(VOID)
matcal_CFLAGS=\
	$(GIO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	$(VOID)
matcal_LDADD=\
	$(GIO_LIBS) \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	libmatcal.la \
	libmatlib.la \
	$(VOID)

#
# .desc.c files support
#

SUFFIXES=.dasc.c .c

DYNASM_CFLAGS=\
	-D DEBUG=\"${DEBUG}\" \
	-D DEVELOPER=\"${DEVELOPER}\" \
	$(VOID)

.dasc.c.c:
	$(DYNASM) $(DYNASM_CFLAGS) -o $@ $<

#
# GObject-Introspection
#

include $(INTROSPECTION_MAKEFILE)

GIR_VERSION=${PACKAGE_VERSION_MAYOR}.${PACKAGE_VERSION_MINOR}

GIR_INCLUDES=\
	Gio-2.0 \
	GLib-2.0 \
	GObject-2.0 \
	GL-1.0 \
	$(VOID)

SUFFIXES+=.typelib .gir

.gir.typelib:
	${INTROSPECTION_COMPILER} --output $@ $<

Matcal-${GIR_VERSION}.gir: sources.list ${top_srcdir}/src/libmatcal.la
	${INTROSPECTION_SCANNER} \
		--nsversion "${GIR_VERSION}" \
		--library "${top_srcdir}/src/libmatcal.la" \
		--warn-all \
		--namespace "Matcal" \
		--cflags-begin \
			-I${top_srcdir}/src/ \
		--cflags-end \
		--libtool "$(LIBTOOL)" \
		--sources-top-dirs=${top_srcdir} \
		--include-first-in-src="config.h" \
		$(addprefix --include , ${GIR_INCLUDES}) \
		--filelist $< --output $@ \
		$(VOID)

gir_DATA=\
	Matcal-${GIR_VERSION}.gir \
	$(VOID)

typelib_DATA=\
	$(gir_DATA:.gir=.typelib) \
	$(VOID)

CLEANFILES+=\
	$(gir_DATA) \
	$(typelib_DATA) \
	$(VOID)
