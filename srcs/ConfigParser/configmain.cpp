# include "./ConfigParser.hpp"

// Teste para ver se funciona;
// precisa do VirtualServer.cpp para compilar


std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string content;
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        file.close();
    } else {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }

    return content;
}

int main(int argc, char **argv) {
	ConfigParser configuration;

	// std::string content = readFile("../../conf/default.conf");
    // std::cout << content << std::endl;

	configuration.setConfigFilePath("../../conf/default.conf");
	configuration.initConfig();


// Printando Vector de Virtual Server, 
// além de mostrar o caminho de cada coisa dentro do Virtual Server

	for (size_t i = 0; i < configuration.getVServers().size(); i++) {
		std::cout << "\nITERAÇÃO " << i << "\n";
		std::cout << configuration.getVServers()[i].getPort() << std::endl;
		std::cout << configuration.getVServers()[i].getServerName() << std::endl;
		std::cout << configuration.getVServers()[i].getBodySize() << std::endl;
		int j = 0;
		while (j < configuration.getVServers()[i].getErrorPage().size()) {
			std::cout << configuration.getVServers()[i].getErrorPage()[j] << std::endl;
			j++;
		}
		int x = 0;
		while (x < configuration.getVServers()[i].getLocation().size()) {
			std::cout << configuration.getVServers()[i].getLocation()[x]._root << std::endl;
			std::cout << configuration.getVServers()[i].getLocation()[x]._index[1] << std::endl;
			x++;
		}
		std::cout << "\n";
	}
}