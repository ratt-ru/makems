#
# automake specification for TH_ShMem transport.
#

# Definition of target library
lib_LTLIBRARIES = \
	libshmem.la

AM_CPPFLAGS = \
	-DUSE_PUBLIC_MALLOC_WRAPPERS \
	-DUSE_DL_PREFIX \
	-DHAVE_MMAP=0 \
	-DMORECORE=shmbrk \
	-DMORECORE_CONTIGUOUS=0 \
	-DMORECORE_CANNOT_TRIM=1 \
	-DSHMEM_ALLOC \
	-DHAVE_USR_INCLUDE_MALLOC_H 

libshmem_la_SOURCES = 				\
	$(DOCHDRS)				\
	segbasemap.cc				\
	dlmalloc.c				\
	shmem_alloc.cc

include $(top_srcdir)/Makefile.common
