#include <gxx/packager/opackager.h>
#include <gxx/print.h>
#include <gxx/print/stdprint.h>

#include <gxx/io/fstream.h>
#include <gxx/io/serial_port.h>
#include <unistd.h>

#include <gxx/read.h>
#include <gxx/getopt/cliopts.h>
#include <gxx/string.h>

#include <iostream>
#include <gxx/serialize/serialize.h>

int main(int argc, char* argv[]) {	
	gxx::cliopts opts;
	opts.add_string("path", 'p', "output");

	opts.parse(argc - 1, argv + 1);

	std::string opath = opts.get_string("path").unwrap();
	gxx::fprintln("opath: {}", opath);

	char dump[128];
	gxx::archive::binary_writer archive(dump);
	
	int a = 33;
	int b = 66;

	gxx::serialize(archive, "add");
	gxx::serialize(archive, a);
	gxx::serialize(archive, b);

	auto tpl = std::make_tuple(33,55,66);
	gxx::serialize(archive, tpl);


	debug_print_dump(dump, archive.length());

	gxx::io::serial_port port(opath);	
	gxx::opackager pack(port);
	
	port.debug_output(true);
	//port.print("HelloWorld");

	pack.prefix();
	pack.part(dump, archive.length());
	pack.postfix();
//
	//ofile.close();
	//ifile.close();
}