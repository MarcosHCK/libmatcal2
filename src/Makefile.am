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

#
# Binaries and libraries
# - declare
#

EXTRA_DIST+=\
	libmatcal.h \
	libmatcalext.h \
	libmatlib.h \
	libmatree.h \
	libast.h \
	$(VOID)

pkglib_LTLIBRARIES=\
	libmatcal.la \
	libmatlib.la \
	libmatree.la \
	libast.la \
	$(VOID)

#
# Binaries and libraries
# - sources
#

EXTRA_DIST+=\
	coreext.h \
	core.h \
	number.h \
	numberext.h \
	object.h \
	$(VOID)

libmatcal_la_SOURCES=\
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
	-DG_LOG_DOMAIN=\"Matcal\" \
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

EXTRA_DIST+=\
	arithmetics.h \
	$(VOID)

libmatlib_la_SOURCES=\
	arithmetics.c \
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
	expression.h \
	rules.h \
	rulesext.h \
	$(VOID)

libmatree_la_SOURCES=\
	expression.c \
	rules.c \
	$(VOID)
libmatree_la_CFLAGS=\
	$(GIO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	-DG_LOG_DOMAIN=\"Matree\" \
	-D__LIBMATREE_INSIDE__=1 \
	$(VOID)
libmatree_la_LIBADD=\
	$(GIO_LIBS) \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	libast.la \
	$(VOID)
libmatree_la_LDFLAGS=\
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
	-rpath ${pkglibdir} \
	-release ${PACKAGE_RELEASE} \
	-version-info ${PACKAGE_VERSION_MAYOR}:${PACKAGE_VERSION_MINOR}:${PACKAGE_VERSION_MICRO} \
	$(VOID)

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

SUFFIXES=.typelib .gir

.gir.typelib:
	${INTROSPECTION_COMPILER} --output $@ $<

Matcal-${GIR_VERSION}.gir: matcal.list ${top_srcdir}/src/libmatcal.la
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

Matlib-${GIR_VERSION}.gir: matlib.list ${top_srcdir}/src/libmatlib.la Matcal-${GIR_VERSION}.gir
	${INTROSPECTION_SCANNER} \
		--nsversion "${GIR_VERSION}" \
		--library "${top_srcdir}/src/libmatlib.la" \
		--warn-all \
		--namespace "Matlib" \
		--cflags-begin \
			-I${top_srcdir}/src/ \
		--cflags-end \
		--libtool "$(LIBTOOL)" \
		--sources-top-dirs=${top_srcdir} \
		--include-first-in-src="config.h" \
		$(addprefix --include , ${GIR_INCLUDES}) \
		--include-uninstalled="Matcal-${GIR_VERSION}.gir" \
		--filelist $< --output $@ \
		$(VOID)

Matree-${GIR_VERSION}.gir: matree.list ${top_srcdir}/src/libmatree.la Matcal-${GIR_VERSION}.gir Ast-${GIR_VERSION}.gir
	${INTROSPECTION_SCANNER} \
		--nsversion "${GIR_VERSION}" \
		--library "${top_srcdir}/src/libmatree.la" \
		--warn-all \
		--namespace "Matree" \
		--cflags-begin \
			-I${top_srcdir}/src/ \
		--cflags-end \
		--libtool "$(LIBTOOL)" \
		--sources-top-dirs=${top_srcdir} \
		--include-first-in-src="config.h" \
		$(addprefix --include , ${GIR_INCLUDES}) \
		--include-uninstalled="Matcal-${GIR_VERSION}.gir" \
		--include-uninstalled="Ast-${GIR_VERSION}.gir" \
		--filelist $< --output $@ \
		$(VOID)

Ast-${GIR_VERSION}.gir: ast.list ${top_srcdir}/src/libast.la
	${INTROSPECTION_SCANNER} \
		--nsversion "${GIR_VERSION}" \
		--library "${top_srcdir}/src/libast.la" \
		--warn-all \
		--namespace "Ast" \
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
	Matlib-${GIR_VERSION}.gir \
	Matree-${GIR_VERSION}.gir \
	Ast-${GIR_VERSION}.gir \
	$(VOID)

typelib_DATA=\
	$(gir_DATA:.gir=.typelib) \
	$(VOID)

CLEANFILES=\
	$(gir_DATA) \
	$(typelib_DATA) \
	$(VOID)
