####Getting started

It is assumed that docker has been installed and that you have tge following image *jpmaree/ide* availible. If not, then one need to
clone the following repository *git@github.com:PhillipMaree/Docker.git* and builf the image for project *Docker/02_intellij_ide/*. The 
latter can be done by running:

	
	./docker_build.sh

One the image is availible, then the user can mount his project folder (user defined) in a running container of the aforementioned image
by running:

	./docker_run.sh

The user can modify .screenrc to customize the *screen* configuration/control in the interactive terminal console.
