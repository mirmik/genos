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

#include <gxx/serialize/gbson.h>

int main(int argc, char* argv[]) {	
	gxx::cliopts opts;
	opts.add_string("path", 'p', "output");

	opts.parse(argc - 1, argv + 1);

	std::string opath = opts.get_string("path").unwrap();
	gxx::fprintln("opath: {}", opath);

	gxx::strvec args = opts.get_args();
	gxx::fprintln("args: {}", args);

	gxx::trent tr(gxx::trent::type::array);
	auto& trvec = tr.as_vector();
	for (const auto& s: args) {
		gxx::trent temp;
		
		try {
			temp = std::stol(s);
		} 
		catch(std::invalid_argument e) {
			temp = s;
		}

		trvec.push_back(temp);
	}

	gxx::println(tr);

	std::string dump;
	gxx::gbson::dump(tr, gxx::io::ostringstream(dump));

	debug_print_dump(dump.data(), dump.size());

	gxx::io::serial_port port(opath);	
	gxx::opackager pack(port);
	
	port.debug_output(true);
	//port.print("HelloWorld");

	pack.prefix();
	pack.part(dump);
	pack.postfix();
//
	//ofile.close();
	//ifile.close();
}