#include <gxx/packager/opackager.h>
#include <gxx/io/fstream.h>
#include <gxx/read.h>

#include <unistd.h>

int main(int argc, char* argv[]) {	
	gxx::io::fstream ofile(STDOUT_FILENO);
	gxx::io::fstream ifile(STDIN_FILENO);

	gxx::opackager pack(ofile);
	std::string text = gxx::readall(ifile);	

	pack.prefix();
	pack.part(text);
	pack.postfix();

	ofile.close();
	ifile.close();
}