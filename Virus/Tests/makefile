#ALIASES

#A rajouter pour lib dynamiques dans etc/profile ou home/moi/profile : LD_LIBRARY_PATH=mon/chemin/préféré:$LD_LIBRARY_PATH
#https://openclassrooms.com/courses/compilez-sous-gnu-linux

# définition des cibles particulières
.PHONY: clean, mrproper

#paramètres à passer au compilateur C++ 
CXXFLAGS = -Wall -O6 -lpthread -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-network -lsfml-system 

#désactivation des règles implicites
.SUFFIXES:

#compilateur C++
CXX = g++

src = main.cpp
objectFiles = $(src:.cpp=.o)
headerFiles = $(src:.cpp=.h)


#racine du répertoire d'installation (= /usr/local) ;
prefix = /usr/local

#racine pour les binaires
exec_prefix= $(prefix)

#répertoire d'installation des binaires 
bindir = $(exec_prefix)/bin

#répertoire d'installation des librairies
libdir = $(exec_prefix)/lib

#répertoire d'installation des données statiques pour le programme
datadir = $(exec_prefix)/lib

#répertoire d'installation des données modifiables par le programme
statedir = $(prefix)/lib

#répertoire d'installation des en-têtes
includedir =/home/theodore/Bureau/C++/
vpath %.h = /home/theodore/Bureau/C++/
#	$(prefix)/include\

#	/usr/include/OGRE


#répertoire d'installation des fichiers de manuel
mandir= $(prefix)/man

#répertoire d'installation des fichiers de la section x du manuel
manxdir= $(prefix)/manx

#répertoire d'installation des fichiers info
infodir= $(prefix)/info

#répertoire d'installation des fichiers sources
srcdir = /root/Bureau/C++/
vpath %.cpp /root/Bureau/C++/	
#$(prefix)/src\   /home/cyprien/codesC++/CodeBlocks/Tests/
  	

#////////////////////////////////////////////////////////////////////

#MAKE



Saucisse : $(objectFiles) main.o
	$(CXX) $^ -o $@  $(CXXFLAGS)

%.o:%.cpp
	$(CXX) -c  $< -I$(includedir) -o  $@ $(CXXFLAGS)

# -I$(includedir) $(srcdir) -I$(includedir)
# Dim.o:Dim.h
# Vecteur.o:Vecteur.h
# Geometrie.o:Geometrie.h
# RN.o:RN.h
# Geo.o:$(headerFiles)
# Spr.o:Spr.h 
# Cmp.o:Cmp.h


 


# Grand nettoyage  /*.cpp
#	.PHONY : clean
clean:
	rm -rf *.bak rm -rf *.o

 

# Mr Proper
mrproper: clean
	rm -rf Programme
