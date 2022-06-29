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
# Bindings
#

libmatcal.xml: sources.api sources.metadata
	$(GAPI_PARSER) $<
	$(GAPI_FIXUP) \
		--metadata=sources.metadata \
		--api=$@ \
		$(VOID)

glue.c: libmatcal.xml
	$(GAPI_CODEGEN) \
		--outdir MatcalSharp/ \
		--generate $< $(GLIB_SHARP_CFLAGS) \
		--glue-filename glue.c \
		--glue-includes config.h \
		--glue-includes libmatcal.h \
		--glue-includes node.h \
		--gluelib-name libmatcal_glue.dll \
		$(VOID)

CLEANFILES+=\
	libmatcal.xml \
	glue.c \
	$(VOID)

#
# Glue libraries
#

pkglib_LTLIBRARIES=\
	libmatcal_glue.la \
	$(VOID)

libmatcal_glue_la_SOURCES=\
	glue.c \
	$(VOID)
libmatcal_glue_la_CFLAGS=\
	$(GIO_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(GOBJECT_CFLAGS) \
	-I${top_builddir}/src/ \
	-DG_LOG_DOMAIN=\"MatcalGlue\" \
	$(VOID)
libmatcal_glue_la_LIBADD=\
	$(GIO_LIBS) \
	$(GLIB_LIBS) \
	$(GOBJECT_LIBS) \
	../libmatcal.la \
	$(VOID)
libmatcal_glue_la_LDFLAGS=\
	-rpath ${pkglibdir} \
	-release ${PACKAGE_RELEASE} \
	-version-info ${PACKAGE_VERSION_MAYOR}:${PACKAGE_VERSION_MINOR}:${PACKAGE_VERSION_MICRO} \
	$(VOID)

../libmatcal.la:
	$(MAKE) -C .. libmatcal.la
