sbin_PROGRAMS     = LimitExec pkgdep

LimitExec_SOURCES = LimitExec.cc
pkgdep_SOURCES    = pkgdep.cc

scriptdir = $(sbindir)
dist_script_SCRIPTS = lofarcvsmv lofarcvsx makedep.sh finddep makedoc checkrun \
		      fillreview makecov \
		      countalllines countlines countpurecode countpurecodecomm

glishdir = $(libexecdir)/glish
dist_glish_SCRIPTS = 

pythondir = $(sbindir)
dist_python_SCRIPTS = makePackage.py \
		      makeClass.py \
                      makeTest.py \
		      processgcov

templatedir = $(datadir)/templates
dist_template_DATA  = templates/header.cc_template \
		      templates/header.h_template \
		      templates/main.cc_template \
		      templates/package_bootstrap_template \
		      templates/package_configure.in_template \
		      templates/package_spec.in_template \
		      templates/package_lofarconf.in_template \
		      templates/package_makefile.am_template \
		      templates/include_makefile.am_template \
		      templates/include_pkg_makefile.am_template \
		      templates/src_makefile.am_template \
	              templates/templated_header.cc_template \
		      templates/templated_header.h_template \
		      templates/templated_header.tcc_template \
		      templates/test.cc_template \
		      templates/test.in_template \
		      templates/test.out_template \
		      templates/test.sh_template \
		      templates/test_makefile.am_template

# Install lofarinit without execute permissions.
# They have to be sourced; fill in the correct path. 
install-data-local:
	@for FIL in lofarinit.sh lofarinit.csh; do \
	  sed -e "s%[@]prefix[@]%$(prefix)%" $(srcdir)/$$FIL.in > $$FIL.tmp; \
	  echo "$(INSTALL_DATA) $$FIL.tmp $(prefix)/$$FIL"; \
	  $(INSTALL_DATA) $$FIL.tmp $(prefix)/$$FIL; \
	  $(RM) $$FIL.tmp; \
	done

uninstall-local:
	@$(RM) $(prefix)/lofarinit.sh $(prefix)/lofarinit.csh


include $(top_srcdir)/Makefile.common
