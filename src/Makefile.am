# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/depends.pot


#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = depends

# the application source, library search path, and link libraries
depends_LDFLAGS = $(KDE_RPATH) $(all_libraries)
depends_LDADD = $(LIB_KDEUI)

# this is where the desktop file will go 
shelldesktopdir   = $(kde_appsdir)/Utilities

# this is where the shell's XML-GUI resource file goes
shellrcdir   = $(kde_datadir)/depends

depends_SOURCES = dependencyjob.cpp dependsui.cpp dependsuibase.ui \
	importsexportsjob.cpp initjob.cpp main.cpp
noinst_HEADERS = dependencyjob.h dependsui.h importsexportsjob.h initjob.h
