ACLOCAL_AMFLAGS = --install -I m4

SUBDIRS = lib src
DIST_SUBDIRS = lib src

if HAVE_UTILS
SUBDIRS += utils
endif

appicondir = $(datadir)/pixmaps
appicon_DATA = extras/logos/naev.png
Graphicsdir = $(datadir)/applications
Graphics_DATA = naev.desktop

dist_man6_MANS = naev.6
dist_doc_DATA = AUTHORS README TODO LICENSE

DATA_ARCHIVE = ndata
PACK	     = utils/pack/pack$(EXEEXT)
NAEV	     = naev$(EXEEXT)

EXTRA_DIST = LICENSE extras naev.desktop
CLEANFILES = $(DATA_ARCHIVE) $(NAEV)

.PHONY: docs help install-ndata VERSION

all-local: $(NAEV)

$(NAEV):
	@echo "  LN    $(NAEV)"
	$(LN_S) "src/$(NAEV)" "$(NAEV)"

$(PACK):
	$(MAKE) -C utils/pack

VERSION:
	echo $(VERSION) > $@

if HAVE_UTILS
$(DATA_ARCHIVE): $(PACK) VERSION
	@echo " PACK   $(DATA_ARCHIVE)"
	cd "$(top_srcdir)" && \
	$(abs_top_builddir)/$(PACK) "$(abs_top_builddir)/$(DATA_ARCHIVE)" \
	  AUTHORS VERSION \
	  $$(find ai      -type f -name '*.lua'; \
	     find gfx     -type f -name '*.png'; \
	     find snd     -type f \
	          \( -name '*.ogg' -o -name '*.wav' -o -name '*.lua' \); \
	     find scripts -type f -name '*.lua'; \
	     find dat     -type f);
else
$(DATA_ARCHIVE):
	@echo "***"
	@echo "*** You must clone the full git repo to create $(DATA_ARCHIVE)"
	@echo "***"
	@false
endif

install-ndata: $(DATA_ARCHIVE)
	ndata="$(DESTDIR)$(ndata_path)"; \
	dir=$$(echo "$$ndata" | $(SED) 's/\(.*\)[\/\\].*/\1/'); \
	echo " $(MKDIR_P) '$$dir'"; \
	$(MKDIR_P) "$$dir"; \
	echo " $(INSTALL_DATA) -c '$(DATA_ARCHIVE)' '$$ndata'"; \
	$(INSTALL_DATA) -c "$(DATA_ARCHIVE)" "$$ndata"

install-data-hook:
	if test -e $(top_srcdir)/dat && test -e $(top_srcdir)/gfx; then \
	  $(MAKE) install-ndata; \
	fi

uninstall-hook:
	echo " rm -rf '$(DESTDIR)$(pkgdatadir)'"; \
	echo " rm -rf '$(docdir)'"; \
	rm -rf "$(DESTDIR)$(pkgdatadir)"
	rm -rf "$(docdir)"

docs:
	$(MAKE) -C docs

help:
	@echo "Possible targets are:"
	@echo "        all - builds everything"
	@echo "      ndata - creates the ndata file"
	@echo "       docs - creates the doxygen documentation"
	@echo "      clean - removes binaries and object files"
	@echo "    install - installs naev"
	@echo "  uninstall - removes previously installed files"
	@echo " dist-bzip2 - creates a source tarball"
	@echo "  distcheck - checks if the created tarball works properly"
	@echo "  distclean - does clean and removes files created by configure"
